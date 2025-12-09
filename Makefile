# ========================================================
# Project: Real-Time Face Detection using OpenCV and Dlib
# Makefile for building the project with OpenCV and Dlib
# Author: Radithole Maripa
# Date: 12/09/2025
# Description:
#   Compiles all C++ source files in the project and links
#   against OpenCV and Dlib libraries.
#   Supports Windows with MinGW.
# ========================================================

# ---------------------
# Target executable
# ---------------------
TARGET := FaceDetect.exe

# Find all C++ source files in the current directory
SRC := $(wildcard *.cpp)

# Directory to store object files
BUILD_DIR := build

# List of object files (one per source file)
OBJ := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SRC))

# ---------------------
# Compiler settings
# ---------------------
CXX := g++

# C++17 standard, optimization, AVX instructions
CXXFLAGS := -std=c++17 -O2 -mavx

# ---------------------
# OpenCV paths
# ---------------------
OPENCV_DIR := C:/OpenCV/install
OPENCV_LIB := $(OPENCV_DIR)/x64/mingw/lib
OPENCV_INCLUDES := $(OPENCV_DIR)/include

# ---------------------
# Dlib paths
# ---------------------
DLIB_DIR := C:/Dlib
DLIB_INCLUDES := $(DLIB_DIR)/include
DLIB_LIB  := $(DLIB_DIR)/lib

# ---------------------
# Include paths for compiler
# ---------------------
INCLUDES := -I$(OPENCV_INCLUDES) \
	-I$(DLIB_INCLUDES)

# ---------------------
# Library paths for linker
# ---------------------
LIBDIRS := -L$(OPENCV_LIB) \
	-L$(DLIB_LIB)

# ---------------------
# Libraries to link
# ---------------------
LIBS := -lopencv_core4110 \
	-lopencv_highgui4110 \
	-lopencv_imgproc4110 \
	-lopencv_videoio4110 \
	-ldlib \
	-luser32 \
	-lgdi32 \
	-lwinmm \
	-lshell32 \
	-lole32 \
	-loleaut32 \
	-luuid \
	-lversion \
	-lws2_32 \
	-lcomctl32

# ---------------------
# Build rules
# ---------------------

# Default rule: create build directory and build target
all: $(BUILD_DIR) $(TARGET)

# Create build directory if it doesn't exist
$(BUILD_DIR):
	if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)

# Link object files to produce the final executable
$(TARGET): $(OBJ)
	$(CXX) $(OBJ) $(LIBDIRS) $(LIBS) -o $@

# Compile each C++ source file into an object file
$(BUILD_DIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Remove build directory and executable
clean:
	if exist $(BUILD_DIR) rmdir /S /Q $(BUILD_DIR)
	if exist $(TARGET) del /Q $(TARGET)

# Mark these as "phony" targets (not actual files)
.PHONY: all clean
