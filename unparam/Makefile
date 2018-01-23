CFLAGS := -Wall -Wextra -pedantic -O3
TARGET := unparam
SRC := unparam.cpp
CC := c++

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

install:
	cp $(TARGET) /usr/bin

clean:
	rm -f $(TARGET)
