#ifndef _ECHO_EVENT_H
#define _ECHO_EVENT_H
namespace echo {
  enum class EchoEvent {
    STREAM,
    READ,
    SENT,
    SEND_ERR
  };
}


#endif
