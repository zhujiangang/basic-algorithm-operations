WMATCH = wmatch.c pairs.c pointer.c term.c unpairs.c readgraph.c

wmatch: main.o wmatch.o glib.o
	cc -o wmatch main.o wmatch.o glib.o -lm

$(WMATCH): match.defs

wmatch.o: $(WMATCH)
	cc -c wmatch.c -lm

