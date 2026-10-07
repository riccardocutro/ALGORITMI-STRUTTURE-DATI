CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -std=c11 -g -O2 -Iinclude

SRCDIR = src
INCDIR = include
MAINDIR = main
TESTDIR = test
BINDIR = bin
BUILDDIR = build

UNITY_DIR = ../Unity
UNITY_SRC = $(UNITY_DIR)/src/unity.c
UNITY_INC = -I$(UNITY_DIR)/src

LIBS_SRCS = $(SRCDIR)/sort.c $(SRCDIR)/utils.c
LIBS_OBJS = $(LIBS_SRCS:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)

all: dirs main_ex1 test_ex1 debug_view

dirs:
	mkdir -p $(BINDIR)
	mkdir -p $(BUILDDIR)

main_ex1: $(LIBS_OBJS) $(BUILDDIR)/main_ex1.o
	$(CC) $(CFLAGS) $^ -o $(BINDIR)/main_ex1

test_ex1: $(LIBS_OBJS) $(BUILDDIR)/test_ex1.o
	$(CC) $(CFLAGS) $(UNITY_INC) $(UNITY_SRC) $^ -o $(BINDIR)/test_ex1

debug_view: $(BINDIR)/debug_ex1

$(BINDIR)/debug_ex1: $(BUILDDIR)/debug_view.o $(BUILDDIR)/utils.o
	$(CC) $(CFLAGS) $^ -o $@

$(BUILDDIR)/debug_view.o: $(SRCDIR)/debug_view.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/main_ex1.o: $(MAINDIR)/main_ex1.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/test_ex1.o: $(TESTDIR)/test_ex1.c
	$(CC) $(CFLAGS) $(UNITY_INC) -c $< -o $@

clean:
	rm -rf $(BUILDDIR) $(BINDIR)

.PHONY: all clean dirs debug_view