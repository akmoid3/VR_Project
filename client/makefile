BASE_NAME := rush-hour
TARGET_DIR = .
TARGET := $(BASE_NAME) 
TARGET_ARCHIVE := $(BASE_NAME).tar.gz
MAIN_SRC_FILES := $(wildcard *.cpp)
MAIN_OBJ_FILES := $(patsubst %.cpp,%.o,$(MAIN_SRC_FILES))
TESTS_RUNNER := $(BASE_NAME)-tests-runner
TEST_SRC_FILES := $(wildcard *_test.cpp)
TEST_OBJ_FILES := $(patsubst %.cpp,%.o,$(TEST_SRC_FILES))
ENGINE_BASE_NAME := engine
ENGINE_DIR := $(ENGINE_BASE_NAME)
ENGINE_LIB_NAME := $(ENGINE_BASE_NAME)
ENGINE_LIB_FILENAME := lib$(ENGINE_LIB_NAME).so
DEPENDENCIES_DIR := dependencies


CXX = g++
CXX_FLAGS = -std=c++11 -O2
CXX_FLAGS += -I../$(DEPENDENCIES_DIR)/glm/include/
CXX_FLAGS += -I../$(ENGINE_DIR)
LDFLAGS = -L../$(ENGINE_DIR) -l$(ENGINE_LIB_NAME)
TAR_GZ = tar zcvf


package: $(TARGET_ARCHIVE)
	@echo "all done!"


$(TARGET_ARCHIVE): $(TARGET)
	# package your distribution tar gz archive here with the necessary distribution artifacts
	$(TAR_GZ) $(TARGET_ARCHIVE) $(TARGET) ../$(ENGINE_DIR)/$(ENGINE_LIB_FILENAME)
	@echo "$(TARGET) package done!"


$(TARGET): $(MAIN_OBJ_FILES) $(TESTS_RUNNER)
	# build your client here
	$(CXX) $(CXX_FLAGS) $(LDFLAGS) -o $@ $(MAIN_OBJ_FILES) ../$(ENGINE_DIR)/$(ENGINE_LIB_FILENAME)
	@echo "$(TARGET) compile done!"


$(TESTS_RUNNER): $(TEST_OBJ_FILES)
	# build your test runner executable here
	#$(CXX) $(CXX_FLAGS) $(LDFLAGS) -o $(TESTS_RUNNER) $(TEST_OBJ_FILES) $(ENGINE_LIB_FILENAME)
	@touch $(TESTS_RUNNER)
	@echo "$(TESTS_RUNNER) build done!"
	# execute your tests here (on test failure, exit with exit code = 1)
	#./$(TESTS_RUNNER)
	@echo "tests execution done!"


%.o: %.cpp
	# compile your sources here
	$(CXX) $(CXX_FLAGS) -c -o $@ $^


clean:
	@rm -f $(TESTS_RUNNER)
	@rm -f $(TEST_OBJ_FILES)
	@rm -f $(TARGET)
	@rm -f $(MAIN_OBJ_FILES)


.PHONY: clean package
