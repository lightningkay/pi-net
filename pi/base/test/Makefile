CC=g++
LDFLAGS=
CPPFLAGS=-I ../../../
Test=AtomicUnitTest    \
	MutexUnitTest      \
	TimestampUnitTest  \
	ThreadUnitTest     \
	SingletonTest


all:$(Test)

$(Test):
	$(CC) -o $@ $(filter %.cc,$^) $(CPPFLAGS) $(LDFLAGS)

AtomicUnitTest: AtomicUnitTest.cc

MutexUnitTest:  LDFLAGS += -lpthread

MutexUnitTest:  MutexUnitTest.cc	\
				../Thread.cc		\
				../Logging.cc		\
				../TimeZone.cc		\
				../Timestamp.cc		\
				../Date.cc			\
				../Exception.cc		\
				../LogStream.cc		\


ThreadUnitTest:  LDFLAGS += -lpthread
ThreadUnitTest: ThreadUnitTest.cc	\
				../Thread.cc		\
				../Logging.cc		\
				../TimeZone.cc		\
				../Timestamp.cc		\
				../Date.cc			\
				../Exception.cc		\
				../LogStream.cc		\


TimestampUnitTest: TimestampUnitTest.cc ../Timestamp.cc

SingletonTest: LDFLAGS += -lpthread
SingletonTest: SingletonTest.cc     \
				../Thread.cc		\
				../Logging.cc		\
				../TimeZone.cc		\
				../Timestamp.cc		\
				../Date.cc			\
				../Exception.cc		\
				../LogStream.cc		\


.PHONY:clean
clean:
	rm -rf $(Test)
