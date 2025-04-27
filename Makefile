# Ορισμός μεταβλητών
CC = gcc
CFLAGS = -Wall
TARGET = task4
SRC = /mnt/c/Users/Asus/OneDrive/University/Operating_Systems/Assignment_4/assignemt4.c

# Default στόχος
all: $(TARGET)

# Κανόνας για τη μεταγλώττιση
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Καθαρισμός των παραγόμενων αρχείων
clean:
	rm -f $(TARGET)

