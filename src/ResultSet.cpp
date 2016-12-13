// (C) 2016 Marvin Sielenkemper

#include "ResultSet.hpp"

#include <mysql.h>

#include <boost/any.hpp>
#include <boost/range/iterator_range_core.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/foreach.hpp>

#include <sstream>
#include <vector>


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
    : m_statement(other.m_statement), m_resultMetaData(nullptr) {
  std::swap(m_resultMetaData, other.m_resultMetaData);
}


db::ResultSet::ResultSet(Statement const& statement)
    : m_statement(statement), m_resultMetaData(nullptr) {
}


db::ResultSet::~ResultSet() {
  if (m_resultMetaData) {
    mysql_free_result(m_resultMetaData);
  }
}


auto db::ResultSet::begin() const -> const_iterator {
  return const_iterator();
}


auto db::ResultSet::end() const -> const_iterator {
  return const_iterator();
}
