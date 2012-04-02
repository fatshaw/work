MYSQL_INC := $(shell mysql_config --include)
MYSQL_LIB := $(shell mysql_config --libs)  -L/usr/lib/mysql -lmysqlclient -lmysqlpp

MEMCACHED_LIB := -lmemcached

CXXFLAGS = -Wall -pedantic -Wno-deprecated

INCPATH  = $(MYSQL_INC) -I/usr/include/mysql++

SRC      = extractdict.cpp  matchdict.cpp englishdict.cpp channel.cpp helper.cpp service.cpp md5.cpp memcachedst.cpp frequencylimiter.cpp
OBJ      = $(SRC:.cpp=.o)
EXE      = service

$(EXE): $(OBJ)
	$(CXX) $(MYSQL_LIB)  $(MEMCACHED_LIB) $^ -o $@

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCPATH) -c $< -o $@

clean:
	$(RM) $(OBJ)

cleanall: clean
	$(RM) $(EXE)
