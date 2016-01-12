// (C) 2016 Marvin Sielenkemper

#include "db.hpp"
#include "serial.hpp"

#include <iostream>


int main(int argc, char** argv) {
  Db db(argv[1]);
  Serial serial;

  db.addSerialLine("Foo!");



  char c(0);
  while (c != 'q') {
    std::cin >> c;
  }

  return 0;
}
