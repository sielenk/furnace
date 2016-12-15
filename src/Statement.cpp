// (C) 2016 Marvin Sielenkemper

#include "Statement.hpp"

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
    : m_statementPtr(mysql_stmt_init(mysql))
    , m_bindings()
    , m_buffers() {
  if (mysql_stmt_prepare(*this, statement.data(),
                         statement.length())) {
    std::ostringstream buffer;

    buffer << "failed to prepare statement '" << statement << "': '"
           << mysql_stmt_error(*this) << '\'';

    throw std::runtime_error(buffer.str());
  }

  if (auto const paramCount = mysql_stmt_param_count(*this)) {
    m_bindings.reset(new MYSQL_BIND[paramCount]);
    m_buffers.resize(paramCount);
  }
}


db::Statement::~Statement() {
}


void db::Statement::set(int index, std::string const& param) {
  typedef std::pair<std::string, unsigned long> Buffer;

  auto& anyBuffer(m_buffers.at(index));
  auto& binding(m_bindings[index]);

  anyBuffer = Buffer(param, param.length());

  auto& buffer(boost::any_cast<Buffer&>(anyBuffer));

  binding.buffer_type   = MYSQL_TYPE_STRING;
  binding.buffer        = const_cast<char*>(buffer.first.data());
  binding.buffer_length = buffer.second;
  binding.is_null       = nullptr;
  binding.length        = &buffer.second;
}


db::ResultSet db::Statement::execute() {
  if (mysql_stmt_bind_param(*this, m_bindings.get())) {
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
