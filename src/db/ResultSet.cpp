// (C) 2016 Marvin Sielenkemper

#include "ResultSet.hpp"

#include "Statement.hpp"

#include <mysql.h>

#include <boost/any.hpp>
#include <boost/range/iterator_range_core.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/foreach.hpp>

#include <sstream>
#include <vector>


void db::ResultSet::Deleter::operator()(MYSQL_RES* p) const {
  if (p) {
    mysql_free_result(p);
  }
}

auto db::ResultSet::const_iterator::dereference() const -> reference {
  throw std::runtime_error("dereference() not implemented");
}


void db::ResultSet::const_iterator::increment() {
  throw std::runtime_error("increment() not implemented");
}


bool db::ResultSet::const_iterator::equal(
    const_iterator const& other) const {
  throw std::runtime_error("equal(...) not implemented");
}


db::ResultSet::ResultSet(ResultSet&& other)
    : m_statement(other.m_statement), m_resultMetaData() {
  std::swap(m_resultMetaData, other.m_resultMetaData);
}


db::ResultSet::ResultSet(Statement const& statement)
    : m_statement(statement)
    , m_resultMetaData(mysql_stmt_result_metadata(statement)) {
  if (m_resultMetaData) {
    auto const fieldCount(mysql_num_fields(m_resultMetaData.get()));
    auto const fields(mysql_fetch_fields(m_resultMetaData.get()));

    for (auto const& field :
         boost::make_iterator_range(fields, fields + fieldCount)) {
      (void)field.length;
    }
  }
}


db::ResultSet::~ResultSet() {
}


auto db::ResultSet::begin() const -> const_iterator {
  return const_iterator();
}


auto db::ResultSet::end() const -> const_iterator {
  return const_iterator();
}
