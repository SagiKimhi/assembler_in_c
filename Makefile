# ---------------------------------------------
#  Makefile Variables:
# ---------------------------------------------

# Compiler
CC=gcc

# Flags Variables
CFLAGS=-ansi -pedantic -g -Wall
OFLAGS=-c

# Final executable/binary
BIN=asmake

# Directory Variables
PROJDIR ::= $(realpath $(CURDIR))
HEADERDIR ::= $(PROJDIR)/hdr
SOURCEDIR ::= $(PROJDIR)/src
OBJECTDIR ::= $(PROJDIR)/obj

# Create a list of subdirectories
SUBDIRS = data_structures
HEADERDIRS = $(HEADERDIR) $(foreach dir, $(SUBDIRS), $(addprefix $(HEADERDIR)/, $(dir)))
SOURCEDIRS = $(SOURCEDIR) $(foreach dir, $(SUBDIRS), $(addprefix $(SOURCEDIR)/, $(dir)))
OBJECTDIRS = $(OBJECTDIR) $(foreach dir, $(SUBDIRS), $(addprefix $(OBJECTDIR)/, $(dir)))

# Search Path Variables
VPATH = $(SOURCEDIRS)
INCLUDES = $(foreach dir, $(HEADERDIRS), $(addprefix -I, $(dir)))

# Files Variables
SOURCES = $(foreach dir,$(SOURCEDIRS),$(wildcard $(dir)/*.c))
OBJECTS ::= $(subst $(SOURCEDIR),$(OBJECTDIR),$(SOURCES:.c=.o))

# Useful Variables
SEP=/
RM = rm -rf
MKDIR = mkdir -p

# ---------------------------------------------

# Remove spaces after seperator
PSEP = $(strip $(SEP))

# Defines a function that will generate each rule
define generateRules
$(1)/%.o: %.c
	@echo Building $$@
	$$(CC) $$(CFLAGS) $$(OFLAGS) $$(INCLUDES) -o $$(subst /,$$(PSEP),$$@) $$(subst /,$$(PSEP),$$<) -MMD
endef


# ---------------------------------------------
#  Compilation options:
# ---------------------------------------------

.PHONY: all clean directories

# Default binary compilation method
all: directories withTree

# The program's data structure of
# choice would be a Linked List structure
withList: CFLAGS+=-D__USE_LIST__
withList: $(BIN)

# The program's data structure of
# choice would be a Hash Table structure
withTable: CFLAGS+=-D__USE_TABLE__
withTable: $(BIN)

# The program's data structure of
# choice would be a Binary Tree structure
withTree: CFLAGS+=-D__USE_TREE__
withTree: $(BIN)

# ---------------------------------------------


# ---------------------------------------------
#  Actual Compilation:
# ---------------------------------------------

# Creation of binary file
$(BIN): $(OBJECTS)
	echo Linking $@
	$(CC) $(CFLAGS) $(OBJECTS) -o $(BIN)

# Creation of object files (Generate rules)
$(foreach dir, $(OBJECTDIRS), $(eval $(call generateRules, $(dir))))

# Creates a directory for storing object files
directories:
	$(MKDIR) $(subst /,$(PSEP),$(OBJECTDIRS))
# ---------------------------------------------


# ---------------------------------------------
#  Cleanup:
# ---------------------------------------------

# Removes object files and executable/binary file
clean:
ifdef OBJECTDIRS
	$(RM) $(subst /, $(PSEP), $(OBJECTDIRS))
endif
ifdef BIN
	$(RM) $(BIN)
endif
# ---------------------------------------------
