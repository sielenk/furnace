// (C) 2016 Marvin Sielenkemper

#include "Statement.hpp"

#include "ResultSet.hpp"

#include <mysql.h>

#include <boost/range/counting_range.hpp>

#include <sstream>
#include <stdexcept>


namespace {
  struct MySqlResultDeleter {
    void operator()(MYSQL_RES* p) const {
      mysql_free_result(p);
    }
  };
}

typedef std::unique_ptr<MYSQL_RES, MySqlResultDeleter> MySqlResult;


void db::Statement::Deleter::operator()(MYSQL_STMT* p) const {
  mysql_stmt_close(p);
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

  if (auto const paramMetaData =
          MySqlResult(mysql_stmt_param_metadata(*this))) {
    auto const fieldCount(mysql_num_fields(paramMetaData.get()));
    auto const fields(mysql_fetch_fields(paramMetaData.get()));

    m_bindings.reset(new MYSQL_BIND[fieldCount]);
    m_buffers.resize(fieldCount);

    for (auto const i : boost::counting_range(0U, fieldCount)) {
      auto const& field(fields[i]);
      auto& binding(m_bindings[i]);

      binding.buffer_type = field.type;
    }
  }
}


db::Statement::~Statement() {
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

  if (auto const resultMetaData =
          MySqlResult(mysql_stmt_result_metadata(*this))) {
    auto const fieldCount(mysql_num_fields(resultMetaData.get()));
    auto const fields(mysql_fetch_fields(resultMetaData.get()));

    for (auto const& field :
         boost::make_iterator_range(fields, fields + fieldCount)) {
      (void)field.length;
    }
  }

  return ResultSet(*this);
}


db::Statement::operator MYSQL_STMT*() const {
  return m_statementPtr.get();
}
