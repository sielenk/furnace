// (C) 2016 Marvin Sielenkemper

#include "db.hpp"

#include <mariadb/mysql.h>
#include <sstream>


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
        : m_statmentPtr(mysql_stmt_init(mysql)) {
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
  MySql mysql;
  Statement insert;

  Impl(char const* passwd)
      : mysql(passwd)
      , insert(mysql, "insert into foo(bar, baz) values (?, ?)") {
  }
};


Db::Db(char* passwd) : m_implPtr(new Impl(passwd)) {
}

Db::~Db() {
}
