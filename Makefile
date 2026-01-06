CXX       := clang++
CXXFLAGS  := -ggdb -std=c++23 -Isrc
CPPFLAGS  := -MMD
BUILD_DIR := build

COMPILE   := $(CXX) $(CXXFLAGS) $(CPPFLAGS)

SRCS      := $(wildcard src/*.cpp)
OBJS      := $(patsubst src/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))
DEPS      := $(OBJS:.o=.d)


lox: $(OBJS)
	@$(COMPILE) $^ -o $@

$(BUILD_DIR)/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(COMPILE) -c $< -o $@


.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) lox


-include $(DEPS)


.PHONY: test-lexing
test-lexing:
	@make lox >/dev/null
	@echo "testing lox with test-lexing.lox ..."
	@./lox test/01-test-lexing.lox | diff -u --color test/01-test-lexing.lox.expected -;


.PHONY: test-lexing2
test-lexing2:
	@make lox >/dev/null
	@echo "testing lox with test-lexing2.lox ..."
	@./lox test/02-test-lexing.lox | diff -u --color test/02-test-lexing.lox.expected -;
