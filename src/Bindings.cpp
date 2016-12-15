// (C) 2016 Marvin Sielenkemper

#include "Bindings.hpp"


db::Bindings::Bindings(Tag tag)
    : boost::noncopyable()
    , m_bindings(new MYSQL_BIND[tag.size])
    , m_buffers(tag.size, boost::any()) {
}


db::Bindings::~Bindings() {
}


auto db::Bindings::size() const -> size_type {
  return m_buffers.size();
}


db::Bindings::operator MYSQL_BIND*() const {
  return m_bindings.get();
}


void db::Bindings::set(size_type index,
                       std::string const& parameter) {
  typedef std::pair<std::string, unsigned long> Buffer;

  auto& anyBuffer(m_buffers.at(index));
  auto& binding(m_bindings[index]);

  anyBuffer = Buffer(parameter, parameter.length());

  auto& buffer(boost::any_cast<Buffer&>(anyBuffer));

  binding.buffer_type   = MYSQL_TYPE_STRING;
  binding.buffer        = const_cast<char*>(buffer.first.data());
  binding.buffer_length = buffer.second;
  binding.is_null       = nullptr;
  binding.length        = &buffer.second;
}
