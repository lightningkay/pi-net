CC=g++
LDFLAGS=
CPPFLAGS=-I /home/seth/git-repo/pi-net/
SRC=$(wildcard *.cc)
OBJ=$(patsubst %.cc,%.o,$(SRC))
TARGET=$(basename $(SRC))

all:$(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $<
%.o:%.cc
	$(CC) -o $@ -c $< $(CPPFLAGS)

.PHONY:clean

clean:
	rm -rf $(TARGET)
	rm -rf $(OBJ)
