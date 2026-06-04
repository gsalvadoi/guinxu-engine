CXX = g++
CXXFLAGS = -std=c++17 -g -O0 -Wall -Iinclude

SRC = main.cpp \
      src/models.cpp \
      src/textures.cpp \
      src/colors.cpp \
      src/draw2d.cpp \
      src/draw3d.cpp \
      src/transforms.cpp \
      src/vectors.cpp


TARGET = build/GuinxuEngine

SDLFLAGS = $(shell pkg-config --cflags --libs sdl2 SDL2_image)

all:
	$(CXX) $(SRC) $(CXXFLAGS) -o $(TARGET) $(SDLFLAGS)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET)
