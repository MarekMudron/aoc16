CXX = g++
CFLAGS  = -g -Wall
TARGET = aoc16

all: $(TARGET)

$(TARGET): utils.o main.o
	$(CXX) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET) *.o