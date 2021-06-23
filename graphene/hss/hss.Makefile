# Build HSS as follows:
#
# - make -f hss.Makefile               -- create non-SGX no-debug-log manifest
# - make SGX=1 -f hss.Makefile         -- create SGX no-debug-log manifest
# - make SGX=1 DEBUG=1 -f hss.Makefile -- create SGX debug-log manifest
# - make clean -f hss.Makefile         -- remove Graphene-generated files

################################# CONSTANTS ###################################

# Relative path to Graphene root
GRAPHENEDIR = /root/graphene/latest/graphene/
SGX_SIGNER_KEY ?= $(GRAPHENEDIR)/Pal/src/host/Linux-SGX/signer/enclave-key.pem
VERIFIER_PROVISIONED_CERT_DIR ?= /provisioned_certs/

HSSDIR = ../../hss

ifeq ($(DEBUG),1)
GRAPHENE_LOG_LEVEL = debug
else
GRAPHENE_LOG_LEVEL = error
endif

.PHONY: all
all: hss.manifest pal_loader hss conf logs
ifeq ($(SGX),1)
all: hss.manifest.sgx hss.sig hss.token
#all: libcert_prov_attest.so hss.manifest.sgx hss.sig hss.token
endif

include $(GRAPHENEDIR)/Scripts/Makefile.configs


################################ HSS MANIFEST ###############################

# The template file contains almost all necessary information to run HSS
# under Graphene / Graphene-SGX. We create hss.manifest (to be run under
# non-SGX Graphene) by simply replacing variables in the template file via sed.

hss.manifest: hss.manifest.template
	sed -e 's|$$(GRAPHENEDIR)|'"$(GRAPHENEDIR)"'|g' \
		-e 's|$$(GRAPHENE_LOG_LEVEL)|'"$(GRAPHENE_LOG_LEVEL)"'|g' \
		-e 's|$$(ARCH_LIBDIR)|'"$(ARCH_LIBDIR)"'|g' \
		-e 's|$$(VERIFIER_PROVISIONED_CERT_DIR)|'"$(VERIFIER_PROVISIONED_CERT_DIR)"'|g' \
		$< > $@

# Manifest for Graphene-SGX requires special "pal-sgx-sign" procedure. This procedure measures all
# HSS trusted files, adds the measurement to the resulting manifest.sgx file (among other,
# less important SGX options) and creates hss.sig (SIGSTRUCT object).
#
# Graphene-SGX requires EINITTOKEN and SIGSTRUCT objects (see SGX hardware ABI, in particular EINIT
# instruction). The "pal-sgx-get-token" script generates EINITTOKEN based on a SIGSTRUCT and puts it
# in .token file. Note that filenames must be the same as the manifest name (i.e., "hss").
# EINITTOKEN must be generated on the machine where the application will run, not where it was
# built.

hss.sig hss.manifest.sgx &: hss.manifest $(HSSDIR)/bin/hss
	$(GRAPHENEDIR)/Pal/src/host/Linux-SGX/signer/pal-sgx-sign \
		-libpal $(GRAPHENEDIR)/Runtime/libpal-Linux-SGX.so \
		-key $(SGX_SIGNER_KEY) \
		-manifest hss.manifest \
		-output hss.manifest.sgx

hss.token: hss.sig
	$(GRAPHENEDIR)/Pal/src/host/Linux-SGX/signer/pal-sgx-get-token -output $@ -sig $<

########################### COPIES OF EXECUTABLES #############################

# Create a link to pal_loader and hss app for simplicity.

hss:
	ln -s $(HSSDIR)/bin/hss $@

conf:
	ln -s $(HSSDIR)/conf $@

logs:
	ln -s $(HSSDIR)/logs $@

pal_loader:
	ln -s $(GRAPHENEDIR)/Runtime/pal_loader $@

libcert_prov_attest.so:
	cp $(GRAPHENEDIR)/Pal/src/host/Linux-SGX/tools/ra-tls/$@ .

############################## RUNNING TESTS ##################################
.PHONY: start-hss
start-hss: all
	$(HSSDIR)/bin/hss -j $(HSSDIR)/conf/hss.json

.PHONY: start-graphene-hss
start-graphene-hss: all
	./pal_loader $(HSSDIR)/bin/hss -j $(HSSDIR)/conf/hss.json

################################## CLEANUP ####################################

.PHONY: clean
clean:
	$(RM) *.token *.sig *.manifest.sgx *.manifest pal_loader *.so
