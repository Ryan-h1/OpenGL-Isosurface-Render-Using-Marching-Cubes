CC = g++
CXXFLAGS = -Wall -Wextra -Wpedantic -O3 -std=c++17
LDFLAGS = -lglfw -lGLEW -lOpenGL -lpthread
TARGET = assign5

# List all the source files
SRCS = main.cpp

# List all the header files
HDRS = headers.hpp Constants.hpp HelperFunctions.hpp shader.hpp UserConstants.hpp TriTable.hpp Worldboundaries.hpp

# Set the obj directory path
OBJ_DIR = ObjectFiles

# Generate a list of object files from the source files
OBJS = $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(TARGET)

# Compile each source file into an object file
$(OBJ_DIR)/%.o: %.cpp $(HDRS)
	mkdir -p $(OBJ_DIR)
	$(CC) -c $(CXXFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -rf $(TARGET) $(OBJ_DIR)
