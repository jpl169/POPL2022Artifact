CC = gcc

float34ROFNames = log2

float34ROSrcDir = source/float34RO
float34ROObjDir = obj/float34RO
libDir = lib

float34ROSrc = $(patsubst %, $(float34ROSrcDir)/%.c, $(float34ROFNames))
float34ROObj = $(patsubst %, $(float34ROObjDir)/%.o, $(float34ROFNames))

all: $(libDir)/small34ROMathLib.a

$(small34ROObjDir)/%.o: $(float34ROSrcDir)/%.c
	@mkdir -p $(float34ROObjDir)
	$(CC) -std=c11 -O3 -c $^ -Iinclude/ -o $@

$(libDir)/small34ROMathLib.a: $(float34ROObj)
	@mkdir -p $(libDir)
	ar rc $@ $^

small34ROmlib: $(libDir)/small34ROMathLib.a

clean:
	rm -rf obj lib *.dSYM
