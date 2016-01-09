// (C) 2016 Marvin Sielenkemper

#include <boost/noncopyable.hpp>
#include <memory>


class Db : boost::noncopyable {
public:
  Db(char* passwd);
  ~Db();

private:
  struct Impl;

  std::unique_ptr<Impl> const m_implPtr;
};
