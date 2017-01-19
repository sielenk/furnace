// (C) 2016 Marvin Sielenkemper

#pragma once

#include "Config.hpp"
#include <boost/noncopyable.hpp>


class ConfigFile : boost::noncopyable {
public:
  ConfigFile(char const* passwd = nullptr);
  ~ConfigFile();

  Config const& config() const;

private:
  class Impl;

  Impl* m_pImpl;
};
