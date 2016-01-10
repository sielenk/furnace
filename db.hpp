// (C) 2016 Marvin Sielenkemper

#include <boost/noncopyable.hpp>
#include <memory>


class Db : boost::noncopyable {
public:
  Db(char* passwd);
  ~Db();

  void addSerialLine(std::string const& line);

private:
  struct Impl;

  std::unique_ptr<Impl> const m_implPtr;
};
