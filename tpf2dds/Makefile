CFLAGS := -Wall -Wextra -O3
TARGET := tpf2dds
SRC := tpf2dds.cpp
CC := c++

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

install:
	cp $(TARGET) /usr/bin

clean:
	rm -f $(TARGET)
