.PHONY: clean

all: FTNTerms1.0.0.c menu.h
	gcc -m32 FTNTerms1.0.0.c -o FTNTerms -lncurses -lm

clean:
	-rm FTNTerms > /dev/null
