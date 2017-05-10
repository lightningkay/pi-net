target=$(basename $(wildcard *.cc))

boosttest: LDLIBS += -lboost_unit_test_framework

boosttest: CC = g++

all:$(target)

%.o:%.cc

.PHONY:clean

clean:
	rm -rf $(target)
