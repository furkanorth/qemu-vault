CC = gcc
CFLAGS = -Wall -Wextra -O2

TARGET = qemu-vault
SOURCE = qemu-vault.c

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) $(SOURCE) -o $(TARGET)

clean:
	rm -f $(TARGET)
