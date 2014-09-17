
LDFLAGS = -lrt -m32
COPT := $(shell if [ `uname -s` = SunOS ] ; then echo -xO2 ; else echo -m32 -O2 ; fi)
CFLAGS = -D$(shell uname) $(COPT)

all: watchf

watchf: watchf.o

watchf.o: watchf.c

clean:
	rm -f watchf watchf.o

install:
	cp watchf $(PLATFORMDIR)/bin
