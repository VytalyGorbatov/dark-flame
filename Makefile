# DarkFlame Copyright (C) 2014 Alexey Shumeiko
#
# This file is part of DarkFlame.
#
# DarkFlame is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# DarkFlame is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with DarkFlame.  If not, see <http://www.gnu.org/licenses/>.
#
# For more details see LICENSE file.

################################################################
# During development you can redefine some behaviour
# in Makefile.config.

# Available for customization:
#   bprefix - build dir prefix
#   prefix  - install dir prefix

ifneq '' '$(wildcard Makefile.config)'
  include Makefile.config
endif

################################################################
# Auxiliary macros.

error_unknown_value = $(error Unknown $(1): '$($(1))')

################################################################
# Determine platform/environment.

host_os := linux
ifneq '' '$(COMSPEC)'
  ifneq '' '$(WINDIR)'
    # Probably under Windows.
    host_os := windows
  endif
endif

ifeq 'windows' '$(host_os)'
  TARGET := windows
  toolchain := msvc
else ifeq 'linux' '$(host_os)'
  TARGET := linux
  toolchain := gcc
else
  $(call error_unknown_value,host_os)
endif

################################################################
# Build tools definition.

ifeq 'msvc' '$(toolchain)'
  CC := cl
else ifeq 'gcc' '$(toolchain)'
  CC := g++
else
  $(call error_unknown_value,toolchain)
endif

# Usage: $(call COMPILE,flags,input,output)
ifeq 'msvc' '$(toolchain)'
  COMPILE = $(CC) /Fo$(3) $(1) $(2)
else ifeq 'gcc' '$(toolchain)'
  COMPILE = $(CC) -o $(3) $(1) $(2)
endif

# Usage: $(call LINK,flags,input,output)
ifeq 'msvc' '$(toolchain)'
  LINK = $(CC) /Fe$3 $2 $1
else ifeq 'gcc' '$(toolchain)'
  LINK = $(CC) -o $3 $2 $1
endif

# File extensions.
ifeq 'msvc' '$(toolchain)'
  OBJ_EXT    = obj
  OBJ_SUFFIX = .obj
else ifeq 'gcc' '$(toolchain)'
  OBJ_EXT    = o
  OBJ_SUFFIX = .o
endif

ifeq 'msvc' '$(toolchain)'
  MSVC_CONF_FILE := msvc.config
  # Check for MSVC environment config file.
  ifeq '' '$(wildcard $(MSVC_CONF_FILE))'
    $(error MSVC environment not set. May be you forget to create $(MSVC_CONF_FILE) file)
  endif
  include $(MSVC_CONF_FILE)
  # Build in Windows under Cygwin.
  ifeq (cl,$(CC))
    PATH_ := $(shell cygpath -u --path '$(PATH_)')
    export PATH := $(PATH_):$(PATH)
  endif
endif

################################################################
# Warning flags.

# /W0 - only errors
# /W1, /W2, /W3 - extra warrings
# /W4 (/Wall) - and remarks
ifeq 'msvc' '$(toolchain)'
  WARNING_CFLAGS = /W3 /nologo
else ifeq 'gcc' '$(toolchain)'
  WARNING_CFLAGS = -Wall -pedantic -Wno-unknown-pragmas
endif

CFLAGS += $(WARNING_CFLAGS)

################################################################
# Optimisation flags.

ifeq 'msvc' '$(toolchain)'
  # /GS[-] enable security checks, /Gy[-] separate functions for linker,
  # /O2 maximize speed, /Ob<n> inline expansion, /Ox max optimisation,
  # /Oy[-] enable frame pointer omission
  OPTIM_CFLAGS = /O2 /Ob2 /Ox
else ifeq 'gcc' '$(toolchain)'
  # -O3 max safe optimisation
  OPTIM_CFLAGS = -O3 -finline -fomit-frame-pointer
endif

CFLAGS += $(OPTIM_CFLAGS)

################################################################
# Other FLAGS.

ifeq 'msvc' '$(toolchain)'
    CFLAGS += /Oi
    CFLAGS += /Zl
    CFLAGS += /D_CRT_SECURE_NO_WARNINGS
    CFLAGS += /GL
    CFLAGS += /D "WIN32"
    CFLAGS += /D "NDEBUG"
    CFLAGS += /D "_UNICODE" /D "UNICODE"
    CFLAGS += /D "_CONSOLE"
    CFLAGS += /Gm-
    CFLAGS += /EHsc
    CFLAGS += /fp:precise
    CFLAGS += /Zc:wchar_t
    CFLAGS += /Zc:forScope
    CFLAGS += /Gd
    CFLAGS += /analyze-
