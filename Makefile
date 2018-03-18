CC = gcc
OS = $(shell uname -s)



SRC_DIR = ./src
INC_DIR = ./include
INC_DIR_HACK = ./hack_include
TEST_DIR = ./test

BUILD_DIR = ./build
OBJ_DIR = $(BUILD_DIR)/objects

TARGET_MAIN = $(BUILD_DIR)/modbusi2c

SOURCES = $(shell find $(SRC_DIR) -type f -name *.c)
HEADERS = $(shell find $(INC_DIR) -type f -name *.h)
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))


INC_FLAGS = -I$(SRC_DIR) -I$(INC_DIR)
INC_LIB = -Wl,-Bstatic -li2cdevices
ifeq ($(OS),Darwin)
	INC_FLAGS += -I$(INC_DIR_HACK)
endif


build: build_obj
	$(CC) -o $(TARGET_MAIN) $(OBJECTS) $(INC_LIB) 

build_obj: $(OBJECTS) $(HEADERS)
	@echo $(OBJECTS)

	


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(INC_FLAGS) -c $< -o $@

run: build
	$(TARGET_MAIN)

info:
	@echo $(SOURCES)
	@echo $(OBJECTS)
	@echo $(HEADERS)

clean:
	rm -rf $(OBJ_DIR)
	rm -f $(TARGET_MAIN)

