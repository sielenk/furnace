// (C) 2016 Marvin Sielenkemper

#include <boost/signals2.hpp>
#include <boost/noncopyable.hpp>
#include <string>
#include <memory>


class Serial : boost::noncopyable {
public:
  typedef boost::signals2::signal<void(std::string const&)>
      LineReceived;

  Serial();
  ~Serial();

  LineReceived lineReceived;

private:
  struct Impl;

  std::unique_ptr<Impl> m_implPtr;
};
