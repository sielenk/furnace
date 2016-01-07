// (C) 2016 Marvin Sielenkemper

#include "db.hpp"

#include <iostream>

Db::Db(char* passwd) : m_mysqlPtr(mysql_init(nullptr)) {
  auto const p(mysql_real_connect(m_mysqlPtr, "192.168.2.51", "furnace", passwd,
                                  "furnace", 3306, nullptr, CLIENT_COMPRESS));

  std::cout << p << " " << m_mysqlPtr << std::endl << mysql_errno(m_mysqlPtr)
            << std::endl << mysql_error(m_mysqlPtr) << std::endl;
}

Db::~Db() {
  mysql_close(m_mysqlPtr);
}
