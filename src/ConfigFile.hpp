// (C) 2016 Marvin Sielenkemper

#pragma once

#include "Config.hpp"
#include <boost/noncopyable.hpp>


class ConfigFile : boost::noncopyable {
public:
  ConfigFile(std::string const& passwd = std::string());
  ~ConfigFile();

  Config const& config() const;

private:
  class Impl;

  Impl* m_pImpl;
};
