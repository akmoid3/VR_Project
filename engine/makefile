BASE_NAME := engine
TARGET_DIR = .
TARGET := lib$(BASE_NAME).so
MAIN_SRC_FILES := $(wildcard *.cpp)
MAIN_OBJ_FILES := $(patsubst %.cpp,%.o,$(MAIN_SRC_FILES))
TESTS_RUNNER := $(BASE_NAME)-tests-runner
TEST_SRC_FILES := $(wildcard *_test.cpp)
TEST_OBJ_FILES := $(patsubst %.cpp,%.o,$(TEST_SRC_FILES))
DEPENDENCIES_DIR := dependencies


CXX = g++
CXX_FLAGS = -c -fPIC -std=c++11 -O2
LD_FLAGS = -shared
LIBS = -lglut -lGL -lGLU -lfreeimage


install: $(TARGET)
	@echo "all done!"


$(TARGET): $(MAIN_OBJ_FILES) $(TESTS_RUNNER)
	# build your .so library here
	$(CXX) $(LD_FLAGS) -o $(TARGET) $(MAIN_OBJ_FILES) $(LIBS)
	@echo "$(TARGET) build done!"


$(TESTS_RUNNER): $(TEST_OBJ_FILES)
	# build your test runner executable here
	$(CXX) -o $(TESTS_RUNNER) $(MAIN_OBJ_FILES) $(LIBS)
	@echo "$(TESTS_RUNNER) build done!"
	# execute your tests here (on test failure, exit with exit code = 1)
	./$(TESTS_RUNNER)
	@echo "tests execution done!"


%.o: %.cpp
	# compile your sources here
	$(CXX) $(CXX_FLAGS) -c -o $@ $^

clean: 
	@rm -f $(TESTS_RUNNER)
	@rm -f $(TEST_OBJ_FILES)
	@rm -f $(TARGET)
	@rm -f $(MAIN_OBJ_FILES)


.PHONY: clean install
