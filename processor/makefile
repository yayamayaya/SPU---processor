sp = src/processor.h
spcpp = src/processor.cpp

func = ../../stack/bin/functions.o
gl = ../../stack/bin/global.o
fr = ../../common/file_reading/bin/file_reading.o

all: spu.o $(gl) $(fr) $(func)
	g++ -Wall -o bin/spu bin/spu.o  $(fr) $(gl)

spu.o: $(spcpp) $(sp)
	g++ -Wall -DDEBUG -g -c $(spcpp) -o bin/spu.o

rp:	bin/spu
	valgrind bin/spu ../assembler/data/bytecode.txt