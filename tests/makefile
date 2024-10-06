.PHONY:all
all:test_utill.exe test_level.exe

test_utill.exe:test_utill.cc
	g++ -o $@ $^ -std=c++11

test_level.exe:test_level.cc
	g++ -o $@ $^ -std=c++11

test_logmsg.exe:test_logmsg.cc
	g++ -o $@ $^ -std=c++11

.PHONY:clean
clean:
	rm -f test_utill.exe test_level.exe test_logmsg