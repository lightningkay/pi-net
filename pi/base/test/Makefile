CC=g++
LDFLAGS=
CPPFLAGS=-I /home/seth/git-repo/pi-net/
Test=AtomicUnitTest    \
	 MutexUnitTest     \
	TimestampUnitTest
all:$(Test)

#$(Test):
#	$(CC) -o $@ $(filter %.cc,$^) $(CPPFLAGS)

AtomicUnitTest: AtomicUnitTest.cc

MutexUnitTest: LDFLAGS += -lpthread
MutexUnitTest: MutexUnitTest.cc ../Thread.cc

TimestampUnitTest: TimestampUnitTest.cc ../Timestamp.cc
.PHONY:clean

clean:
	rm -rf $(Test)
