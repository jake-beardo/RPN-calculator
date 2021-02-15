CFLAGS = -std=c99 -Wall

CFILES =\
	bigint.c\
	rpn.c\
	rational.c

HFILES =\
	bigint.h\
	rpn.h\
	rational.h



all: rpn.exe

rpn.exe: rpnmain.c $(CFILES) $(HFILES)
	gcc rpnmain.c $(CFILES) -o rpn.exe
test: rpn.exe
	python tests/runtests.py -k
zip:
	cd .. && zip -r rpn-2.0.zip rpn-2.0 -x rpn-2.0/.DS_Store rpn-2.0/rpn.exe
