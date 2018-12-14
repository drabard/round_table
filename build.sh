./compile_all_templates.sh
clang++-3.9 -g -Wall -Werror -I../src \
-Ideps/drb_graphics/include/ \
-Ideps/glfw-3.2.1/include/ \
-Ideps/imgui-1.65/ \
 -fno-exceptions -ferror-limit=1 \
      src/main.cpp \
      deps/drb_graphics/lib/libdrb_graphics.a \
      deps/glfw-3.2.1/lib/libglfw3.a \
      deps/imgui-1.65/imgui.a \
      -o round_table -lGL -ldl -lm -lX11 -lXxf86vm -lpthread -lXrandr -lXinerama -lXcursor