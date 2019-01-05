clang -g -c -Wall -Werror -std=c11 -Iinclude/ -Isrc/ -fno-exceptions -ferror-limit=1 \
      src/graphics.c \
      src/gpu.c \
	  src/math.c \
	  src/imvis.c \
	  src/camera.c \
      src/GL/gl3w.c
ar rcs lib/libdrb_graphics.a *.o
rm -rf *.o
