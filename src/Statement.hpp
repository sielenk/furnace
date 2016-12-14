// (C) 2016 Marvin Sielenkemper

#pragma once

#include "MySql.hpp"

#include <boost/any.hpp>

#include <vector>
#include <memory>


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
    struct Deleter {
      void operator()(MYSQL_STMT*) const;
    };

    typedef std::unique_ptr<MYSQL_STMT, Deleter> StatementPtr;
    typedef std::unique_ptr<MYSQL_BIND[]> BindingsPtr;
    typedef std::vector<boost::any> Buffers;

    StatementPtr const m_statementPtr;
    BindingsPtr m_bindings;
    Buffers m_buffers;
  };
}
