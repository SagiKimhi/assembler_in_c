# ---------------------------------------------
#  Makefile Variables:
# ---------------------------------------------

# Compiler
CC = gcc

# Flags Variables
CFLAGS = -ansi -pedantic -g -Wall
OFLAGS = -c

# Final executable/binary
BIN = assembler

# Directory Variables
HEADERDIR = hdr
SOURCEDIR = src
OBJECTDIR = obj

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

# Useful Commands
RM = rm
RMDIR = rm -r
MKDIR = mkdir -p
# Remove spaces after seperator
SEP=/
PSEP = $(strip $(SEP))

# ---------------------------------------------


# ---------------------------------------------
#  Compilation options:
# ---------------------------------------------

# Decide whether or not commands will be shown
VERBOSE = FALSE
ifeq ($(VERBOSE),TRUE)
	HIDE =
else
	HIDE = @
endif

# Defines a function that will generate each rule
define generateRules
$(1)/%.o: %.c
	@echo Building $$@
	$(HIDE)$(CC) $$(CFLAGS) $$(OFLAGS) $$(INCLUDES) $$(subst /,$$(PSEP),$$<) -o $$(subst /,$$(PSEP),$$@)
endef

.PHONY: all clean directories

# Default binary compilation method
all: directories $(BIN)

# ---------------------------------------------


# ---------------------------------------------
#  Actual Compilation:
# ---------------------------------------------

# Creation of binary file
$(BIN): $(OBJECTS)
	@echo Linking $@
	$(HIDE)$(CC) $(CFLAGS) $(OBJECTS) -o $(BIN)
	@echo Done!

# Creation of object files (Generate rules)
$(foreach dir, $(OBJECTDIRS), $(eval $(call generateRules, $(dir))))

# Creates a directory for storing object files
directories:
	$(HIDE)$(MKDIR) $(subst /,$(PSEP),$(OBJECTDIRS))
# ---------------------------------------------


# ---------------------------------------------
#  Cleanup:
# ---------------------------------------------

# Removes object files and executable/binary file
clean:
ifdef OBJECTDIR
	$(HIDE)$(RMDIR) $(OBJECTDIR)
endif
ifdef BIN
	$(HIDE)$(RM) $(BIN)
endif
	@echo Cleaning done!

# ---------------------------------------------
