// (C) 2016 Marvin Sielenkemper

#include <mariadb/mysql.h>
#include <boost/noncopyable.hpp>

class Db : boost::noncopyable {
 public:
  Db(char* passwd);
  ~Db();

 private:
  MYSQL* m_mysqlPtr;
};
