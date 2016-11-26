// (C) 2016 Marvin Sielenkemper

#include "db.hpp"

#include <mysql.h>

#include <boost/any.hpp>
#include <boost/range/iterator_range_core.hpp>

#include <sstream>
#include <vector>


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

    operator MYSQL*() const {
      return m_mysqlPtr;
    }

  private:
    MYSQL* const m_mysqlPtr;
  };


  class Statement;

  class ResultSet : boost::noncopyable {
  public:
    ResultSet(ResultSet&& other)
        : m_statement(other.m_statement), m_resultMetaData(nullptr) {
      std::swap(m_resultMetaData, other.m_resultMetaData);
    }

    ResultSet(Statement const& statement)
        : m_statement(statement), m_resultMetaData(nullptr) {
    }

    ~ResultSet() {
      if (m_resultMetaData) {
        mysql_free_result(m_resultMetaData);
      }
    }


  private:
    Statement const& m_statement;
    MYSQL_RES* m_resultMetaData;

    MYSQL_RES* getResultMetaData();
  };


  class Statement : boost::noncopyable {
  public:
    Statement(MySql& mysql, std::string const& statement)
        : m_statementPtr(mysql_stmt_init(mysql))
        , m_bindings(nullptr)
        , m_buffers() {
      if (mysql_stmt_prepare(m_statementPtr, statement.data(),
                             statement.length())) {
        std::ostringstream buffer;

        buffer << "failed to prepare statement '" << statement
               << "': '" << mysql_stmt_error(m_statementPtr) << '\'';

        throw std::runtime_error(buffer.str());
      }

      if (auto const paramCount =
              mysql_stmt_param_count(m_statementPtr)) {
        m_bindings = new MYSQL_BIND[paramCount];
        m_buffers.resize(paramCount);
      }
    }

    void set(int index, std::string const& param) {
      typedef std::pair<std::string, unsigned long> Buffer;

      auto& anyBuffer(m_buffers.at(index));
      auto& mysqlBinding(m_bindings[index]);

      anyBuffer = Buffer(param, param.length());

      auto& buffer(boost::any_cast<Buffer&>(anyBuffer));

      mysqlBinding.buffer_type = MYSQL_TYPE_STRING;
      mysqlBinding.buffer = const_cast<char*>(buffer.first.data());
      mysqlBinding.buffer_length = buffer.second;
      mysqlBinding.is_null = nullptr;
      mysqlBinding.length = &buffer.second;
    }

    ResultSet execute() {
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

    ~Statement() {
      if (m_statementPtr) {
        mysql_stmt_close(m_statementPtr);
      }

      if (m_bindings) {
        delete[] m_bindings;
      }
    }

    operator MYSQL_STMT*() const {
      return m_statementPtr;
    }

  private:
    MYSQL_STMT* const m_statementPtr;
    MYSQL_BIND* m_bindings;
    std::vector<boost::any> m_buffers;
  };

  MYSQL_RES* ResultSet::getResultMetaData() {
    if (!m_resultMetaData) {
      m_resultMetaData = mysql_stmt_result_metadata(m_statement);

      auto const fieldCount(mysql_num_fields(m_resultMetaData));
      auto const fields(mysql_fetch_fields(m_resultMetaData));

      for (auto const& field :
           boost::make_iterator_range(fields, fields + fieldCount)) {
        (void)field.length;
      }
    }


    return m_resultMetaData;
  }
}


struct Db::Impl : boost::noncopyable {
  MySql mysql;
  Statement insert;
  Statement query;

  Impl(std::string const& passwd)
      : mysql(passwd.c_str())
      , insert(mysql, "INSERT INTO serial_log(line) VALUES(?)")
      , query(mysql, "SELECT id, time, line FROM serial_log") {
  }
};


Db::Db(std::string const& passwd) : m_implPtr(new Impl(passwd)) {
}

Db::~Db() {
}

void Db::addSerialLine(std::string const& line) {
  auto& insert(m_implPtr->insert);

  insert.set(0, line);
  insert.execute();
}

void Db::getLines(
    std::function<void(int, int, std::string const&)> const& callback) {
  auto& query(m_implPtr->query);
  auto const result(query.execute());

}
