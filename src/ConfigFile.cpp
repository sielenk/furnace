// (C) 2017 Marvin Sielenkemper

#include "ConfigFile.hpp"

#include "libconfig.h++"

namespace lc = libconfig;

//-------------------------------------------------------------------


namespace {
  class DbConfigImpl : public db::Config {
  public:
    DbConfigImpl(lc::Setting const& setting);
    virtual ~DbConfigImpl();

    virtual std::string dbHost() const override;
    virtual std::string dbUser() const override;
    virtual std::string dbName() const override;
    virtual std::string dbPassword() const override;
    virtual unsigned int dbPort() const override;

  private:
    lc::Setting const& m_setting;
  };
}


DbConfigImpl::DbConfigImpl(lc::Setting const& setting)
    : db::Config(), m_setting(setting) {
}


DbConfigImpl::~DbConfigImpl() {
}


std::string DbConfigImpl::dbHost() const {
  std::string host("localhost");
  m_setting.lookupValue("host", host);
  return host;
}


std::string DbConfigImpl::dbUser() const {
  std::string user("furnace");
  m_setting.lookupValue("user", user);
  return user;
}


std::string DbConfigImpl::dbName() const {
  std::string name("furnace");
  m_setting.lookupValue("name", name);
  return name;
}


std::string DbConfigImpl::dbPassword() const {
  std::string password;
  m_setting.lookupValue("password", password);
  return password;
}


unsigned int DbConfigImpl::dbPort() const {
  unsigned int port(3306);
  m_setting.lookupValue("port", port);
  return port;
}


//-------------------------------------------------------------------


namespace {
  class ConfigImpl : public Config {
  public:
    ConfigImpl(lc::Setting const& setting);
    virtual ~ConfigImpl();

    virtual db::Config const& dbConfig() const override;

  private:
    DbConfigImpl m_dbConfig;
  };
}


ConfigImpl::ConfigImpl(lc::Setting const& setting)
    : Config(), m_dbConfig(setting["db"]) {
}


ConfigImpl::~ConfigImpl() {
}


db::Config const& ConfigImpl::dbConfig() const {
  return m_dbConfig;
}


//-------------------------------------------------------------------


class ConfigFile::Impl {
  static lc::Config& init(lc::Config& config,
                          std::string const& configPath);

  lc::Config m_config;

public:
  Impl(std::string const& configPath);
  ~Impl();

  ConfigImpl config;
};


lc::Config& ConfigFile::Impl::init(lc::Config& config,
                                   std::string const& configPath) {
  config.readFile(configPath.c_str());

  return config;
}


ConfigFile::Impl::Impl(std::string const& configPath)
    : m_config(), config(init(m_config, configPath).getRoot()) {
}


ConfigFile::Impl::~Impl() {
}


//-------------------------------------------------------------------


ConfigFile::ConfigFile(std::string const& configPath)
    : boost::noncopyable(), m_pImpl(new Impl(configPath)) {
}


ConfigFile::~ConfigFile() {
}


Config const& ConfigFile::config() const {
  return m_pImpl->config;
}
