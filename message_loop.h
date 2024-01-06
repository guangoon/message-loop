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
  int epoll_fd_;
  bool running_;
};

#endif