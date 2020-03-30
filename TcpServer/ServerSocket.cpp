#include "ServerSocket.h"

ServerSocket::ServerSocket() : Socket()
{

}

ServerSocket::~ServerSocket()
{

}

bool ServerSocket::bind(const char* address, const uint16_t port)
{
   // locals
   bool rV = true;

   this->fillIpProtocolFamily();
   this->fillPort(port);
   rV = this->fillNetworkAddressStructure(address);

   if (true == rV &&
      ::bind(this->socketId, (sockaddr*)this->socketAddr.get(), sizeof(*this->socketAddr)) == SOCKET_ERROR)
   {
      rV = false;
   }

   return rV;
}

bool ServerSocket::listen(const int backlog)
{
   // locals
   bool rV = true;

   if (::listen(this->socketId, backlog) == SOCKET_ERROR)
   {
      rV = false;
   }

   return rV;
}

ClientSocket* ServerSocket::accept(void)
{
   // locals
   ClientSocket* clientSocket = nullptr;
   std::unique_ptr<sockaddr_in> remote = std::make_unique<sockaddr_in>();
   std::unique_ptr<sockaddr_in> sockAddr = std::make_unique<sockaddr_in>();
   int remoteSize = sizeof(*remote), sockAddrSize = sizeof(*sockAddr);
   SOCKET clientSocketId = ::accept(this->socketId, (sockaddr*)remote.get(), &remoteSize);

   if (clientSocketId != INVALID_SOCKET &&
      ::getsockname(clientSocketId, (sockaddr*)sockAddr.get(), &sockAddrSize) == 0)
   {
      clientSocket = new ClientSocket(clientSocketId);

      clientSocket->setLocalAddressIp(Socket::convertAddressIpToStr(remote.get()).c_str());
      clientSocket->setPort(Socket::convertPortFromNetworkEndianness(remote.get()));
      clientSocket->setServerAddressIp(Socket::convertAddressIpToStr(sockAddr.get()).c_str());
      clientSocket->setLocalPort(Socket::convertPortFromNetworkEndianness(sockAddr.get()));
   }

   return clientSocket;
}