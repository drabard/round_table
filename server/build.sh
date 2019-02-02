clang++ -g -std=c++11 -Wall -Werror -I../src \
-fno-exceptions -ferror-limit=1 \
      src/main.cpp \
      -o round_table_server