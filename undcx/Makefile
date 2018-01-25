CFLAGS := -Wall -Wextra -pedantic -O3
LIB := -lz
TARGET := undcx
SRC := undcx.cpp
CC := c++

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(LIB) $(CFLAGS) $(SRC) -o $(TARGET)

install:
	cp $(TARGET) /usr/bin

clean:
	rm -f $(TARGET)
