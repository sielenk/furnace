// (C) 2016 Marvin Sielenkemper

#include "db.hpp"

#include "MySql.hpp"
#include "Statement.hpp"
#include "ResultSet.hpp"
#include "Bindings.hpp"

#include <mysql.h>

#include <boost/any.hpp>
#include <boost/range/iterator_range_core.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/foreach.hpp>

#include <sstream>
#include <vector>


struct Db::Impl : boost::noncopyable {
  db::MySql mysql;
  db::Statement insert;
  db::Statement query;

  Impl(char const* password)
      : mysql(password)
      , insert(mysql, "INSERT INTO serial_log(line) VALUES(?)")
      , query(mysql, "SELECT id, time, line FROM serial_log") {
  }
};


Db::Db() : m_implPtr(new Impl(nullptr)) {
}

Db::Db(std::string const& password)
    : m_implPtr(new Impl(password.c_str())) {
}

Db::~Db() {
}

void Db::addSerialLine(std::string const& line) {
  auto& insert(m_implPtr->insert);
  db::Bindings bindings(1);

  bindings.set(0, line);
  insert.execute(bindings);
}

void Db::getLines(
    std::function<void(int, int, std::string const&)> const&
        callback) {
  auto& query(m_implPtr->query);
  auto const resultSet(query.execute());

  BOOST_FOREACH (auto const& row, resultSet) {
    callback(row.get<int>("id"), row.get<int>("time"),
             row.get<std::string>("line"));
  }
}
