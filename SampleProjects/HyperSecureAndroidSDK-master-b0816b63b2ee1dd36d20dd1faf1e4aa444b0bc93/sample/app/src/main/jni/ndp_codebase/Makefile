# Simple Makefile
t = $(shell mkdir -p obj bin)

# Your compiler
CC = g++

# Include directories
INCLUDE_DIRS := ./include/

# Library directories
LIBRARY_DIRS := /usr/local/cuda-7.0/lib64/

# Libraries
#LIBRARIES := caffe cudart cublas curand glog gflags protobuf leveldb snappy lmdb boost_system hdf5_hl hdf5 m opencv_core opencv_highgui opencv_imgproc boost_thread stdc++ cblas atlas aerospike ev2citrusleaf event ssl crypto dlib pthread


# Compilation flags
# '-g' turns debugging flags on.
CXXFLAGS = `pkg-config --cflags opencv` -std=c++11 -O3 -fopenmp
LDFLAGS := `pkg-config --libs opencv` -O3 -fopenmp
LDFLAGS += $(foreach librarydir,$(LIBRARY_DIRS),-L$(librarydir)) \
                $(foreach library,$(LIBRARIES),-l$(library))
CXXFLAGS += $(foreach includedir,$(INCLUDE_DIRS),-I$(includedir))

# Linker flags
# When you need to add a library
LDFLAGS += -lm 

#Dlib Flags

# all is a target. main is a 'dependency' or requirement for all to be complete
all: obj/NPDFaceDetector.o obj/graph.o bin/main

bin/main: ./tools/detect_faces.cpp obj/NPDFaceDetector.o obj/graph.o
	$(CC) -o $@ $^ $(CXXFLAGS) $(LDFLAGS)

obj/NPDFaceDetector.o: ./src/NPDFaceDetector.cpp
	mkdir -p obj bin
	$(CC) -c -o $@ $^ $(CXXFLAGS)

obj/graph.o: ./src/graph.cpp
	$(CC) -c -o $@ $^ $(CXXFLAGS)


# .PHONY tells make that 'all' or 'clean' aren't _actually_ files, and always
# execute the compilation action when 'make all' or 'make clean' are used
.PHONY: all clean 

# Delete all the temporary files we've created so far
clean:
	rm -rf obj/*
	rm -rf bin/*
