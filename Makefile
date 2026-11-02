CC = gcc
CCFLAGS = -Wall -Wextra -g

SRC = $(wildcard *.c)
HDR = $(wildcard *.h)
BUILD_DIR = bin/DEBUG
TARGET = $(BUILD_DIR)/output

all: $(TARGET)

$(TARGET): $(SRC) $(HDR)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CCFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./${TARGET}

release:
	$(MAKE) BUILD_DIR=bin/RELEASE  CCFLAGS="-Wall -Wextra -O3 -DNDEBUG" all

clean:
	rm -rf bin

.PHONY: all run release clean