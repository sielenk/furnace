AM_CXXFLAGS = @MYSQL_CXXFLAGS@ @BOOST_CPPFLAGS@ -std=c++11 -Wall -Werror
AM_LDFLAGS = @MYSQL_LDFLAGS@ @BOOST_LDFLAGS@

LDADD = @MYSQL_LIBS@

bin_PROGRAMS = furnace translate
furnace_SOURCES = main.cpp db.cpp serial.cpp
translate_SOURCES = translate.cpp db.cpp
