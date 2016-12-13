// (C) 2016 Marvin Sielenkemper

#pragma once

#include "MySql.hpp"

#include <boost/any.hpp>
#include <vector>


namespace db {
  class ResultSet;

  class Statement : boost::noncopyable {
  public:
    Statement(MySql& mysql, std::string const& statement);
    ~Statement();

    void set(int index, std::string const& param);

    ResultSet execute();

    operator MYSQL_STMT*() const;

  private:
    MYSQL_STMT* const m_statementPtr;
    MYSQL_BIND* m_bindings;
    std::vector<boost::any> m_buffers;
  };
}
