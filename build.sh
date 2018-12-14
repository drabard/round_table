./compile_all_templates.sh
clang-3.9 -g -Wall -Werror -std=c11 -I../src -I../ext/drb_graphics/include -fno-exceptions -ferror-limit=1 \
      src/main.c \
      deps/drb_graphics/lib/libdrb_graphics.a \
      deps/glfw-3.2.1/lib/libglfw3.a \
      deps/imgui-1.65/imgui.a \
      -o round_table -lSDL2 -lGL -ldl -lm
