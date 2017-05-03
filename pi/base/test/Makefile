CC=g++
LDFLAGS=
CPPFLAGS=-I /home/seth/git-repo/pi-net/
Test=AtomicUnitTest    \
	 MutexUnitTest     \
	TimestampUnitTest
all:$(Test)

$(Test):
	$(CC) -o $@ $(filter %.cc,$^) $(CPPFLAGS) -lpthread

AtomicUnitTest: AtomicUnitTest.cc

MutexUnitTest: MutexUnitTest.cc ../Thread.cc

TimestampUnitTest: TimestampUnitTest.cc ../Timestamp.cc
.PHONY:clean

clean:
	rm -rf $(Test)
