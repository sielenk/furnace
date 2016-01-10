// (C) 2016 Marvin Sielenkemper

#include "db.hpp"
#include "serial.hpp"


int main(int argc, char** argv) {
  Db db(argv[1]);
  Serial serial;

  db.addSerialLine("Foo!");

  return 0;
}
