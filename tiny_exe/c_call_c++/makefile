test : a.o b.o
	g++ -o test a.o b.o
a.o : a.c
	gcc -c a.c
b.o : b.cpp
	g++ -c b.cpp
clean : 
	rm a.o b.o test
