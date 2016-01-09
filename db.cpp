// (C) 2016 Marvin Sielenkemper

#include "db.hpp"

#include <mariadb/mysql.h>
#include <sstream>


namespace
{
  const char* const db_host("192.168.2.51");
  const char* const db_user("furnace");
  const char* const db_name("furnace");
  unsigned int db_port(3306);
}


struct Db::Impl : boost::noncopyable {
  MYSQL mysql;

  Impl() : mysql() {
    mysql_init(&mysql);
  }
};


Db::Db(char* passwd) : m_implPtr(new Impl()) {
  auto& mysql(m_implPtr->mysql);

  if (!mysql_real_connect(&mysql, db_host, db_user, passwd, db_name,
                                  db_port, nullptr, CLIENT_COMPRESS)) {
    std::ostringstream buffer;

    buffer << "failed to connect to database: '"
       << mysql_error(&mysql) << '\'';

    throw std::runtime_error(buffer.str());
  }
}

Db::~Db() {
}
