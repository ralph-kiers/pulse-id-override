LDFLAGS += -shared -ldl
CFLAGS += -fPIC

TARGET := pulse_id_override.so
SOURCES := main.c
OBJECTS := $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET) $(OBJECTS)

.PHONY: all clean
