CC = gcc
CFLAGS = -g -Wall -std=c11 -Wno-incompatible-pointer-types
CPPFLAGS = $(addprefix -I,$(SOURCE_DIRS))
LDFLAGS = -lm

BIN=test

BUILD_DIR = ./bin
SOURCE_DIR = ./src
SOURCE_DIRS = $(shell find $(SOURCE_DIR) -type d)

SOURCES := $(shell find $(SOURCE_DIR) -name '*.c')
OBJECTS := $(SOURCES:%.c=%.o)

vpath %.c $(SOURCE_DIRS)

all: $(BIN)

$(BIN): $(notdir $(OBJECTS))
	$(CC) $(CFLAGS) $(CPPFLAGS) $(addprefix $(BUILD_DIR)/, $^) -o $(BIN) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $(BUILD_DIR)/$@ 

clean:
	@rm -f $(BUILD_DIR)/*.o $(BIN)

show:
	@echo $(notdir $(OBJECTS))