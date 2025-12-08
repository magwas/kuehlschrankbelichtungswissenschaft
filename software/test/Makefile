MODULES = MessageQueue
MessageQueue_TESTS = add get register unregister dispatch

TESTDATA_OBJS =

CXX = g++
CXXFLAGS = -std=c++17 -Wall -I h  -c
LXXFLAGS = -std=c++17 -I h -pthread
TARGET = tests
GTEST = -lgtest

# Generate object files for each module
MODULE_OBJS = $(addprefix ./obj/,$(addsuffix .o,$(MODULES)))

# Generate test object files for each module and test
# For each module M and test T: generate TestM_T.o
TEST_OBJS = $(foreach MODULE,$(MODULES),\
    $(foreach TEST,$(filter-out \#%,$($(MODULE)_TESTS)),\
        ./obj/Test$(MODULE)_$(TEST).o))

# Combine all objects (keeping tests.o separate)
OBJECTS = $(MODULE_OBJS) ./obj/tests.o $(TEST_OBJS) $(TESTDATA_OBJS)

all: $(TARGET)
	./$(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(LXXFLAGS) -o $(TARGET) $(OBJECTS) $(GTEST)

VPATH = . ..

# Pattern rule for .o files in ./obj directory
./obj/%.o: %.cc
	mkdir -p ./obj
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -frv $(TARGET) obj
