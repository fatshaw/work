CC = gcc
CXX = g++
MKDIR = @mkdir -p
RM = @rm -rf

TARGET = accessloader

VPATH = .

INC =  -I. -I../include -I../../thirdparty/net_manager/src -I../../thirdparty/utils/src

CPPFLAGS = -g -Wall -Wno-invalid-offsetof $(INC) -DLIST_TEST -DDISABLE_MEMPOOL -DDEBUG

LDFLAGS = -L../lib -lnet_manager.64k -L../../thirdparty/utils/src -lutils -lpthread

$(TARGET) : main.o AccessControler.o AccessControlerUtils.o
	$(CXX) $(CPPFLAGS) -o $@ $^ $(LDFLAGS)

main.o : main.cpp 
	$(CXX) $(CPPFLAGS) -c $^ -o $@

AccessControler.o : ../src/AccessControler.cpp
	$(CXX) $(CPPFLAGS) -c $^ -o $@

AccessControlerUtils.o : ../src/AccessControlerUtils.cpp
	$(CXX) $(CPPFLAGS) -c $^ -o $@

clean :
	$(RM) *.o $(TARGET)

install :
	@mv -f $(TARGET) ../bin
