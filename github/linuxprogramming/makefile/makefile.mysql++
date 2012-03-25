MYSQL_INC := $(shell mysql_config --include)
CXX_LIB := $(shell mysql_config --libs) -L/usr/lib64/mysql -lmysqlclient -lmysqlpp

CXXFLAGS= -Wno-deprecated

INCPATH = $(MYSQL_INC) -I/usr/include/mysql++ -I.

LIB_DIR=/usr/local/lib

all:test

object=test.o

test:$(object)
	g++ $(CXXFLAGS) $(CXX_LIB) -o test $(object)

$(object):%.o:%.cpp
	g++ $(CXXFLAGS) $(INCPATH) -c $< -o $@

clean:
	rm -f *.o test

