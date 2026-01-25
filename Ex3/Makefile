CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -std=c11 -g -O2 -Iinclude

BIN_DIR = bin
BUILD_DIR = build
SRC_DIR = src
TEST_DIR = test
MAIN_DIR = main
UNITY_DIR = ../Unity/src

.PHONY: all clean dirs main test

all: dirs main test

dirs:
	mkdir -p $(BIN_DIR)
	mkdir -p $(BUILD_DIR)

main: $(BIN_DIR)/main_ex3

$(BIN_DIR)/main_ex3: $(BUILD_DIR)/main_ex3.o $(BUILD_DIR)/priority_queue.o $(BUILD_DIR)/hash_table.o
	$(CC) $(CFLAGS) $^ -o $@

$(BUILD_DIR)/main_ex3.o: $(MAIN_DIR)/main_ex3.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/priority_queue.o: $(SRC_DIR)/priority_queue.c include/priority_queue.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/hash_table.o: $(SRC_DIR)/hash_table.c include/hash_table.h
	$(CC) $(CFLAGS) -c $< -o $@

test: $(BIN_DIR)/test_ex3

$(BIN_DIR)/test_ex3: $(BUILD_DIR)/test_ex3.o $(BUILD_DIR)/priority_queue.o $(BUILD_DIR)/hash_table.o $(BUILD_DIR)/unity.o
	$(CC) $(CFLAGS) $^ -o $@

$(BUILD_DIR)/test_ex3.o: $(TEST_DIR)/test_ex3.c
	$(CC) $(CFLAGS) -I$(UNITY_DIR) -c $< -o $@

$(BUILD_DIR)/unity.o: $(UNITY_DIR)/unity.c
	$(CC) $(CFLAGS) -I$(UNITY_DIR) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)/* $(BIN_DIR)/*