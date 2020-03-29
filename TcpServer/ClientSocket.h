#pragma once

#ifndef __CLIENT_SOCKET_H__
#define __CLIENT_SOCKET_H__

#include "Socket.h"

class ClientSocket : public Socket
{
public:
   ClientSocket();
   ClientSocket(SOCKET clientSocket);
   ~ClientSocket();
   bool connect(const char* address, const uint16_t port);
   std::string getSocketName(void);
};

#endif //__CLIENT_SOCKET_H__