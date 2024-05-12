CC = gcc
CFLAGS = -Wall -Wextra -g -shared
TARGET = build/clic/clic.dll
SOURCES = src/util/*.c\
		src/token/*.c \
	 	src/*.c \

OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)

.PHONY: all clean