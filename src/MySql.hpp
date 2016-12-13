// (C) 2016 Marvin Sielenkemper

#include <mysql.h>

#include <boost/noncopyable.hpp>


namespace db {
  class MySql : boost::noncopyable {
  public:
    MySql(char const* passwd);
    ~MySql();

    operator MYSQL*() const;

  private:
    MYSQL* const m_mysqlPtr;
  };
}
