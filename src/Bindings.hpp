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

    template <class... P>
    Bindings(P... p)
        : Bindings(Tag{0}, p...) {
    }

    ~Bindings();

    size_type size() const;

    operator MYSQL_BIND*() const;

  private:
    struct Tag {
      size_type size;
    };

    typedef std::unique_ptr<MYSQL_BIND[]> BindingsPtr;
    typedef std::vector<boost::any> Buffers;

    BindingsPtr m_bindings;
    Buffers m_buffers;

    Bindings(Tag);

    template <class P, class... Ps>
    Bindings(Tag tag, P p, Ps... ps)
        : Bindings(Tag{tag.size + 1}, ps...) {
      set(tag.size, p);
    }

    void set(size_type index, std::string const& param);
  };
}
