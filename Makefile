CC = gcc
CFLAGS = -std=c11 -Wall -Os
LDFLAGS = -lpng -lwebp -lm -lpthread

c_files = $(wildcard *.c)
o_files = $(c_files:%.c=%.o)

webp2png: $(o_files)
	$(CC) $< -o $@ $(LDFLAGS)


$(o_files) : $(c_files)

clean:
	rm -rf main all webp2png *.o