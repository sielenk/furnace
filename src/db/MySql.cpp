// (C) 2016 Marvin Sielenkemper

#include "MySql.hpp"

#include <sstream>
#include <stdexcept>


namespace {
  char const* const db_host("192.168.2.51");
  char const* const db_user("furnace");
  char const* const db_name("furnace");
  unsigned int const db_port(3306);
}


db::MySql::MySql(Config const& config)
    : m_mysqlPtr(mysql_init(nullptr)) {
  if (!mysql_real_connect(m_mysqlPtr, config.dbHost().c_str(),
                          config.dbUser().c_str(),
                          config.dbPassword().c_str(),
                          config.dbName().c_str(), config.dbPort(),
                          nullptr, CLIENT_COMPRESS)) {
    std::ostringstream buffer;

    buffer << "failed to connect to database: '"
           << mysql_error(m_mysqlPtr) << '\'';

    mysql_close(m_mysqlPtr);

    throw std::runtime_error(buffer.str());
  }
}

db::MySql::~MySql() {
  mysql_close(m_mysqlPtr);
}


db::MySql::operator MYSQL*() const {
  return m_mysqlPtr;
}
