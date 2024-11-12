TARGET_EXEC ?= Chess

BUILD_DIR ?= ./build
SRC_DIRS ?= src/
CCX = g++ 

SRCS := $(shell find $(SRC_DIRS) -name *.cpp)
#OBJS := $(SRCS:%=%.o)
#DEPS := $(OBJS:.o=.d)

DEPS = $(shell find build/ -name *.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS ?= $(INC_FLAGS) -std=c++17 -MMD -MD -MP -Wall -Wextra #-Wfloat-equal -Wundef -Wunreachable-code -Wcast-qual
LDFLAGS_INTERN ?= $(LDFLAGS) -lsfml-graphics -lsfml-window -lsfml-system -lGL

#
# Debug build settings
#
DBGDIR := $(BUILD_DIR)/debug
DBGEXE := $(DBGDIR)/$(TARGET_EXEC)
DBGOBJS := $(SRCS:%=$(DBGDIR)/%.o)
DBGCFLAGS := $(CPPFLAGS) -g -O0 -ftrapv -DDEBUG

#
# Release build settings
#
RELDIR := $(BUILD_DIR)/release
RELEXE := $(RELDIR)/$(TARGET_EXEC)
RELOBJS := $(SRCS:%=$(RELDIR)/%.o)
RELCFLAGS := $(CPPFLAGS) -O3 -Ofast -DRELEASE -DNDEBUG

.PHONY: clean release debug run rund all remake

all: release debug
remake: clean all

release: $(RELEXE)

$(RELEXE): $(RELOBJS)
	$(MKDIR_P) $(dir $@)
	$(CCX) $(RELOBJS) -o $@ $(LDFLAGS_INTERN)

$(RELDIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(RELCFLAGS) -c $< -o $@


debug: $(DBGEXE)

$(DBGEXE): $(DBGOBJS)
	$(MKDIR_P) $(dir $@)
	$(CCX) $(DBGOBJS) -o $@ $(LDFLAGS_INTERN)

$(DBGDIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(DBGCFLAGS) -c $< -o $@


clean:
	$(RM) -r $(BUILD_DIR)

run:
	$(RELEXE)

rund:
	$(DBGEXE)

test:
	valgrind --undef-value-errors=no -v $(DBGEXE)

-include $(DEPS)

MKDIR_P ?= mkdir -p
