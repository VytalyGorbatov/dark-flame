##########
# Target #
##########

TARGET = host

#############
# Compilers #
#############

CC        = g++
LNK       = g++
OBJ_EXT   = o

##################
# Extended goals #
##################

DIST_GOALS         = dist $(filter dist-%,$(MAKECMDGOALS))
UTEST_GOALS        = utest $(filter utest-%,$(MAKECMDGOALS))

###############
# Directories #
###############

TMP_DIR            = /tmp

SRC_DIR            = src/cpp
AI_SRC_DIR         = src/cpp/ai
ENTITY_SRC_DIR     = src/cpp/entity
FUNCTION_SRC_DIR   = src/cpp/function
MATH_SRC_DIR       = src/cpp/math
MODEL_SRC_DIR      = src/cpp/model
PHYSIC_SRC_DIR     = src/cpp/physic
RENDERER_SRC_DIR   = src/cpp/renderer
TEST_DIR           = test/cpp

BUILD_DIR          = $(TMP_DIR)/build
DIST_DIR           = $(TMP_DIR)/dist
TARGET_BUILD_DIR   = $(BUILD_DIR)/$(TARGET)
TARGET_DIST_DIR    = $(DIST_DIR)/$(TARGET)

UTEST_BUILD_DIR    = $(TARGET_BUILD_DIR)/utest

VDIRS              = $(BUILD_DIR)\
                     $(DIST_DIR)\
                     $(TARGET_BUILD_DIR)\
                     $(TARGET_DIST_DIR)\
                     $(UTEST_BUILD_DIR)\
                     $(TARGET_BUILD_DIR)/ai\
                     $(TARGET_BUILD_DIR)/entity\
                     $(TARGET_BUILD_DIR)/function\
                     $(TARGET_BUILD_DIR)/math\
                     $(TARGET_BUILD_DIR)/model\
                     $(TARGET_BUILD_DIR)/physic\
                     $(TARGET_BUILD_DIR)/renderer

CFLAGS += -I$(SRC_DIR)\
          -I$(AI_SRC_DIR)\
          -I$(ENTITY_SRC_DIR)\
          -I$(FUNCTION_SRC_DIR)\
          -I$(MATH_SRC_DIR)\
          -I$(MODEL_SRC_DIR)\
          -I$(PHYSIC_SRC_DIR)\
          -I$(RENDERER_SRC_DIR)

###########################
# Source and object files #
###########################

SRC_OBJS        = $(patsubst $(SRC_DIR)/%.cpp,$(TARGET_BUILD_DIR)/%.$(OBJ_EXT),$(wildcard $(SRC_DIR)/*.cpp))
AI_OBJS         = $(patsubst $(AI_SRC_DIR)/%.cpp,$(TARGET_BUILD_DIR)/%.$(OBJ_EXT),$(wildcard $(AI_SRC_DIR)/*.cpp))
ENTITY_OBJ      = $(patsubst $(ENTITY_SRC_DIR)/%.cpp,$(TARGET_BUILD_DIR)/%.$(OBJ_EXT),$(wildcard $(ENTITY_SRC_DIR)/*.cpp))
FUNCTION_OBJS   = $(patsubst $(FUNCTION_SRC_DIR)/%.cpp,$(TARGET_BUILD_DIR)/%.$(OBJ_EXT),$(wildcard $(FUNCTION_SRC_DIR)/*.cpp))
MATH_OBJS       = $(patsubst $(MATH_SRC_DIR)/%.cpp,$(TARGET_BUILD_DIR)/%.$(OBJ_EXT),$(wildcard $(MATH_SRC_DIR)/*.cpp))
MODEL_OBJS      = $(patsubst $(MODEL_SRC_DIR)/%.cpp,$(TARGET_BUILD_DIR)/%.$(OBJ_EXT),$(wildcard $(MODEL_SRC_DIR)/*.cpp))
PHYSIC_OBJS     = $(patsubst $(PHYSIC_SRC_DIR)/%.cpp,$(TARGET_BUILD_DIR)/%.$(OBJ_EXT),$(wildcard $(PHYSIC_SRC_DIR)/*.cpp))
RENDERER_OBJS   = $(patsubst $(RENDERER_SRC_DIR)/%.cpp,$(TARGET_BUILD_DIR)/%.$(OBJ_EXT),$(wildcard $(RENDERER_SRC_DIR)/*.cpp))

SRCS_OBJS       = $(SRC_OBJS)\
                  $(AI_OBJS)\
                  $(ENTITY_OBJS)\
                  $(FUNCTION_OBJS)\
                  $(MATH_OBJS)\
                  $(MODEL_OBJS)\
                  $(PHYSIC_OBJS)\
                  $(RENDERER_OBJS)

UTEST_CPP       = $(patsubst $(TEST_DIR)/%,%,$(wildcard $(TEST_DIR)/unit*.cpp))
UTEST_CPP      += utils.cpp
UTEST_OBJS      = $(addprefix $(UTEST_BUILD_DIR)/,$(patsubst %.cpp,%.$(OBJ_EXT),$(UTEST_CPP)))

###################
# Default targets #
###################

.PHONY: all clean rebuild

all: $(DIST_GOALS)

rebuild: clean all

clean:
	rm -rf $(BUILD_DIR) $(DIST_DIR)

$(VDIRS):
	mkdir $@

############################
# Compilation and building #
############################

$(SRC_OBJS): $(TARGET_BUILD_DIR)/%.$(OBJ_EXT): $(SRC_DIR)/%.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

$(AI_OBJS): $(TARGET_BUILD_DIR)/%.$(OBJ_EXT): $(AI_SRC_DIR)/%.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

$(ENTITY_OBJ): $(TARGET_BUILD_DIR)/%.$(OBJ_EXT): $(ENTITY_SRC_DIR)/%.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

$(FUNCTION_OBJS): $(TARGET_BUILD_DIR)/%.$(OBJ_EXT): $(FUNCTION_SRC_DIR)/%.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

$(MATH_OBJS): $(TARGET_BUILD_DIR)/%.$(OBJ_EXT): $(MATH_SRC_DIR)/%.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

$(MODEL_OBJS): $(TARGET_BUILD_DIR)/%.$(OBJ_EXT): $(MODEL_SRC_DIR)/%.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

$(PHYSIC_OBJS): $(TARGET_BUILD_DIR)/%.$(OBJ_EXT): $(PHYSIC_SRC_DIR)/%.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

$(RENDERER_OBJS): $(TARGET_BUILD_DIR)/%.$(OBJ_EXT): $(RENDERER_SRC_DIR)/%.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

$(UTEST_OBJS): $(UTEST_BUILD_DIR)/%.$(OBJ_EXT): $(TEST_DIR)/%.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

$(UTEST_BUILD_DIR)/unit: $(UTEST_OBJS) $(SRCS_OBJS)
	$(LNK) $^ $(CFLAGS) $(LDFLAGS) -o $@

################
# Unit testing #
################

.PHONY: $(UTEST_GOALS)

$(UTEST_GOALS): $(VDIRS) $(UTEST_BUILD_DIR)/unit
	$(UTEST_BUILD_DIR)/unit

######################
# Debug the Makefile #
######################

.PHONY: debug

debug:
	@echo $(UTEST_OBJS)
	@echo $(UTEST_BUILD_DIR)
	@echo $(TEST_DIR)
	@echo $(SRCS_OBJS)
