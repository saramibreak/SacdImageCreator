TARGET := SacdImageCreator
TOOLCHAIN := /toolchain-binary
CC := $(TOOLCHAIN)/bin/armv7a-mediatek451_001_vfp-linux-gnueabi-gcc

INCLUDE := -I.
CFLAGS := -std=gnu99 -march=armv6k

ifeq ($(DEBUG), 1)
	CFLAGS += -O0 -g -DDEBUG
else
	CFLAGS += -O3 -funroll-loops -Wall -Wextra -Wno-unknown-pragmas
endif

SOURCES_C := \
 main.o \
 filesystem.o \
 sacd_read_ioctl.o \

OBJECTS := $(SOURCES_C:.c=.o)

all: $(TARGET)
$(TARGET): $(OBJECTS)
	$(CC) -o $@ $(OBJECTS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS) $(INCLUDE)

clean-objs:
	rm -f $(OBJECTS)

clean:
	rm -f $(OBJECTS)
	rm -f $(TARGET)

.PHONY: clean clean-objs
