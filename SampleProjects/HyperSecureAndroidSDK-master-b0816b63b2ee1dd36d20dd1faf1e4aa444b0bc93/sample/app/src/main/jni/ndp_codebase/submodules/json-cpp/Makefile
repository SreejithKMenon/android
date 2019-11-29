# Simple Makefile
t = $(shell mkdir -p obj bin)

# Your compiler
CC = g++

# Include directories
INCLUDE_DIRS := ./include

CXXFLAGS = -std=c++11
CXXFLAGS += $(foreach includedir,$(INCLUDE_DIRS),-I$(includedir))

all: obj/json_reader.o obj/json_writer.o obj/json_value.o bin/example1

bin/example1: examples/example1.cpp obj/json_reader.o obj/json_value.o\
	obj/json_writer.o
	$(CC) -o $@ $^ $(CFLAGS) $(CXXFLAGS) 

obj/json_reader.o: src/json/json_reader.cpp
	mkdir -p obj bin
	$(CC) -c $(CXXFLAGS) -o $@ $<

obj/json_writer.o: src/json/json_writer.cpp
	$(CC) -c $(CXXFLAGS) -o $@ $<

obj/json_value.o: src/json/json_value.cpp
	$(CC) -c $(CXXFLAGS) -o $@ $<

clean:
	rm -rf obj/*
	rm -rf bin/*









