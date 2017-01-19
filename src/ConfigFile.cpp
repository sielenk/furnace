// (C) 2017 Marvin Sielenkemper

#include "ConfigFile.hpp"


//-------------------------------------------------------------------


namespace {
  class DbConfigImpl : public db::Config {
  public:
    DbConfigImpl(std::string const& passwd);
    virtual ~DbConfigImpl();

    virtual std::string dbHost() const override;
    virtual std::string dbUser() const override;
    virtual std::string dbName() const override;
    virtual std::string dbPassword() const override;
    virtual unsigned int dbPort() const override;

  private:
    std::string m_passwd;
  };
}


DbConfigImpl::DbConfigImpl(std::string const& passwd)
    : db::Config(), m_passwd(passwd) {
}


DbConfigImpl::~DbConfigImpl() {
}


std::string DbConfigImpl::dbHost() const {
  return m_passwd.empty() ? "localhost" : "192.168.2.51";
}


std::string DbConfigImpl::dbUser() const {
  return "furnace";
}


std::string DbConfigImpl::dbName() const {
  return "furnace";
}


std::string DbConfigImpl::dbPassword() const {
  return m_passwd;
}


unsigned int DbConfigImpl::dbPort() const {
  return 3306;
}


//-------------------------------------------------------------------


namespace {
  class ConfigImpl : public Config {
  public:
    ConfigImpl(std::string const& passwd);
    virtual ~ConfigImpl();

    virtual db::Config const& dbConfig() const override;

  private:
    DbConfigImpl m_dbConfig;
  };
}


ConfigImpl::ConfigImpl(std::string const& passwd)
    : Config(), m_dbConfig(passwd) {
}


ConfigImpl::~ConfigImpl() {
}


db::Config const& ConfigImpl::dbConfig() const {
  return m_dbConfig;
}


//-------------------------------------------------------------------


class ConfigFile::Impl {
public:
  Impl(std::string const& passwd);
  ~Impl();

  ConfigImpl config;
};


ConfigFile::Impl::Impl(std::string const& passwd) : config(passwd) {
}


ConfigFile::Impl::~Impl() {
}


//-------------------------------------------------------------------


ConfigFile::ConfigFile(std::string const& passwd)
    : boost::noncopyable(), m_pImpl(new Impl(passwd)) {
}


ConfigFile::~ConfigFile() {
  delete m_pImpl;
  m_pImpl = nullptr;
}


Config const& ConfigFile::config() const {
  return m_pImpl->config;
}
