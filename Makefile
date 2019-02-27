RECURSIVETARGETS := all clean
SET1 := cdf ctf hsssec pcrf
SET2 := hss hssgtw

$(RECURSIVETARGETS): util $(SET1) $(SET2)
util:
	$(MAKE) -C $@ $(MAKECMDGOALS)
$(SET1): util
	$(MAKE) -C $@ $(MAKECMDGOALS)
$(SET2): util hsssec
	$(MAKE) -C $@ $(MAKECMDGOALS)

.PHONY: $(RECURSIVETARGETS) $(SET1) $(SET2) util
