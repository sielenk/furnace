// (C) 2016 Marvin Sielenkemper

#include "serial.hpp"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <signal.h>
#include <unistd.h>

#include <boost/signals2.hpp>


namespace {
  class SignalHandler {
  public:
    // static SignalHandler instance;

    boost::signals2::signal<void()> ioSignal;

  private:
    static void handler(int status) {
      //      instance.ioSignal();
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
      : ttyFd(open("/dev/ttyAMA0",
                   O_RDONLY | O_NOCTTY /*| O_NONBLOCK*/))
      , oldTIo() {
    if (ttyFd < 0) {
      throw std::runtime_error("");
    }

    tcgetattr(ttyFd, &oldTIo);

    //    fcntl(ttyFd, F_SETOWN, getpid());
    //    fcntl(ttyFd, F_SETFL, FASYNC);

    struct termios newTIo = {};

    newTIo.c_cflag = CS8 | CLOCAL | CREAD;
    newTIo.c_iflag = IGNPAR | IGNCR;
    newTIo.c_lflag = ICANON;
    newTIo.c_cc[VMIN] = 1;
    newTIo.c_cc[VTIME] = 0;
    cfsetispeed(&newTIo, B19200);

    tcflush(ttyFd, TCIOFLUSH);
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

void Serial::run() {
  char buffer[512];

  while (true) {
    int const readCount(read(m_implPtr->ttyFd, buffer, 512));

    if (readCount > 0) {
      std::cout << std::string(buffer, readCount - 1) << std::endl;
    }
  }
}
