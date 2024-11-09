CC=gcc
CFLAGS=-Wall -Wextra -Werror -std=c99 -pedantic -O3


all: main-part1 main-part2 main-part3
	$(CC) $(CFLAGS) -o main-part1 main-part1.c
	$(CC) $(CFLAGS) -o main-part2 main-part2.c
	$(CC) $(CFLAGS) -o main-part3 main-part3.c

clean:
	rm -f main-part1 
# main-part2 main-part3

run: all
	./main-part1 ../../../inputs/2024/2/1.txt && ./main-part2 ../../../inputs/2024/2/2.txt && ./main-part3 ../../../inputs/2024/2/3.txt

.PHONY: all clean