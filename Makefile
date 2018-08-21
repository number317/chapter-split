cc = gcc
target = chapter_split
source = chapter_split.c

$(target): $(source)
	$(cc) -g -o $(target) -Wall -std=c11 $(source) -lm

clean:
	rm $(target)
