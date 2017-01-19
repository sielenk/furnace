SUBDIRS = db

AM_CXXFLAGS = $(MYSQL_CXXFLAGS) $(LIBCONFIGXX_CFLAGS) $(BOOST_CPPFLAGS) -std=c++11 -Wall -Werror
AM_LDFLAGS = $(MYSQL_LDFLAGS)

LDADD = $(MYSQL_LIBS) $(LIBCONFIGXX_LIBS) db/libdb.a

bin_PROGRAMS = furnace translate

furnace_SOURCES = Config.cpp ConfigFile.cpp main.cpp db.cpp serial.cpp
translate_SOURCES = Config.cpp ConfigFile.cpp translate.cpp db.cpp
