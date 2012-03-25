object= curtest.o

lib=-lpcre -lcurl

all:$(object)
	g++ -o curtest $(object) $(lib)
	
$(object):%.o:%.cpp
	g++ -c $< -o $@
	
clean:
	rm -f *.o test