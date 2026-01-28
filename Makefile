CXX = g++
CXXFLAGS = -std=c++17 -Wall

SRC = main.cpp \
      src/app/glut.cpp \
      $(wildcard src/ray/*.cpp) \
      $(wildcard src/vectors/*.cpp) \
      $(wildcard src/colors/*.cpp) \
      $(wildcard src/material/*.cpp) \
      $(wildcard src/objects/*.cpp) \
      $(wildcard src/transform/*.cpp) \
      $(wildcard src/texture/*.cpp) \
      $(wildcard src/camera/*.cpp)

LIBS = -lfreeglut -lopengl32 -lglu32

app: $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o app.exe $(LIBS)

.PHONY: clean

clean:
	del /F /Q app.exe 2>nul

