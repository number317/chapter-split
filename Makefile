cc = gcc
target = chapter_split
source = chapter_split.c

$(target): $(source)
	$(cc) -g -o $(target) -Wall -std=gnu11 $(source) -lm --static

clean:
	rm $(target)
