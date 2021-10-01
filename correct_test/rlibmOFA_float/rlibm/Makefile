CC = gcc

funcNames = Log Log2 Log10 Exp Exp10 Exp2 Sinh Cosh Sinpi Cospi

src = $(patsubst %, %.c, $(funcNames))
obj = $(patsubst %, %, $(funcNames))

all: $(obj)

%: %.c
	$(CC) -O3 -std=c11 -I../../../helper/ -I../../../include/  $^ ../../../lib/float34ROMathLib.a -o $@

clean:
	rm -rf $(obj) *.dSYM
