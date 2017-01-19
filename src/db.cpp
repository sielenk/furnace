// (C) 2016 Marvin Sielenkemper

#include "db.hpp"

#include "Config.hpp"
#include "db/MySql.hpp"
#include "db/Statement.hpp"
#include "db/ResultSet.hpp"
#include "db/Bindings.hpp"

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

  Impl(Config const& config)
      : mysql(config.dbConfig())
      , insert(mysql, "INSERT INTO serial_log(line) VALUES(?)")
      , query(mysql, "SELECT id, time, line FROM serial_log") {
  }
};


Db::Db(Config const& config) : m_implPtr(new Impl(config)) {
}

Db::~Db() {
}

void Db::addSerialLine(std::string const& line) {
  auto& insert(m_implPtr->insert);
  db::Bindings bindings(line);

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
