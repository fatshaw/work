lib=-lrt
mempool=freelist.o
global=globalMemAlloc.o
FLAGS=-DDEBUG -g 

all: mem global

global:$(global)
	g++ -o global $(global)

mem:$(mempool)
	g++ -o mem $(mempool)

$(mempool):%.o:%.cpp
	g++ $(FLAGS) -c $< -o $@

clean:
	rm -f $(mempool)  mem
	rm -f $(global) global