endif

################################################################
# Include libs.

ifeq 'msvc' '$(toolchain)'
    LDFLAGS += libcmt.lib
    LDFLAGS += advapi32.lib
    LDFLAGS += user32.lib
    LDFLAGS += opengl32.lib
    LDFLAGS += glu32.lib
    LDFLAGS += gdi32.lib
else ifeq 'gcc' '$(toolchain)'
  LDFLAGS += -lglut -lGL -lGLU
  LDFLAGS += -lX11
  LDFLAGS += -lpthread
endif

################################################################
# Extended goals.

$(filter clean-%,$(MAKECMDGOALS)):   clean
$(filter build-%,$(MAKECMDGOALS)):   build
$(filter rebuild-%,$(MAKECMDGOALS)): rebuild
$(filter dist-%,$(MAKECMDGOALS)):    dist
$(filter utest-%,$(MAKECMDGOALS)):   utest
$(filter atest-%,$(MAKECMDGOALS)):   atest

################################################################
# Directories.

TMP_DIR            = /tmp

SRC_DIR            = src/cpp
AI_SRC_DIR         = src/cpp/ai
ENTITY_SRC_DIR     = src/cpp/entity
FUNCTION_SRC_DIR   = src/cpp/function
MATH_SRC_DIR       = src/cpp/math
MODEL_SRC_DIR      = src/cpp/model
PHYSIC_SRC_DIR     = src/cpp/physic
RENDERER_SRC_DIR   = src/cpp/renderer
WINDOW_SRC_DIR     = src/cpp/window
TEST_DIR           = test/cpp

BUILD_DIR          ?= $(bprefix)build
DIST_DIR           ?= $(bprefix)dist

TARGET_BUILD_DIR   = $(BUILD_DIR)/$(TARGET)
TARGET_DIST_DIR    = $(DIST_DIR)/$(TARGET)

UTEST_BUILD_DIR    = $(TARGET_BUILD_DIR)/utest
ATEST_BUILD_DIR    = $(TARGET_BUILD_DIR)/atest

VDIRS              = $(BUILD_DIR)\
                     $(DIST_DIR)\
                     $(TARGET_BUILD_DIR)\
                     $(TARGET_DIST_DIR)\
                     $(UTEST_BUILD_DIR)\
                     $(ATEST_BUILD_DIR)\
                     $(TARGET_BUILD_DIR)/ai\
                     $(TARGET_BUILD_DIR)/entity\
                     $(TARGET_BUILD_DIR)/function\
                     $(TARGET_BUILD_DIR)/math\
                     $(TARGET_BUILD_DIR)/model\
                     $(TARGET_BUILD_DIR)/physic\
                     $(TARGET_BUILD_DIR)/renderer\
                     $(TARGET_BUILD_DIR)/window

CFLAGS += -I$(SRC_DIR)\
          -I$(AI_SRC_DIR)\
          -I$(ENTITY_SRC_DIR)\
          -I$(FUNCTION_SRC_DIR)\
          -I$(MATH_SRC_DIR)\
          -I$(MODEL_SRC_DIR)\
          -I$(PHYSIC_SRC_DIR)\
          -I$(RENDERER_SRC_DIR)\
          -I$(WINDOW_SRC_DIR)

################################################################
# Source and object files.

