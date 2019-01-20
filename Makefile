all: parta partb
parta: phistogram.c
	cc -Wall -g -o	phistogram phistogram.c

partb: thistogram.c
	cc -Wall -g -o	thistogram thistogram.c -lpthread
