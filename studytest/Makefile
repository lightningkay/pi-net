target=$(basename $(wildcard *.cc))

boosttest: LDLIBS += -lboost_unit_test_framework

boosttest: CC = g++

all:$(target)

%.o:%.cc
	$(CC) -c $< -o $@

.PHONY:clean

clean:
	rm -rf $(target)
