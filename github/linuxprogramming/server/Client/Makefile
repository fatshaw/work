lib=-lrt
cobj=client.o ../header/header.o
ctarget=../client
FLAGS=-DDEBUG -DAFUNIX
Client:$(cobj)
	g++ -o $(ctarget) $(cobj) $(lib)

$(cobj):%.o:%.cpp
	g++ $(FLAGS) -c $< -o $@

clean:
	rm -f  $(cobj) $(ctarget)
