./format.sh
./compile_all_templates.sh
pushd deps/drb_graphics/
./build.sh
popd
clang++ -g -std=c++11 -Wall -Werror -I../src \
-Ideps/drb_graphics/include/ \
-Ideps/glfw-3.2.1/include/ \
-Ideps/imgui-1.65/ \
 -fno-exceptions -ferror-limit=1 \
      src/main.cpp \
      src/window.cpp \
      src/gui.cpp \
      src/scene.cpp \
      src/rendering.cpp \
      src/input.cpp \
      src/stb_image.cpp \
      deps/drb_graphics/lib/libdrb_graphics.a \
      deps/glfw-3.2.1/lib/libglfw3.a \
      deps/imgui-1.65/imgui.a \
      -o round_table -lGL -ldl -lm -lX11 -lXxf86vm -lpthread -lXrandr -lXinerama -lXcursor