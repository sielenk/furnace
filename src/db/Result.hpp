// (C) 2016 Marvin Sielenkemper

#pragma once

#include <string>


namespace db {
  class Result {
  public:
    template <class T>
    T get(std::string const& columnName) const {
      return T();
    }
  };
}
