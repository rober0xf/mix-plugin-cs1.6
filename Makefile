.PHONY: help compile list clean

SRC_DIR := src
BUILD_DIR := build
INC_DIR := include
AMXX_DIR := ./tools/amxx

PLUGINS := $(wildcard $(SRC_DIR)/*.sma)

help:
	@echo "Available commands:"
	@echo "  make compile - compile all plugins"
	@echo "  make list    - list source plugins"
	@echo "  make clean   - remove compiled plugins"

compile: $(PLUGINS:$(SRC_DIR)/%.sma=$(BUILD_DIR)/%.amxx)

$(BUILD_DIR)/%.amxx: $(SRC_DIR)/%.sma
	mkdir -p $(BUILD_DIR)
	LD_LIBRARY_PATH=$(AMXX_DIR) $(AMXX_DIR)/amxxpc -i$(INC_DIR) $< -o$@

list:
	@for f in $(PLUGINS); do \
		echo "$$(basename $$f .sma)"; \
	done

clean:
	rm -rf $(BUILD_DIR)
