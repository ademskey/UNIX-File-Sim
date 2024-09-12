prog: main.o commands.o
	gcc -fopenmp -g pa0_caudle.o commands.o -o linux_sim

main.o: pa0_caudle.c header.h
	gcc -fopenmp -c -g -Wall -std=c11 pa0_caudle.c

commands.o: commands.c commands.h
	gcc -fopenmp -c -g -Wall -std=c11 commands.c

clean: 
	-rm *.o; rm linux_sim
	
run:
	@./linux_sim
