object=fcgiclient.o

all:$(object)
	g++ -o fcgiclient $(object)
	
$(object):%.o:%.cpp
	g++ -c $< -o $@
	
clean:
	rm - *.o fcgiclient