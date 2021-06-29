
CC ?= gcc
CXX ?= g++
CFLAGS ?= -O2 -Wall
LDFLAGS += -Wl,-Bsymbolic-functions
CFLAGS32 = $(CFLAGS) -m32

PKG_JACK:=$(shell pkg-config --cflags --libs jack)
PKG_QTCORE:=$(shell pkg-config --cflags --libs Qt5Core)


all: build

build:
	# Library
	$(CXX) libaam.cpp -o libaam.so -shared -fPIC $(CFLAGS32) $(LDFLAGS) $(PKG_JACK) $(PKG_QTCORE)

	# Transport utility
	$(CC) xt2-trans2midi.c -o xt2-trans2midi $(CFLAGS) $(LDFLAGS) $(PKG_JACK)

	# Config GUI
	make -C config

clean:
	rm -f libaam.so xt2-trans2midi config/xt2-config
	make clean -C config

distclean: clean
