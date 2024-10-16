.PHONY:all
all:test_utill.exe test_level.exe test_log.exe

test_utill.exe:test_utill.cc
	g++ -o $@ $^ -std=c++11

test_level.exe:test_level.cc
	g++ -o $@ $^ -std=c++11

test_logmsg.exe:test_logmsg.cc
	g++ -o $@ $^ -std=c++11

test_log.exe:test_log.cc
	g++ -o $@ $^ -std=c++11 -lpthread

.PHONY:clean
clean:
	rm -f test_utill.exe test_level.exe test_logmsg test_log.exe