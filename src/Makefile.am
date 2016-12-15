AM_CXXFLAGS = $(MYSQL_CXXFLAGS) $(BOOST_CPPFLAGS) -std=c++11 -Wall -Werror
AM_LDFLAGS = $(MYSQL_LDFLAGS)

LDADD = $(MYSQL_LIBS) libdb.a

lib_LIBRARIES = libdb.a
bin_PROGRAMS = furnace translate

libdb_a_SOURCES = MySql.cpp Result.cpp ResultSet.cpp Statement.cpp Bindings.cpp
furnace_SOURCES = main.cpp db.cpp serial.cpp
translate_SOURCES = translate.cpp db.cpp
