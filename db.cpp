// (C) 2016 Marvin Sielenkemper

#include "db.hpp"

#include <mariadb/mysql.h>
#include <sstream>
#include <iostream>

namespace {
  char const* const db_host("192.168.2.51");
  char const* const db_user("furnace");
  char const* const db_name("furnace");
  unsigned int const db_port(3306);


  class MySql : boost::noncopyable {
  public:
    MySql(char const* passwd) : m_mysqlPtr(mysql_init(nullptr)) {
      if (!mysql_real_connect(m_mysqlPtr, db_host, db_user, passwd,
                              db_name, db_port, nullptr,
                              CLIENT_COMPRESS)) {
        std::ostringstream buffer;

        buffer << "failed to connect to database: '"
               << mysql_error(m_mysqlPtr) << '\'';

        mysql_close(m_mysqlPtr);

        throw std::runtime_error(buffer.str());
      }
    }

    ~MySql() {
      mysql_close(m_mysqlPtr);
    }

    operator MYSQL*() {
      return m_mysqlPtr;
    }

  private:
    MYSQL* const m_mysqlPtr;
  };


  class Statement : boost::noncopyable {
  public:
    Statement(MySql& mysql, std::string const& statement)
        : m_statementPtr(mysql_stmt_init(mysql)) {
      if (!mysql_stmt_prepare(m_statementPtr, statement.data(),
                              statement.length())) {
        std::ostringstream buffer;

        buffer << "failed to prepare statement '" << statement
               << "': '" << mysql_stmt_error(m_statementPtr) << '\'';

        throw std::runtime_error(buffer.str());
      }

      std::cout << statement << std::endl
                << mysql_stmt_param_count(m_statementPtr)
                << std::endl;
    }

    ~Statement() {
      if (m_statementPtr) {
        mysql_stmt_close(m_statementPtr);
      }
    }

  private:
    MYSQL_STMT* const m_statementPtr;
  };
}


struct Db::Impl : boost::noncopyable {
  MySql mysql;
  Statement insert;

  Impl(char const* passwd)
      : mysql(passwd)
      , insert(mysql, "INSERT INTO serial_log(line) VALUES(?)") {
  }
};


Db::Db(char* passwd) : m_implPtr(new Impl(passwd)) {
}

Db::~Db() {
}
