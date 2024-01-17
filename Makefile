TESTCASE1 ?= test1.c
TESTCASE2 ?= test2.c

all:
	gcc ${TESTCASE1} -o test1 -lpthread
	gcc ${TESTCASE2} -o test2 -lpthread

clean:
	rm -f test1
	rm -f test2