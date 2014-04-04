UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
CC=g++
LDFLAGS_ADD=-lrt
endif

ifeq ($(UNAME), Darwin)
CC=clang++
LDFLAGS_ADD=
endif

CXX=$(CC)
LD=$(CC)
AR=ar

CFLAGS=-g -fPIC -Wall -I/usr/local/include/ -std=c++11 -stdlib=libc++
CXXFLAGS=$(CFLAGS)
LDFLAGS=$(LDFLAGS_ADD) -L/usr/local/lib -L/usr/local/opt/openssl/lib -lcrypto -lssl -lboost_thread-mt -lboost_system-mt -lboost_date_time-mt -lboost_serialization -L./ -lIRCd

ARFLAGS=-cr

BASEDIR := $(shell pwd)

PLUGINS=Plugin_Quit.plugin Plugin_Nick.plugin Plugin_User.plugin Plugin_MOTD.plugin

IRCD_LIBRARY_OBJECTS=IRC_Client.o IRC_Server.o IRC_User.o IRC_Channel.o IRC_Plugin.o IRC_Plugin_Loader.o IRC_Message.o

all: libIRCd.a $(PLUGINS)

%.o: %.cpp %.h Makefile
	@$(CXX) $(CXXFLAGS) -I./ -I../ -c -o $@ $<

%.o: %.cpp Makefile
	@$(CXX) $(CXXFLAGS) -I./ -I../ -c -o $@ $<

libIRCd.a: $(IRCD_LIBRARY_OBJECTS) Makefile
	@$(AR) $(ARFLAGS) libIRCd.a $(IRCD_LIBRARY_OBJECTS)

%.plugin: %.cpp Makefile
	@$(CXX) $(CXXFLAGS) $(LDFLAGS) -flat_namespace -dynamiclib -o $@ $<

clean:
	@rm -f $(PLUGINS)
	@rm -f $(PLUGINS:.plugin=.o)
	@rm -f libIRCd.a
