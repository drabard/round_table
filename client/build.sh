clang++ -g -std=c++11 -Wall -Werror -I../src \
-I../deps/drb_graphics/include/ \
-I../deps/glfw-3.2.1/include/ \
-I../deps/imgui-1.65/ \
-I../deps/jsmn-1.0.0/ \
-fno-exceptions -ferror-limit=1 \
      src/main.cpp \
      src/window.cpp \
      src/gui.cpp \
      src/node.cpp \
      src/scene.cpp \
      src/rendering.cpp \
      src/input.cpp \
      src/network.cpp \
      src/stb_image.cpp \
      ../deps/jsmn-1.0.0/jsmn.a \
      ../deps/drb_graphics/lib/libdrb_graphics.a \
      ../deps/glfw-3.2.1/lib/libglfw3.a \
      ../deps/imgui-1.65/imgui.a \
      -o round_table -lGL -ldl -lm -lX11 -lXxf86vm -lpthread -lXrandr -lXinerama -lXcursor