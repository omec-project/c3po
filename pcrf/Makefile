CC := g++ # This is the main compiler
SRCDIR := src
BINDIR := bin
BUILDDIR := build
TARGETDIR := bin
TARGET := $(TARGETDIR)/pcrf
 
SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
DEPENDS := $(OBJECTS:%.o=%.d)
CFLAGS := -g -pthread -std=c++11 # -Wall
LFLAGS := -g -pthread -lpthread -Wl,-rpath,/usr/local/lib/x86_64-linux-gnu:/usr/local/lib

LIBS := \
 ../util/lib/libc3po.a \
 /usr/local/lib/libcares.so \
 /usr/local/lib/libfdcore.so \
 /usr/local/lib/libfdproto.so \
 -L/usr/local/lib/x86_64-linux-gnu \
 -lcassandra \
 -lmemcached \
 -lrt \
 -lpistache
 
INCS := \
 -I ./include \
 -I ../util/include \
 -I ../modules/rapidjson/include \
 -I ../modules/spdlog/include \
 -I /usr/local/include/freeDiameter

all: $(TARGET)
	$(MAKE) -C pcef
	$(MAKE) -C tdf
	$(MAKE) -C tssf
	
$(TARGET): $(OBJECTS)
	@echo " Linking..."
	@mkdir -p $(BINDIR)
	@echo " $(CC) $(LFLAGS) $^ -o $(TARGET) $(LIBS)"; $(CC) $(LFLAGS) $^ -o $(TARGET) $(LIBS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INCS) -MMD -c -o $@ $<"; $(CC) $(CFLAGS) $(INCS) -MMD -c -o $@ $<

clean:
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)
	$(MAKE) -C pcef clean
	$(MAKE) -C tdf clean
	$(MAKE) -C tssf clean

-include $(DEPENDS)

.PHONY: clean
