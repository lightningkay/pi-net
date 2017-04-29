CC=g++
LDFLAGS=
CPPFLAGS=-I /home/seth/git-repo/pi-net/
Test=AtomicUnitTest    \
	 MutexUnitTest     \

all:$(Test)

$(Test):
	$(CC) -o $@ $(filter %.cc,$^) $(CPPFLAGS) -lpthread

AtomicUnitTest: AtomicUnitTest.cc

MutexUnitTest: MutexUnitTest.cc

.PHONY:clean

clean:
	rm -rf $(Test)
