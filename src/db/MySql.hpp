// (C) 2016 Marvin Sielenkemper

#pragma once

#include "Config.hpp"

#include <mysql.h>

#include <boost/noncopyable.hpp>


namespace db {
  class MySql : boost::noncopyable {
  public:
    MySql(Config const& config);
    ~MySql();

    operator MYSQL*() const;

  private:
    MYSQL* const m_mysqlPtr;
  };
}
