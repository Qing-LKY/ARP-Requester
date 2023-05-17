export CC := gcc
export INCLUDE := include
export CFLAGS := -g -O2 -Wall
export LDFLAGS := -lpthread
export TARGET := arp-send

export SHELL := /bin/sh

all:
	$(MAKE) -C src

.phony: clean static

static:
	$(MAKE) -C src static

clean:
	$(MAKE) -C src clean
	$(RM) $(TARGET) $(TARGET)-static