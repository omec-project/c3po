CC := g++ # This is the main compiler
SRCDIR := src
BINDIR := bin
BUILDDIR := build
TARGETDIR := bin
TARGET := $(TARGETDIR)/smsrouter
 
SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
DEPENDS := $(OBJECTS:%.o=%.d)
CFLAGS := -g -pthread -std=c++11 # -Wall
LFLAGS := -g -pthread -lpthread -Wl,-rpath,/usr/local/lib/x86_64-linux-gnu:/usr/local/lib
LIBS := \
 $(ROOT)/util/lib/libc3po.a \
 -lcares \
 -lcassandra \
 -lfdcore \
 -lfdproto \
 -lmemcached \
 -lrt

INCS := \
 -I ./include \
 -I $(ROOT)/util/include \
 -I $(ROOT)/modules/spdlog/include

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
