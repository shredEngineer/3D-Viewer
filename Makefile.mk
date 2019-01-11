BIN=3D-Viewer.exe

CC=g++
SRC=$(wildcard *.cpp)

CPPFLAGS=-std=c++11 -Wall -I"glfw\include"
LDFLAGS=-static-libgcc -static-libstdc++ -L"glfw\lib" -Wl,-static -lstdc++ -lwinpthread -lglfw3 -lwinmm -lgdi32 -lopengl32

default: build start

build:
	windres App.rc -O coff -o App.res
	$(CC) $(CPPFLAGS) -o $(BIN) $(SRC) App.res $(LDFLAGS)

start:
	@$(BIN)
