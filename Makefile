all : bc clean
List.o : List.c List.h
	        gcc -Wall -c -g List.c
Operations.o : Operations.c Operations.h List.c List.h
	        gcc -Wall -c -g Operations.c
Stack.o : Stack.c Stack.h
	        gcc -Wall -c -g Stack.c
Infix.o : Stack.c Stack.h Infix.c Infix.h Operations.c Operations.h
	        gcc -Wall -c -g Infix.c
Help.o : Help.c Help.h
		gcc -Wall -c -g Help.c
Main.o : Operations.c Operations.h List.c List.h Infix.c Infix.h Help.c Help.h Main.c
		gcc -Wall -c -g Main.c
bc : List.o Operations.o Stack.o Infix.o Help.o Main.o
	        gcc -Wall -g List.o Operations.o Stack.o Infix.o Help.o Main.o -o bc
clean :
	        rm *.o
