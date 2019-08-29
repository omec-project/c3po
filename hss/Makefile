CC := g++ # This is the main compiler
SRCDIR := src
BINDIR := bin
BUILDDIR := build
TARGETDIR := bin
TARGET := $(TARGETDIR)/hss
#PERFORMANCE_TIMING := -DPERFORMANCE_TIMING
PERFORMANCE_TIMING :=
#TRACK_EXECUTION := -DTRACK_EXECUTION
TRACK_EXECUTION :=
#MONITOR_PENDING_MESSAGE_LEVEL :=-DMONITOR_PENDING_MESSAGE_LEVEL
MONITOR_PENDING_MESSAGE_LEVEL :=

SECURITY_FLAGS := -D_FORTIFY_SOURCE=2 -fasynchronous-unwind-tables -fexceptions
SECURITY_FLAGS += -fstack-protector-all -fstack-protector-strong -Wall
SECURITY_FLAGS += -Werror=format-security -Werror=implicit-function-declaration
 
SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
DEPENDS := $(OBJECTS:%.o=%.d)
CFLAGS := -g -pthread -std=c++11 $(PERFORMANCE_TIMING) $(TRACK_EXECUTION) $(MONITOR_PENDING_MESSAGE_LEVEL) $(SECURITY_FLAGS)# -Wall
LFLAGS := -g -pthread -lpthread -Wl,-rpath,/usr/local/lib/x86_64-linux-gnu:/usr/local/lib
LIBS := \
 $(ROOT)/util/lib/libc3po.a \
 $(ROOT)/hsssec/lib/libhsssec.a \
 -l:libpistache.a \
 -lcassandra \
 -lfdcore \
 -lfdproto \
 -lgmp \
 -lnettle \
 -lrt

INCS := \
 -I ./include \
 -I $(ROOT)/util/include \
 -I $(ROOT)/hsssec/include \
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