SRC_OBJS        = $(patsubst $(SRC_DIR)/%.cpp,$(TARGET_BUILD_DIR)/%.$(OBJ_EXT),$(wildcard $(SRC_DIR)/*.cpp))
AI_OBJS         = $(patsubst $(SRC_DIR)/%.cpp,$(TARGET_BUILD_DIR)/%.$(OBJ_EXT),$(wildcard $(AI_SRC_DIR)/*.cpp))
ENTITY_OBJS     = $(patsubst $(SRC_DIR)/%.cpp,$(TARGET_BUILD_DIR)/%.$(OBJ_EXT),$(wildcard $(ENTITY_SRC_DIR)/*.cpp))
FUNCTION_OBJS   = $(patsubst $(SRC_DIR)/%.cpp,$(TARGET_BUILD_DIR)/%.$(OBJ_EXT),$(wildcard $(FUNCTION_SRC_DIR)/*.cpp))
MATH_OBJS       = $(patsubst $(SRC_DIR)/%.cpp,$(TARGET_BUILD_DIR)/%.$(OBJ_EXT),$(wildcard $(MATH_SRC_DIR)/*.cpp))
MODEL_OBJS      = $(patsubst $(SRC_DIR)/%.cpp,$(TARGET_BUILD_DIR)/%.$(OBJ_EXT),$(wildcard $(MODEL_SRC_DIR)/*.cpp))
PHYSIC_OBJS     = $(patsubst $(SRC_DIR)/%.cpp,$(TARGET_BUILD_DIR)/%.$(OBJ_EXT),$(wildcard $(PHYSIC_SRC_DIR)/*.cpp))
RENDERER_OBJS   = $(patsubst $(SRC_DIR)/%.cpp,$(TARGET_BUILD_DIR)/%.$(OBJ_EXT),$(wildcard $(RENDERER_SRC_DIR)/*.cpp))
WINDOW_OBJS     = $(patsubst $(SRC_DIR)/%.cpp,$(TARGET_BUILD_DIR)/%.$(OBJ_EXT),$(wildcard $(WINDOW_SRC_DIR)/*.cpp))

SRCS_OBJS       = $(SRC_OBJS)\
                  $(AI_OBJS)\
                  $(ENTITY_OBJS)\
                  $(FUNCTION_OBJS)\
                  $(MATH_OBJS)\
                  $(MODEL_OBJS)\
                  $(PHYSIC_OBJS)\
                  $(RENDERER_OBJS)\
                  $(WINDOW_OBJS)

UTEST_CPP       = $(patsubst $(TEST_DIR)/%,%,$(wildcard $(TEST_DIR)/unit*.cpp))
UTEST_CPP      += utils.cpp
UTEST_OBJS      = $(addprefix $(UTEST_BUILD_DIR)/,$(patsubst %.cpp,%.$(OBJ_EXT),$(UTEST_CPP)))

ATEST_CPP       = $(patsubst $(TEST_DIR)/%,%,$(wildcard $(TEST_DIR)/accept*.cpp))
ATEST_CPP      += utils.cpp
ATEST_OBJS      = $(addprefix $(ATEST_BUILD_DIR)/,$(patsubst %.cpp,%.$(OBJ_EXT),$(ATEST_CPP)))

################################################################
# Default targets.

.PHONY: all clean rebuild dist

all: dist

rebuild: clean all

clean:
	rm -rf $(BUILD_DIR) $(DIST_DIR)

$(VDIRS):
	mkdir -p $@

dist: $(VDIRS) $(SRCS_OBJS)

################################################################
# Compilation and building.

$(SRCS_OBJS): $(TARGET_BUILD_DIR)/%.$(OBJ_EXT): $(SRC_DIR)/%.cpp
	$(call COMPILE,$(CFLAGS) -c,$<,$@)

$(UTEST_OBJS): $(UTEST_BUILD_DIR)/%.$(OBJ_EXT): $(TEST_DIR)/%.cpp
	$(call COMPILE,$(CFLAGS) -c,$<,$@)

$(UTEST_BUILD_DIR)/unit: $(UTEST_OBJS) $(SRCS_OBJS)
	$(call LINK,$(CFLAGS) $(LDFLAGS),$^,$@)

$(ATEST_OBJS): $(ATEST_BUILD_DIR)/%.$(OBJ_EXT): $(TEST_DIR)/%.cpp
	$(call COMPILE,$(CFLAGS) -c,$<,$@)

$(ATEST_BUILD_DIR)/accept: $(ATEST_OBJS) $(SRCS_OBJS)
	$(call LINK,$(CFLAGS) $(LDFLAGS),$^,$@)

################################################################
# Unit testing.

.PHONY: utest

utest: $(VDIRS) $(UTEST_BUILD_DIR)/unit
	$(UTEST_BUILD_DIR)/unit

################################################################
# Accept testing.

.PHONY: atest

atest: $(VDIRS) $(ATEST_BUILD_DIR)/accept
	$(ATEST_BUILD_DIR)/accept

################################################################
# Debug the Makefile.

.PHONY: debug

debug:
	@echo test
