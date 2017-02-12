// (C) 2016 Marvin Sielenkemper

#include "serial.hpp"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <signal.h>
#include <unistd.h>

namespace sig = boost::signals2;


namespace {
  class SignalHandler {
  public:
    static SignalHandler instance;

    sig::signal<void()> ioSignal;

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

SignalHandler SignalHandler::instance;


struct Serial::Impl : boost::noncopyable {
  int ttyFd;
  struct termios oldTIo;
  sig::scoped_connection sigConnection;
  LineReceived& lineReceived;

  Impl(LineReceived& lineReceived)
      : ttyFd(open("/dev/ttyAMA0", O_RDONLY | O_NOCTTY | O_NONBLOCK))
      , oldTIo()
      , sigConnection(SignalHandler::instance.ioSignal.connect(
            [this]() { handleIo(); }))
      , lineReceived(lineReceived) {
    if (ttyFd < 0) {
      throw std::runtime_error("failed to open /dev/ttyAMA0");
    }

    tcgetattr(ttyFd, &oldTIo);

    fcntl(ttyFd, F_SETOWN, getpid());
    fcntl(ttyFd, F_SETFL, FASYNC);

    struct termios newTIo = {};

    newTIo.c_cflag     = CS8 | CLOCAL | CREAD;
    newTIo.c_iflag     = IGNPAR | IGNCR;
    newTIo.c_lflag     = ICANON;
    newTIo.c_cc[VMIN]  = 1;
    newTIo.c_cc[VTIME] = 0;
    cfsetispeed(&newTIo, B19200);

    tcflush(ttyFd, TCIOFLUSH);
    tcsetattr(ttyFd, TCSANOW, &newTIo);
  }

  ~Impl() {
    tcsetattr(ttyFd, TCSANOW, &oldTIo);
    close(ttyFd);
  }

  void handleIo() {
    std::string line;
    char buffer[256];

    for (;;) {
      int const readCount(
          read(ttyFd, buffer, sizeof(buffer) / sizeof(buffer[0])));

      if (readCount <= 0) {
        break;
      }

      auto const isLast(buffer[readCount - 1] == '\n');
      std::string const fragment(buffer, readCount - isLast);

      line += fragment;

      if (isLast) {
        break;
      }
    }

    lineReceived(line);
  }
};


Serial::Serial() : lineReceived(), m_implPtr(new Impl(lineReceived)) {
}

Serial::~Serial() {
}
