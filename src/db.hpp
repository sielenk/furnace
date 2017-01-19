// (C) 2016 Marvin Sielenkemper

#pragma once

#include "Config.hpp"

#include <boost/noncopyable.hpp>
#include <memory>
#include <string>
#include <functional>


class Db : boost::noncopyable {
public:
  Db(Config const& config);
  ~Db();

  void addSerialLine(std::string const& line);
  void getLines(std::function<
                void(int, int, std::string const&)> const& callback);

private:
  struct Impl;

  std::unique_ptr<Impl> const m_implPtr;
};
