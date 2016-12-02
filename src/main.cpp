// (C) 2016 Marvin Sielenkemper

#include "db.hpp"
#include "serial.hpp"

#include <memory>
#include <iostream>

#include <unistd.h>


int main(int argc, char** argv) {
  std::string passwd;

  getline(std::cin, passwd);

  std::unique_ptr<Db> dbPtr;
  Serial serial;
  boost::signals2::scoped_connection connection(
      serial.lineReceived.connect(
          [&dbPtr, &passwd](std::string const& line) {
            try {
              if (!dbPtr) {
                dbPtr.reset(new Db(passwd));
              }

              auto& db(*dbPtr);

              db.addSerialLine(line);
            } catch (...) {
              dbPtr.reset();
            }
          }));

  for (;;) {
    sleep(1);
  }

  return 0;
}
