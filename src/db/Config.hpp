// (C) 2016 Marvin Sielenkemper

#pragma once

#include <boost/noncopyable.hpp>
#include <string>


namespace db {
  class Config : boost::noncopyable {
  public:
    virtual std::string dbHost() const = 0;
    virtual std::string dbUser() const = 0;
    virtual std::string dbName() const = 0;
    virtual std::string dbPassword() const = 0;
    virtual unsigned int dbPort() const = 0;

  protected:
    Config();
    ~Config();
  };
}
