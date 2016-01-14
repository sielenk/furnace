// (C) 2016 Marvin Sielenkemper

#include <boost/noncopyable.hpp>
#include <memory>
#include <string>


class Db : boost::noncopyable {
public:
  Db(std::string const& passwd);
  ~Db();

  void addSerialLine(std::string const& line);

private:
  struct Impl;

  std::unique_ptr<Impl> const m_implPtr;
};
