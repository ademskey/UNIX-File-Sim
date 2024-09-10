prog: main.o commands.o
	g++ -fopenmp -g main.o commands.o -o linux_sim

main.o: main.c header.h
	g++ -fopenmp -c -g -Wall -std=c++11 main.c

commands.o: commands.c commands.h
	g++ -fopenmp -c -g -Wall -std=c++11 commands.c

clean: 
	-rm *.o
	
run:
	@./linux_sim