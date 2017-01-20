// (C) 2016 Marvin Sielenkemper

#pragma once

#include "Config.hpp"
#include <boost/noncopyable.hpp>
#include <memory>


class ConfigFile : boost::noncopyable {
public:
  ConfigFile(std::string const& configPath);
  ~ConfigFile();

  Config const& config() const;

private:
  class Impl;

  std::unique_ptr<Impl> m_pImpl;
};
