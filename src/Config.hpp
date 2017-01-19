// (C) 2016 Marvin Sielenkemper

#pragma once

#include "db/Config.hpp"


class Config : boost::noncopyable {
public:
  virtual db::Config const& dbConfig() const = 0;

protected:
  Config();
  ~Config();
};
