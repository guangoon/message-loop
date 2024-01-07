#ifndef MESSAGE_LOOP_H_
#define MESSAGE_LOOP_H_
#include <unistd.h>

class MessageLoop {
 public:
  MessageLoop();
  ~MessageLoop();
  void Run();
  void Terminate();
 private:
  bool AddOrRemoveTimerSource(bool add);
  int timerfd_create(int clockid, int flags);
  int epoll_fd_;
  int timer_fd_;
  bool running_;
};

#endif