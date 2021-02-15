CFLAGS = -std=c99 -Wall

CFILES =\
	bigint.c\
	rpn.c\
	rationalinuse.c

HFILES =\
	bigint.h\
	rpn.h\
	rational.h



all: rpn.exe

rpn.exe: rpnmain.c $(CFILES) $(HFILES)
	gcc rpnmain.c $(CFILES) -o rpn.exe
test: rpn.exe
	python tests/runtests.py -k
