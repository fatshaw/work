lib=-lrt
FLAGS=-DDEBUG -DAFUNIX
sobj=network.o Server.o Client.o ../header/header.o
starget=../server


Server:$(sobj)
	g++ -o $(starget) $(sobj) $(lib)

$(sobj):%.o:%.cpp
	g++ $(FLAGS) -c $< -o $@

clean:
	rm -f $(sobj) $(starget)
