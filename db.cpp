// (C) 2016 Marvin Sielenkemper

#include "db.hpp"

#include <mariadb/mysql.h>
#include <sstream>


namespace {
  char const* const db_host("192.168.2.51");
  char const* const db_user("furnace");
  char const* const db_name("furnace");
  unsigned int const db_port(3306);


  class Statement : boost::noncopyable {
  public:
    Statement(MYSQL& mysql, std::string const& statement)
        : m_statmentPtr(mysql_stmt_init(&mysql)) {
      mysql_stmt_prepare(m_statmentPtr, statement.data(),
                         statement.length());
    }

    ~Statement() {
      if (m_statmentPtr) {
        mysql_stmt_close(m_statmentPtr);
      }
    }

  private:
    MYSQL_STMT* const m_statmentPtr;
  };
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
