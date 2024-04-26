
all:	src/fileRead.cpp src/assembler.cpp src/include/asm.h src/include/fileRead.h
	g++ -Wall -DDEBUG -g src/fileRead.cpp src/assembler.cpp -o bin/asm

rl: bin/asm
	valgrind bin/asm /home/alexandr/Документы/Programming-language/data/code.txt data/bytecode.txt