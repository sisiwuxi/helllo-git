test : a.o b.o
	g++ -o test a.o b.o
a.o : a.cpp
	g++ -c a.cpp
b.o : b.c
	gcc -c b.c
clean : 
	rm a.o b.o test
