// (C) 2016 Marvin Sielenkemper

#pragma once

#include "MySql.hpp"

#include <boost/any.hpp>

#include <vector>
#include <memory>


namespace db {
  class Bindings : boost::noncopyable {
  public:
    typedef unsigned int size_type;

    Bindings();
    Bindings(size_type size);
    ~Bindings();

    size_type size() const;

    void set(size_type index, std::string const& param);

    operator MYSQL_BIND*() const;

  private:
    typedef std::unique_ptr<MYSQL_BIND[]> BindingsPtr;
    typedef std::vector<boost::any> Buffers;

    BindingsPtr m_bindings;
    Buffers m_buffers;
  };
}
