CXX = g++
CXXFLAGS  = -g -Wall
CXXFLAGS += -I/usr/local/include
CXXFLAGS += -L/usr/local/lib
CXXFLAGS += --coverage
TARGET = aoc16

all: test


test: main.o utils.o test.o
	$(CXX) $(CXXFLAGS)  -o $@ $^ -lgtest -lpthread

coverage: test
	./test
	mkdir coverage
	lcov --directory . --capture --output-file coverage/coverage.info
	genhtml  --output-directory coverage coverage/coverage.info

clean:
	rm -rf $(TARGET) *.o coverage *.gcda *.gcno test