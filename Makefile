CC=gcc
EDCFLAGS:= -std=gnu11 -O2 -I gpiodev/ $(CFLAGS)
EDLDFLAGS:= -lpthread $(LDFLAGS)
COBJ=gpiodev/gpiodev.o \
 detect.o

all: $(COBJ)
	$(CC) $(EDCFLAGS) -o detect.out $(COBJ) $(EDLDFLAGS)

%.o: %.c
	$(CC) $(EDCFLAGS) -o $@ -c $<

.PHONY: clean

clean:
	rm -vf *.out
	rm -vf *.o

spotless: clean
	rm -vrf doc