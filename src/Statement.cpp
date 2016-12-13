// (C) 2016 Marvin Sielenkemper

#include "Statement.hpp"

#include "ResultSet.hpp"

#include <mysql.h>

#include <sstream>
#include <stdexcept>


db::Statement::Statement(MySql& mysql, std::string const& statement)
    : m_statementPtr(mysql_stmt_init(mysql))
    , m_bindings(nullptr)
    , m_buffers() {
  if (mysql_stmt_prepare(m_statementPtr, statement.data(),
                         statement.length())) {
    std::ostringstream buffer;

    buffer << "failed to prepare statement '" << statement << "': '"
           << mysql_stmt_error(m_statementPtr) << '\'';

    throw std::runtime_error(buffer.str());
  }

  if (auto const paramCount =
          mysql_stmt_param_count(m_statementPtr)) {
    m_bindings = new MYSQL_BIND[paramCount];
    m_buffers.resize(paramCount);
  }
}


db::Statement::~Statement() {
  if (m_statementPtr) {
    mysql_stmt_close(m_statementPtr);
  }

  if (m_bindings) {
    delete[] m_bindings;
  }
}


void db::Statement::set(int index, std::string const& param) {
  typedef std::pair<std::string, unsigned long> Buffer;

  auto& anyBuffer(m_buffers.at(index));
  auto& mysqlBinding(m_bindings[index]);

  anyBuffer = Buffer(param, param.length());

  auto& buffer(boost::any_cast<Buffer&>(anyBuffer));

  mysqlBinding.buffer_type   = MYSQL_TYPE_STRING;
  mysqlBinding.buffer        = const_cast<char*>(buffer.first.data());
  mysqlBinding.buffer_length = buffer.second;
  mysqlBinding.is_null       = nullptr;
  mysqlBinding.length        = &buffer.second;
}


db::ResultSet db::Statement::execute() {
  if (mysql_stmt_bind_param(m_statementPtr, m_bindings)) {
    std::ostringstream buffer;

    buffer << "failed to bind parameters: '"
           << mysql_stmt_error(m_statementPtr) << '\'';

    throw std::runtime_error(buffer.str());
  }

  if (mysql_stmt_execute(m_statementPtr)) {
    std::ostringstream buffer;

    buffer << "failed to execute prepared statement: '"
           << mysql_stmt_error(m_statementPtr) << '\'';

    throw std::runtime_error(buffer.str());
  }

  return ResultSet(*this);
}


db::Statement::operator MYSQL_STMT*() const {
  return m_statementPtr;
}
