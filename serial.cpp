// (C) 2016 Marvin Sielenkemper

#include "serial.hpp"

struct Serial::Impl : boost::noncopyable {
};


Serial::Serial() : m_implPtr(new Impl()) {
}

Serial::~Serial() {
}
