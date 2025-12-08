TEST_DIR = test
RUNNER_DIR = test/test_runners
# Generate lists of test files and runners
TEST_SOURCES = $(foreach module,$(MODULES),$(foreach test,$($(module)_TESTS),$(TEST_DIR)/Test$(module)_$(test).cc))
TEST_RUNNERS = $(foreach module,$(MODULES),$(foreach test,$($(module)_TESTS),$(RUNNER_DIR)/Test$(module)_$(test)_Runner.cc))
TEST_EXES = $(foreach module,$(MODULES),$(foreach test,$($(module)_TESTS),$(RUNNER_DIR)/Test$(module)_$(test)))

# Pattern rule for generating test runners
$(RUNNER_DIR)/Test%_Runner.cc: $(TEST_DIR)/Test%.cc
	@mkdir -p $(RUNNER_DIR)
	ruby $(UNITY_ROOT)/auto/generate_test_runner.rb $< $@

# Individual test targets (can be run separately)
define TEST_TEMPLATE
test-$(1)-$(2): $(RUNNER_DIR)/Test$(1)_$(2)_Runner.cc
	cc $(INC_DIRS) -I test $(SYMBOLS) $(UNITY_ROOT)/src/unity.c $(1).cc $(TEST_DIR)/*TestData.cc $(TEST_DIR)/Test$(1)_$(2).cc $(RUNNER_DIR)/Test$(1)_$(2)_Runner.cc -o $(RUNNER_DIR)/Test$(1)_$(2)
	@echo "Running test: $(1)_$(2)"
	@$(RUNNER_DIR)/Test$(1)_$(2)

endef

# Generate individual test targets
$(foreach module,$(MODULES),\
	$(foreach test,$($(module)_TESTS),\
		$(eval $(call TEST_TEMPLATE,$(module),$(test)))))

# Main test target - runs all tests
tests: $(foreach module,$(MODULES),\
		$(foreach test,$($(module)_TESTS),\
			test-$(module)-$(test)))

clean::
	rm -f test/test_runners/*

.PHONY: $(foreach module,$(MODULES),$(foreach test,$($(module)_TESTS),test-$(module)-$(test)))