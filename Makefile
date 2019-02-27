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

.PHONY: $(RECURSIVETARGETS) $(WHAT)
