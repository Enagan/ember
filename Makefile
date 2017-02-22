#### PROJECT SETTINGS ####
# Where this makefile is running from
BUILD_ROOT=$(shell pwd)
# The name of the executable to be created
LIB_NAME := libember
# Compiler used
CXX ?= g++
# Ar settings to build the library
AR ?= ar
ARFLAGS = rcv
# Set RANLIB to ranlib on systems that require it (Sun OS < 4, Mac OSX)
# RANLIB  = ranlib
RANLIB = true
# Extension of source files used in the project
SRC_EXT = cpp
# Extension of secondary source files used in the project
SRC_EXT_2 = cc
# Path to the source directory, relative to the makefile
SRC_PATH = src
# Path to the include directory, relative to the makefile
INCL_PATH = include
# General compiler flags
COMPILE_FLAGS = -std=c++14 -Wall -Wextra -Werror -g
# Additional release-specific flags
RCOMPILE_FLAGS = -D NDEBUG
# Additional debug-specific flags
DCOMPILE_FLAGS = -D DEBUG
# Add additional include paths
INCLUDES = -I $(INCL_PATH)
# Destination directory, like a jail or mounted system
DESTDIR = /usr/local
#### END PROJECT SETTINGS ####

# Generally should not need to edit below this line

# Obtains the OS type, either 'Darwin' (OS X) or 'Linux'
UNAME_S:=$(shell uname -s)

# Function used to check variables. Use on the command line:
# make print-VARNAME
# Useful for debugging and adding features
print-%: ; @echo $*=$($*)

# Shell used in this makefile
# bash is used for 'echo -en'
SHELL = /bin/bash
# Clear built-in rules
.SUFFIXES:
# Programs for installation
INSTALL = install
INSTALL_PROGRAM = $(INSTALL)
INSTALL_DATA = $(INSTALL) -m 644

# Append pkg-config specific libraries if need be
ifneq ($(LIBS),)
	COMPILE_FLAGS += $(shell pkg-config --cflags $(LIBS))
	LINK_FLAGS += $(shell pkg-config --libs $(LIBS))
endif

# Verbose option, to output compile and link commands
export V := false
export CMD_PREFIX := @
ifeq ($(V),true)
	CMD_PREFIX :=
endif

# Build and output paths
release: export BUILD_PATH := $(BUILD_ROOT)/build/release
release: export BIN_PATH := $(BUILD_ROOT)/bin/release
release: export ALL_INCLUDES := $(INCLUDES)
debug: export BUILD_PATH := $(BUILD_ROOT)/build/debug
debug: export BIN_PATH := $(BUILD_ROOT)/bin/debug
debug: export ALL_INCLUDES := $(INCLUDES)
install: export BIN_PATH := bin/release

# Combine compiler and linker flags
release: export PROJECT_SPECIFIC_CXXFLAGS := $(CXXFLAGS) $(COMPILE_FLAGS) $(RCOMPILE_FLAGS)
debug: export PROJECT_SPECIFIC_CXXFLAGS := $(CXXFLAGS) $(COMPILE_FLAGS) $(DCOMPILE_FLAGS)


# Find all source files in the source directory, sorted by most
# recently modified
ifeq ($(UNAME_S),Darwin)
	SOURCES = $(shell find $(SRC_PATH) -name '*.$(SRC_EXT)' | sort -k 1nr | cut -f2- & \
			  find $(SRC_PATH) -name '*.$(SRC_EXT_2)' | sort -k 1nr | cut -f2-)
else
	SOURCES = $(shell find $(SRC_PATH) -name '*.$(SRC_EXT)' -printf '%T@\t%p\n' \
						| sort -k 1nr | cut -f2- & \
						find $(SRC_PATH) -name '*.$(SRC_EXT_2)' -printf '%T@\t%p\n' \
											| sort -k 1nr | cut -f2-)
endif

# fallback in case the above fails
rwildcard = $(foreach d, $(wildcard $1*), $(call rwildcard,$d/,$2) \
						$(filter $(subst *,%,$2), $d))
