CC := gcc # This is the main compiler
SRCDIR := src
BUILDDIR := build
TARGETDIR := lib
LIBNAME := libhsssec.a
TARGET := $(TARGETDIR)/$(LIBNAME)
INSTDIRINC := /usr/local/include/hsssec
INSTDIRLIB := /usr/local/lib
INSTLIB := $(INSTDIRLIB)/$(LIBNAME)

SECURITY_FLAGS := -D_FORTIFY_SOURCE=2 -fasynchronous-unwind-tables -fexceptions
SECURITY_FLAGS += -fstack-protector-all -fstack-protector-strong -Wall
SECURITY_FLAGS += -Werror=format-security -Werror=implicit-function-declaration
 
SRCEXT := c
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
DEPENDS := $(OBJECTS:%.o=%.d)
CFLAGS := -std=c99 -Wreturn-type -g -pthread -lrt -DNODEBUG $(SECURITY_FLAGS)# -Wall
INC := -I include

$(TARGET): $(OBJECTS)
	@echo " Linking..."
	@mkdir -p $(TARGETDIR)
	@echo " ar rcs $(TARGET) $(OBJECTS)"; ar rcs $(TARGET) $(OBJECTS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT) Makefile
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -MMD -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -MMD -c -o $@ $<

clean:
	@echo " Cleaning..."; 
	@echo " $(RM) -r $(BUILDDIR) $(TARGETDIR)"; $(RM) -r $(BUILDDIR) $(TARGETDIR)

install: $(TARGET)
	@echo " Installing..."
	@echo "  creating/verifying $(INSTDIRINC)"
	@mkdir -p $(INSTDIRINC)
	@echo "  copying headers to $(INSTDIRINC)"
	@cp -a include/* $(INSTDIRINC)
	@echo "  creating/verifying $(INSTDIRLIB)"
	@mkdir -p $(INSTDIRLIB)
	@echo "  copying $(TARGET) to $(INSTLIB)"
	@cp -a $(TARGET) $(INSTLIB)
	@echo " Installation complete"
	
-include $(DEPENDS)

.PHONY: clean
