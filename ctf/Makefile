#SGX_CDR = 1 Build CTF with SGX
#SGX_CDR = 0 Build CTF without SGX

SGX_CDR ?= 0
SGX_CDR_LIVE ?= 0

ifeq ($(SGX_CDR), 1)
   SGX_CDR_CFLAGS := -DSGX_CDR
endif

ifeq ($(SGX_CDR_LIVE), 1)
   SGX_CDR_CFLAGS := -DSGX_CDR -DSGX_CDR_LIVE
endif


CC := g++ # This is the main compiler
SRCDIR := src
BINDIR := bin
BUILDDIR := build
TARGETDIR := bin
TARGET := $(TARGETDIR)/ctf

SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
DEPENDS := $(OBJECTS:%.o=%.d)
CFLAGS := -g -pthread -std=c++0x $(SGX_CDR_CFLAGS)# -Wall
LFLAGS := -g -pthread -lpthread
LIBS := \
 ../util/lib/libc3po.a \
 /usr/local/lib/libpistache.a \
 /usr/local/lib/libcares.so \
 /usr/local/lib/libfdcore.so \
 /usr/local/lib/libfdproto.so \
 -lmemcached \
 -lrt \
 -lssl \
 -lcrypto
INCS := \
 -I./include \
 -I../util/include \
 -I../modules/rapidjson/include \
 -I /usr/local/include/freeDiameter \
 -I ../modules/spdlog/include \
 -I../modules/linux-sgx/common/inc

$(TARGET): $(OBJECTS)
	@echo " Linking..."
	@mkdir -p $(BINDIR)
	@echo " $(CC) $(LFLAGS) $^ -o $(TARGET) $(LIBS)"; $(CC) $(LFLAGS) $^ -o $(TARGET) $(LIBS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INCS) -MMD -c -o $@ $<"; $(CC) $(CFLAGS) $(INCS) -MMD -c -o $@ $<

clean:
	@echo " Cleaning...";
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)

-include $(DEPENDS)

.PHONY: clean
