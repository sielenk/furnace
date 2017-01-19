// (C) 2017 Marvin Sielenkemper

#include "ConfigFile.hpp"


//-------------------------------------------------------------------


namespace {
  class DbConfigImpl : public db::Config {
  public:
    DbConfigImpl(char const* passwd);
    virtual ~DbConfigImpl();

    virtual std::string dbHost() const override;
    virtual std::string dbUser() const override;
    virtual std::string dbName() const override;
    virtual std::string dbPassword() const override;
    virtual unsigned int dbPort() const override;
    
    private:
    char const* m_passwd;
  };
}


DbConfigImpl::DbConfigImpl(char const* passwd) : db::Config(), m_passwd(passwd) {
}


DbConfigImpl::~DbConfigImpl() {
}


std::string DbConfigImpl::dbHost() const {
  return "localhost";
}


std::string DbConfigImpl::dbUser() const {
  return "furnace";
}


std::string DbConfigImpl::dbName() const {
  return "furnace";
}


std::string DbConfigImpl::dbPassword() const {
  return m_passwd ? m_passwd : "";
}


unsigned int DbConfigImpl::dbPort() const {
  return 3306;
}


//-------------------------------------------------------------------


namespace {
  class ConfigImpl : public Config {
  public:
    ConfigImpl(char const* passwd);
    virtual ~ConfigImpl();

    virtual db::Config const& dbConfig() const override;

  private:
    DbConfigImpl m_dbConfig;
  };
}


ConfigImpl::ConfigImpl(char const* passwd) : Config(), m_dbConfig(passwd) {
}


ConfigImpl::~ConfigImpl() {
}


db::Config const& ConfigImpl::dbConfig() const {
  return m_dbConfig;
}


//-------------------------------------------------------------------


class ConfigFile::Impl {
public:
  Impl(char const* passwd);
  ~Impl();

  ConfigImpl config;
};


ConfigFile::Impl::Impl(char const* passwd) : config(passwd) {
}


ConfigFile::Impl::~Impl() {
}


//-------------------------------------------------------------------


ConfigFile::ConfigFile(char const* passwd) : boost::noncopyable(), m_pImpl(new Impl(passwd)) {
}


ConfigFile::~ConfigFile() {
  delete m_pImpl;
  m_pImpl = nullptr;
}


Config const& ConfigFile::config() const {
  return m_pImpl->config;
}
