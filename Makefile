CC = g++
CFLAGS = -std=c++17
LDFLAGS = -lglfw -lGLEW -lGL -lm

TARGET = water

SOURCES = water.cpp \
          vendor/stb/stb_image.cpp \
	  vendor/imgui/imgui.cpp \
	  vendor/imgui/imgui_draw.cpp \
	  vendor/imgui/imgui_widgets.cpp \
	  vendor/imgui/imgui_tables.cpp \
	  vendor/imgui/imgui_impl_glfw.cpp \
	  vendor/imgui/imgui_impl_opengl3.cpp \

HEADERS = log.hpp \
          shader.hpp \
	  mesh.hpp \
	  texture.hpp \
	  renderer.hpp \
	  vendor/stb/stb_image.hpp \
	  vendor/imgui/imgui.h \
	  vendor/imgui/imgui_internal.h \
	  vendor/imgui/imconfig.h \
	  vendor/imgui/imstb_rectpack.h \
	  vendor/imgui/imstb_textedit.h \
	  vendor/imgui/imstb_truetype.h \
	  vendor/imgui/imgui_impl_glfw.h \
	  vendor/imgui/imgui_impl_opengl3.h \

all: $(TARGET) sanitize

$(TARGET): $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -o $@ $(SOURCES) $(LDFLAGS)

sanitize: $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -fsanitize=address -o $@ $(SOURCES) $(LDFLAGS)

clean:
	rm -f $(TARGET)
	rm -f sanitize
.PHONY: sanitize
