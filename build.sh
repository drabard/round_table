./compile_all_templates.sh
clang-3.9 -g -Wall -Werror -std=c11 -I../src -fno-exceptions -ferror-limit=1 \
      src/main.c \
      -o round_table -lSDL2 -lGL -ldl -lm
