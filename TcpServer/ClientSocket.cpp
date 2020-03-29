#include "ClientSocket.h"

ClientSocket::ClientSocket() : Socket()
{

}

ClientSocket::ClientSocket(SOCKET socketId) : Socket()
{
   this->socketId = socketId;
}

ClientSocket::~ClientSocket()
{

}

bool ClientSocket::connect(const char* address, const uint16_t port)
{
   // locals
   bool rV = true;

   this->fillIpProtocolFamily();
   this->fillPort(port);
   rV = this->fillNetworkAddressStructure(address);

   if (true == rV && 
       ::connect(this->socketId, (sockaddr*)this->socketAddr.get(), sizeof(*this->socketAddr)) == SOCKET_ERROR)
   {
      rV = false;
   }

   return rV;
}

std::string ClientSocket::getSocketName(void)
{
   // locals
   struct sockaddr_in socketAddress;
   int addrlen = sizeof(socketAddress);
   std::string ipAddress = "\"";

   if (getsockname(this->socketId, (struct sockaddr*)&socketAddress, &addrlen) == 0)
   {
      ipAddress += std::to_string(socketAddress.sin_addr.S_un.S_un_b.s_b1) + ".";
      ipAddress += std::to_string(socketAddress.sin_addr.S_un.S_un_b.s_b2) + ".";
      ipAddress += std::to_string(socketAddress.sin_addr.S_un.S_un_b.s_b3) + ".";
      ipAddress += std::to_string(socketAddress.sin_addr.S_un.S_un_b.s_b4) + "\"";
   }
   else
   {

   }

   return ipAddress;
}