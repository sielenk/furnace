// (C) 2016 Marvin Sielenkemper

#include <signal.h>
#include <unistd.h>
#include <semaphore.h>

#include <boost/optional.hpp>
#include <boost/lockfree/queue.hpp>

#include <iostream>
#include <functional>


namespace {
  class MainLoop {
  public:
    class Control {
    public:
      void exit(int result);

    private:
      friend MainLoop;
      boost::optional<int> m_resultOpt;
      Control();
    };

    typedef std::function<void(Control& control, int fd)> Handler;

    MainLoop();
    ~MainLoop();

    Handler getHandler(int signalId);
    void setHandler(int signalId, Handler const& handler);

    int run();

  private:
    struct SigHandler {
      struct sigaction oldAction;
      Handler handler;
    };

    struct PendingCall {
      Handler const* handlerPtr;
      int fd;
    };

    static MainLoop* m_instancePtr;

    static void globalSignalAction(int signalId, siginfo_t*, void*);

    boost::optional<SigHandler> m_sigHandlers[_NSIG];
    sem_t m_semaphore;
    boost::lockfree::queue<PendingCall> m_pendingCalls;

    void signalAction(siginfo_t const& signalInfo);
  };
}

MainLoop* MainLoop::m_instancePtr;


MainLoop::Control::Control() : m_resultOpt() {
}

void MainLoop::Control::exit(int result) {
  m_resultOpt = result;
}

void MainLoop::globalSignalAction(int, siginfo_t* signalInfoPtr,
                                  void*) {
  m_instancePtr->signalAction(*signalInfoPtr);
}


MainLoop::MainLoop()
    : m_sigHandlers(), m_semaphore(), m_pendingCalls(32) {
  if (m_instancePtr) {
    throw std::runtime_error("MainLoop already instantiated.");
  }

  m_instancePtr = this;

  sem_init(&m_semaphore, 0, 0);

  auto const terminationHandler(

      [](Control& control, int) { control.exit(0); });

  setHandler(SIGINT, terminationHandler);
  setHandler(SIGTERM, terminationHandler);
}


MainLoop::~MainLoop() {
  for (unsigned int i = 0;
       i < sizeof(m_sigHandlers) / sizeof(m_sigHandlers[0]); ++i) {
    setHandler(i, Handler());
  }

  sem_destroy(&m_semaphore);

  m_instancePtr = nullptr;
}


MainLoop::Handler MainLoop::getHandler(int signalId) {
  if (auto const& sigHandlerOpt = m_sigHandlers[signalId]) {
    return sigHandlerOpt->handler;
  } else {
    return Handler();
  }
}


void MainLoop::setHandler(int signalId, Handler const& newHandler) {
  auto& sigHandlerOpt(m_sigHandlers[signalId]);

  if (!m_pendingCalls.empty()) {
    throw std::runtime_error("queue not empty");
  }

  if (newHandler) {
    if (sigHandlerOpt) {
      sigHandlerOpt->handler = newHandler;
    } else {
      sigHandlerOpt = SigHandler{{}, newHandler};

      struct sigaction action = {};
      action.sa_sigaction = globalSignalAction;
      action.sa_flags = SA_SIGINFO;
      sigaction(signalId, &action, &sigHandlerOpt->oldAction);
    }
  } else if (sigHandlerOpt) {
    sigaction(signalId, &sigHandlerOpt->oldAction, nullptr);
    sigHandlerOpt = boost::none;
  }
}


int MainLoop::run() {
  Control control;

  while (!control.m_resultOpt) {
    if (sem_wait(&m_semaphore) == 0) {
      m_pendingCalls.consume_one(
          [&control](PendingCall const& pendingCall) {
            auto& handler(*pendingCall.handlerPtr);

            handler(control, pendingCall.fd);
          });
    } else {
      assert(errno == EINTR);
    }
  }

  return *control.m_resultOpt;
}


void MainLoop::signalAction(siginfo_t const& signalInfo) {
  if (auto const& sigHandlerOpt =
          m_sigHandlers[signalInfo.si_signo]) {
    m_pendingCalls.push(
        PendingCall{&sigHandlerOpt->handler, signalInfo.si_fd});
    sem_post(&m_semaphore);
  }
}


int main(int argc, char* argv[], char* envp[]) {
  bool doRestart(false);

  {
    MainLoop loop;

    loop.setHandler(SIGUSR1,
                    [&doRestart](MainLoop::Control& control, int) {
      doRestart = true;
      control.exit(0);
    });

    loop.run();
  }


  if (doRestart) {
    std::cout << "restarting..." << std::endl;
    execve(argv[0], argv, envp);
  }

  std::cout << "done." << std::endl;

  return 0;
}
