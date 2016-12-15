// (C) 2016 Marvin Sielenkemper

#pragma once

#include "db.hpp"

#include "MySql.hpp"
#include "Result.hpp"

#include <mysql.h>

#include <boost/any.hpp>
#include <boost/range/iterator_range_core.hpp>
#include <boost/iterator/iterator_facade.hpp>

#include <vector>
#include <memory>


namespace db {
  class Statement;

  class ResultSet : boost::noncopyable {
  public:
    struct const_iterator
        : boost::iterator_facade<const_iterator, Result const,
                                 boost::single_pass_traversal_tag> {
    private:
      friend boost::iterator_core_access;

      reference dereference() const;
      void increment();
      bool equal(const_iterator const& other) const;
    };

    ResultSet(ResultSet&& other);
    ResultSet(Statement const& statement);
    ~ResultSet();

    const_iterator begin() const;

    const_iterator end() const;

  private:
    struct Deleter {
      void operator()(MYSQL_RES*) const;
    };

    typedef std::unique_ptr<MYSQL_RES, Deleter> MySqlResPtr;

    Statement const& m_statement;
    MySqlResPtr m_resultMetaData;
  };
}
