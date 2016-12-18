// (C) 2016 Marvin Sielenkemper

#include "Statement.hpp"

#include "Bindings.hpp"
#include "ResultSet.hpp"

#include <mysql.h>

#include <boost/range/counting_range.hpp>

#include <sstream>
#include <stdexcept>


void db::Statement::Deleter::operator()(MYSQL_STMT* p) const {
  if (p) {
    mysql_stmt_close(p);
  }
};


db::Statement::Statement(MySql& mysql, std::string const& statement)
    : m_statementPtr(mysql_stmt_init(mysql)) {
  if (mysql_stmt_prepare(*this, statement.data(),
                         statement.length())) {
    std::ostringstream buffer;

    buffer << "failed to prepare statement '" << statement << "': '"
           << mysql_stmt_error(*this) << '\'';

    throw std::runtime_error(buffer.str());
  }
}


db::Statement::~Statement() {
}


db::ResultSet db::Statement::execute() {
  Bindings bindings;

  return execute(bindings);
}


db::ResultSet db::Statement::execute(Bindings& parameterBindings) {
  auto const paramCount(mysql_stmt_param_count(*this));
  auto const bindingCount(parameterBindings.size());

  if (paramCount > bindingCount) {
    std::ostringstream buffer;

    buffer << "not enough bindings: expected " << paramCount
           << ", got only " << bindingCount;

    throw std::runtime_error(buffer.str());
  }

  if (mysql_stmt_bind_param(*this, parameterBindings)) {
    std::ostringstream buffer;

    buffer << "failed to bind parameters: '"
           << mysql_stmt_error(*this) << '\'';

    throw std::runtime_error(buffer.str());
  }

  if (mysql_stmt_execute(*this)) {
    std::ostringstream buffer;

    buffer << "failed to execute prepared statement: '"
           << mysql_stmt_error(*this) << '\'';

    throw std::runtime_error(buffer.str());
  }

  return ResultSet(*this);
}


db::Statement::operator MYSQL_STMT*() const {
  return m_statementPtr.get();
}
