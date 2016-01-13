// (C) 2016 Marvin Sielenkemper

#include "db.hpp"
#include "serial.hpp"

#include <iostream>

#include <unistd.h>


int main(int argc, char** argv) {
  Db db(argv[1]);
  Serial serial;
  boost::signals2::scoped_connection connection(
      serial.lineReceived.connect([&db](std::string const& line) {
        db.addSerialLine(line);
      }));

  for (;;) {
    sleep(1);
  }

  return 0;
}
