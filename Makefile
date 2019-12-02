export ROOT := $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))

VERSION ?= $(shell cat ./VERSION)

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

DOCKER_TAG               ?= ${VERSION}
DOCKER_REGISTRY          ?=
DOCKER_REPOSITORY        ?=
DOCKER_BUILD_ARGS        ?=

## Docker labels. Only set ref and commit date if committed
DOCKER_LABEL_VCS_URL     ?= $(shell git remote get-url $(shell git remote))
DOCKER_LABEL_VCS_REF     ?= $(shell git diff-index --quiet HEAD -- && git rev-parse HEAD || echo "unknown")
DOCKER_LABEL_COMMIT_DATE ?= $(shell git diff-index --quiet HEAD -- && git show -s --format=%cd --date=iso-strict HEAD || echo "unknown" )
DOCKER_LABEL_BUILD_DATE  ?= $(shell date -u "+%Y-%m-%dT%H:%M:%SZ")
# Complete set of targets are "build $(SET1) $(SET2) pcef tdf tssf hssdb"
DOCKER_TARGETS           ?= hss hssdb

# https://docs.docker.com/engine/reference/commandline/build/#specifying-target-build-stage---target
docker-build:
	for target in $(DOCKER_TARGETS); do \
		docker build $(DOCKER_BUILD_ARGS) \
			--target $$target \
			--tag ${DOCKER_REGISTRY}${DOCKER_REPOSITORY}c3po-$$target:${DOCKER_TAG} \
			--label "org.label-schema.schema-version=1.0" \
			--label "org.label-schema.name=$$target" \
			--label "org.label-schema.version=${VERSION}" \
			--label "org.label-schema.vcs-url=${DOCKER_LABEL_VCS_URL}" \
			--label "org.label-schema.vcs-ref=${DOCKER_LABEL_VCS_REF}" \
			--label "org.label-schema.build-date=${DOCKER_LABEL_BUILD_DATE}" \
			--label "org.opencord.vcs-commit-date=${DOCKER_LABEL_COMMIT_DATE}" \
			.; \
	done

# Note that only hss and hssdb repositories are available on Docker Hub now.
docker-push:
	for target in $(DOCKER_TARGETS); do \
		docker push ${DOCKER_REGISTRY}${DOCKER_REPOSITORY}c3po-$$target:${DOCKER_TAG}; \
	done

.PHONY: $(RECURSIVETARGETS) $(WHAT) docker-build docker-push
.SILENT: docker-build docker-push
