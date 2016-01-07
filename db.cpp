// (C) 2016 Marvin Sielenkemper

#include "db.hpp"

#include <mariadb/mysql.h>
#include <iostream>


struct Db::Impl : boost::noncopyable {
  MYSQL mysql;

  Impl()
    : mysql() {
    mysql_init(&mysql);
  }
};


Db::Db(char* passwd) : m_implPtr(new Impl()) {
  auto& mysql(m_implPtr->mysql);
  auto const p(mysql_real_connect(&mysql, "192.168.2.51",
                                  "furnace", passwd, "furnace", 3306,
                                  nullptr, CLIENT_COMPRESS));

  std::cout << p << " " << &mysql << std::endl
            << mysql_errno(&mysql) << std::endl
            << mysql_error(&mysql) << std::endl;
}

Db::~Db() {
}
