export ROOT := $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))

RECURSIVETARGETS := all clean
SET0 := util hsssec
SET1 := cdf ctf pcrf
SET2 := hss hssgtw
# Use 'make WHAT=cdf' to compile cdf only
WHAT ?= $(SET0) $(SET1) $(SET2)

$(RECURSIVETARGETS): $(WHAT)
$(SET0):
	$(MAKE) -C $@ $(MAKECMDGOALS)
$(SET1): util
	$(MAKE) -C $@ $(MAKECMDGOALS)
$(SET2): util hsssec
	$(MAKE) -C $@ $(MAKECMDGOALS)

docker-%: ORG ?= omecproject
docker-%: COMMIT = $(shell git rev-parse --short HEAD)
docker-%: TAG_SUFFIX = $(shell (git status --porcelain | grep -q .) && echo '-dirty')
docker-%: TAG ?= $(COMMIT)$(TAG_SUFFIX)
docker-%: IMAGES ?= build $(SET1) $(SET2) pcef tdf tssf hssdb

# https://docs.docker.com/engine/reference/commandline/build/#specifying-target-build-stage---target
docker-build:
	for img in $(IMAGES); do \
		docker build --rm --progress=plain --target=$$img -t $(ORG)/c3po-$$img:$(TAG) $(DOCKER_BUILD_ARGS) . || exit 1; \
	done;

docker-push:
	for img in $(IMAGES); do \
		docker push $(ORG)/c3po-$$img:$(TAG) || exit 1; \
	done;

.PHONY: $(RECURSIVETARGETS) $(WHAT) docker-build docker-push
.SILENT: docker-build docker-push
