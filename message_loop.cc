#include "message_loop.h"

#include <sys/epoll.h>
#include <error.h>

MessageLoop::MessageLoop() : epoll_fd_(::epoll_create(1)), running_(false) {}
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

void MessageLoop::Terminate() {}
MessageLoop::~MessageLoop() {}