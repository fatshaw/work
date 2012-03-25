XLC=xlc -tb -B/home/xiaochj/tobey.dev/
XLCFLAGS=-qlist -qdebug=plstfe:plstopt:wdump -g -O0 -DDEBUG
#XLCFLAGS=-qlist -qdebug=plstfe:plstopt:tracefld:traceil -g -O0 -q64
#XLCFLAGS=-qlist -qdebug=plstfe:plstopt:tracell -g -O0 -q64
#XLCFLAGS=-qlist -qdebug=plstfe:crlog -g -O2 -q32
XLCFLAGS=-qlist -qdebug=plstfe:tracell -g -O0 -q64 

UNIT_TEST=tmul tneg tnor tnand tsl tsr tload tstore tcrlogic tfarith tsr addc
OVERFLOW=fcmpo neg addc subf store  
#test
####### rule ########

#all: unit_test

#.PHONY: clean

CB:
	@make -f Makefile $(subst C,,$@)

A:	a.o
	gcc	-o	a	a.o
B:
	@echo "adf"

#unit_test: $(UNIT_TEST)
unit_test: $(OVERFLOW)


%: %.c common.o
	$(XLC) $(XLCFLAGS) common.o $< -o $@

common.o: common.c common.h
	$(XLC) $(XLCFLAGS) -c $< -o $@

clean:
	rm -f $(UNIT_TEST) *.o

clobber:
	rm -f $(UNIT_TEST) *.o *.lst

run:
	for i in $(UNIT_TEST); do ./$$i; done
