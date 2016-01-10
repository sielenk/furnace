// (C) 2016 Marvin Sielenkemper

#include "serial.hpp"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <signal.h>

#include <boost/signals2.hpp>


namespace {
  class SignalHandler {
  public:
    static SignalHandler instance;

    boost::signals2::signal<void()> ioSignal;

  private:
    static void handler(int status) {
      instance.ioSignal();
    }


    struct sigaction m_oldAction;


    SignalHandler() {
      struct sigaction newAction = {};

      newAction.sa_handler = handler;

      sigaction(SIGIO, &newAction, &m_oldAction);
    }

    ~SignalHandler() {
      sigaction(SIGIO, &m_oldAction, nullptr);
    }
  };
}

struct Serial::Impl : boost::noncopyable {
  int ttyFd;
  struct termios oldTIo;

  Impl()
      : ttyFd(open("/dev/ttyAMA0", O_RDONLY | O_NOCTTY | O_NONBLOCK))
      , oldTIo() {
    if (ttyFd < 0) {
      throw std::runtime_error("");
    }

    tcgetattr(ttyFd, &oldTIo);

    fcntl(ttyFd, F_SETOWN, getpid());
    fcntl(ttyFd, F_SETFL, FASYNC);

    struct termios newTIo = { };

    newTIo.c_cflag = B19200 | CS8 | CLOCAL | CREAD;
    newTIo.c_iflag = IGNPAR | ICRNL;
    newTIo.c_oflag = 0;
    newTIo.c_lflag = ICANON;
    newTIo.c_cc[VMIN]=1;
    newTIo.c_cc[VTIME]=0;
    tcflush(ttyFd, TCIFLUSH);
    tcsetattr(ttyFd, TCSANOW, &newTIo);
  }

  ~Impl() {
    tcsetattr(ttyFd, TCSANOW, &oldTIo);
    close(ttyFd);
  }
};


Serial::Serial() : m_implPtr(new Impl()) {
}

Serial::~Serial() {
}
