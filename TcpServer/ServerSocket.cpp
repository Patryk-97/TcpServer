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
   int size = sizeof(*remote);
   SOCKET clientSocketId = ::accept(this->socketId, (sockaddr*)remote.get(), &size);

   if (clientSocketId != INVALID_SOCKET)
   {
      clientSocket = new ClientSocket(clientSocketId);
   }

   std::string ipAddress = "\"";

   ipAddress += std::to_string(remote->sin_addr.S_un.S_un_b.s_b1) + ".";
   ipAddress += std::to_string(remote->sin_addr.S_un.S_un_b.s_b2) + ".";
   ipAddress += std::to_string(remote->sin_addr.S_un.S_un_b.s_b3) + ".";
   ipAddress += std::to_string(remote->sin_addr.S_un.S_un_b.s_b4) + "\"";

   std::cout << "Polaczenie z " << ipAddress << ": " << ntohs(remote->sin_port) << "\n";


   return clientSocket;
}