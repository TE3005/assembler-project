mmn14: main.o func.o pass2.o pass1.o 
	gcc -Wall -ansi -pedantic main.o func.o pass2.o pass1.o -o assembler

main.o: main.c global.h header.h 
	gcc  -Wall -ansi -pedantic main.c -c

func.o: func.c global.h header.h
	gcc -Wall -ansi -pedantic func.c -c

pass2.o: pass2.c global.h header.h
	gcc -Wall -ansi -pedantic pass2.c -c

pass1.o: pass1.c global.h header.h
	gcc -Wall -ansi -pedantic pass1.c -c
