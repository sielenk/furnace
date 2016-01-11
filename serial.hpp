// (C) 2016 Marvin Sielenkemper

#include <boost/noncopyable.hpp>
#include <memory>


class Serial : boost::noncopyable {
public:
  Serial();
  ~Serial();

  void run();

private:
  struct Impl;

  std::unique_ptr<Impl> m_implPtr;
};
