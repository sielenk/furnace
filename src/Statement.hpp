// (C) 2016 Marvin Sielenkemper

#pragma once

#include "MySql.hpp"

#include <boost/any.hpp>

#include <vector>
#include <memory>


namespace db {
  class ResultSet;
  class Bindings;

  class Statement : boost::noncopyable {
  public:
    Statement(MySql& mysql, std::string const& statement);
    ~Statement();

    ResultSet execute();
    ResultSet execute(Bindings& parameterBindings);

    operator MYSQL_STMT*() const;

  private:
    struct Deleter {
      void operator()(MYSQL_STMT*) const;
    };

    typedef std::unique_ptr<MYSQL_STMT, Deleter> StatementPtr;

    StatementPtr const m_statementPtr;
  };
}
