SHELL := /bin/bash
all:
	$(MAKE) -C util
	$(MAKE) -C cdf
	$(MAKE) -C ctf
	$(MAKE) -C hsssec
	$(MAKE) -C hss
	$(MAKE) -C hssgtw
	$(MAKE) -C pcrf

clean:
	$(MAKE) -C util clean
	$(MAKE) -C cdf clean
	$(MAKE) -C ctf clean
	$(MAKE) -C hsssec clean
	$(MAKE) -C hss clean
	$(MAKE) -C hssgtw clean
	$(MAKE) -C pcrf clean

#install:
#	$(MAKE) -C c3poutil install
