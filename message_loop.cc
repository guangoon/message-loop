#include "message_loop.h"

#include <sys/epoll.h>

#include <asm/unistd.h>
#include <error.h>
#include <sys/timerfd.h>

static constexpr int kClockType = CLOCK_MONOTONIC;

MessageLoop::MessageLoop()
    : epoll_fd_(::epoll_create(1)),
      timer_fd_(::timerfd_create(kClockType, TFD_NONBLOCK | TFD_CLOEXEC)),
      running_(false) {
        AddOrRemoveTimerSource(true);
      }
void MessageLoop::Run() {
  running_ = true;

  while (running_) {
    struct epoll_event event = {};

    int epoll_result = epoll_wait(epoll_fd_, &event, 1, -1);

    // Errors are fatal.
    if (event.events & (EPOLLERR | EPOLLHUP)) {
      running_ = false;
      continue;
    }

    // Timeouts are fatal since we specified an infinite timeout already.
    // Likewise, > 1 is not possible since we waited for one result.
    if (epoll_result != 1) {
      running_ = false;
      continue;
    }
  }
}

bool MessageLoop::AddOrRemoveTimerSource(bool add) {
  struct epoll_event event = {};

  event.events = EPOLLIN;
  // The data is just for informational purposes so we know when we were worken
  // by the FD.
  event.data.fd = timer_fd_;

  int ctl_result =
      ::epoll_ctl(epoll_fd_, add ? EPOLL_CTL_ADD : EPOLL_CTL_DEL,
                  timer_fd_, &event);
  return ctl_result == 0;
}

int MessageLoop::timerfd_create(int clockid, int flags) {
  return syscall(__NR_timerfd_create, clockid, flags);
}

void MessageLoop::Terminate() {}
MessageLoop::~MessageLoop() {
  AddOrRemoveTimerSource(false);
}