ifeq ($(SOURCES),)
	SOURCES := $(call rwildcard, $(SRC_PATH), *.$(SRC_EXT)) & \
				$(call rwildcard, $(SRC_PATH), *.$(SRC_EXT_2))
endif

# Set the object file names, with the source directory stripped
# from the path, and the build path prepended in its place
OBJECTS_1 = $(SOURCES:$(SRC_PATH)/%.$(SRC_EXT)=$(BUILD_PATH)/%.o)
OBJECTS = $(OBJECTS_1:$(SRC_PATH)/%.$(SRC_EXT_2)=$(BUILD_PATH)/%.o)
# Set the dependency files that will be used to add header dependencies
DEPS = $(OBJECTS:.o=.d)

# Macros for timing compilation
ifeq ($(UNAME_S),Darwin)
	CUR_TIME = awk 'BEGIN{srand(); print srand()}'
	TIME_FILE = $(dir $@).$(notdir $@)_time
	START_TIME = $(CUR_TIME) > $(TIME_FILE)
	END_TIME = read st < $(TIME_FILE) ; \
		$(RM) $(TIME_FILE) ; \
		st=$$((`$(CUR_TIME)` - $$st)) ; \
		echo $$st
else
	TIME_FILE = $(dir $@).$(notdir $@)_time
	START_TIME = date '+%s' > $(TIME_FILE)
	END_TIME = read st < $(TIME_FILE) ; \
		$(RM) $(TIME_FILE) ; \
		st=$$((`date '+%s'` - $$st - 86400)) ; \
		echo `date -u -d @$$st '+%H:%M:%S'`
endif

# Standard, non-optimized release build
.PHONY: release
release: dirs
	@echo "Beginning release build"
	@$(MAKE) all --no-print-directory
	@cp -rf $(INCL_PATH)/. $(BIN_PATH)/include

# Debug build for gdb debugging
.PHONY: debug
debug: dirs
	@echo "Beginning debug build"
	@$(MAKE) all --no-print-directory
	@cp -rf $(INCL_PATH)/. $(BIN_PATH)/include

# Create the directories used in the build
.PHONY: dirs
dirs:
	@mkdir -p $(dir $(OBJECTS))
	@mkdir -p $(BIN_PATH)
	@mkdir -p $(BIN_PATH)/lib
	@mkdir -p $(BIN_PATH)/include

# Installs the lib and attached dependencies at a DESTDIR
.PHONY: install
install:
	@echo "Installing to $(DESTDIR)/lib & $(DESTDIR)/include"
	@cp -rf $(BIN_PATH)/. $(DESTDIR)

# Removes all build files
.PHONY: clean
clean:
	@$(RM) -rf build/
	@$(RM) -rf bin

# Main rule, checks the executable and symlinks to the output
all: $(BIN_PATH)/$(LIB_NAME)

# Link the executable
$(BIN_PATH)/$(LIB_NAME): $(OBJECTS)
	@echo "Linking: $@"
	@$(AR) $(ARFLAGS) $(BIN_PATH)/lib/$(LIB_NAME).a $(OBJECTS) 1>>/dev/null
	@$(RANLIB) $(BIN_PATH)/lib/$(LIB_NAME).a  1>>/dev/null

# Add dependency files, if they exist
-include $(DEPS)

# Source file rules
# After the first compilation they will be joined with the rules from the
# dependency files to provide header dependencies
$(BUILD_PATH)/%.o: $(SRC_PATH)/%.$(SRC_EXT)
	@echo "Compiling: $< -> $@"
	$(CMD_PREFIX)$(CXX) $(PROJECT_SPECIFIC_CXXFLAGS) $(ALL_INCLUDES) -MP -MMD -c $< -o $@

$(BUILD_PATH)/%.o: $(SRC_PATH)/%.$(SRC_EXT_2)
	@echo "Compiling: $< -> $@"
	$(CMD_PREFIX)$(CXX) $(PROJECT_SPECIFIC_CXXFLAGS) $(ALL_INCLUDES) -MP -MMD -c $< -o $@
