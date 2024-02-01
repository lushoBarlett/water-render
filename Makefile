CC = g++
CFLAGS = -std=c++17
LDFLAGS = -lglfw -lGLEW -lGL -lm

TARGET = water

SOURCES = water.cpp \
          vendor/stb/stb_image.cpp \

HEADERS = log.hpp \
          shader.hpp \
	  mesh.hpp \
	  texture.hpp \
	  renderer.hpp \
	  vendor/stb/stb_image.hpp \

all: $(TARGET) sanitize

$(TARGET): $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -o $@ $(SOURCES) $(LDFLAGS)

sanitize: $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -fsanitize=address -o $@ $(SOURCES) $(LDFLAGS)

clean:
	rm -f $(TARGET)
	rm -f sanitize
.PHONY: sanitize
