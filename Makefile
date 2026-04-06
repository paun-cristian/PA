CC = gcc
CFLAGS = -g -Wall -std=c11 -Wno-incompatible-pointer-types
CPPFLAGS = $(addprefix -I,$(SOURCE_DIRS))
LDFLAGS = -lm

BIN=test
PACKAGE = <numele_vostru>.zip

BUILD_DIR = ./bin
SOURCE_DIR = ./src
SOURCE_DIRS = $(shell find $(SOURCE_DIR) -type d)

SOURCES := $(shell find $(SOURCE_DIR) -name '*.c')
HEADERS := $(shell find $(SOURCE_DIR) -name '*.h')
OBJECTS := $(SOURCES:%.c=%.o)
ZIP_FILES := $(SOURCES) $(HEADERS) .gitignore

vpath %.c $(SOURCE_DIRS)

all: $(BIN)

zip: $(PACKAGE)

$(PACKAGE): $(ZIP_FILES)
	rm -f $@
	zip -r $@ .git
	zip -j $@ $(ZIP_FILES)

$(BIN): $(notdir $(OBJECTS))
	$(CC) $(CFLAGS) $(CPPFLAGS) $(addprefix $(BUILD_DIR)/, $^) -o $(BIN) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $(BUILD_DIR)/$@ 

clean:
	@rm -f $(BUILD_DIR)/*.o $(BIN)

show:
	@echo $(notdir $(OBJECTS))