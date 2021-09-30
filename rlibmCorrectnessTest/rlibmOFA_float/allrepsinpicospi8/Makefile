CC = gcc

funcNames = Sinpi Cospi

src = $(patsubst %, %.c, $(funcNames))
obj = $(patsubst %, %, $(funcNames))

all: $(obj)

%: %.c
	$(CC) -O3 -std=c11 -I../../../helper/ -I../../../include/  $^ ../../../lib/float34ROMathLib.a -lmpfr -lgmp -o $@

clean:
	rm -rf $(obj) *.dSYM
