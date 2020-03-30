#include <iostream>
#include <string>
#include <memory>
#include "ServerSocket.h"
#include "WinsockManager.h"

#define RECV_BUFF_SIZE 4096
#define DLL_WINSOCK_VERSION MAKEWORD(2, 2)
#define BACKLOG 1

int main()
{
   std::unique_ptr<WinsockManager> winsockManager = std::make_unique<WinsockManager>();
   std::unique_ptr<ServerSocket> serverSocket = nullptr;
   uint16_t port;
   char recvBuff[RECV_BUFF_SIZE];
   int bytesSend = 0;
   int bytesReceived = 0;

   if (false == winsockManager->startup(DLL_WINSOCK_VERSION))
   {
      printf("Winsock initialization error\n");
      return -1;
   }

   std::cout << "Enter port: ";
   std::cin >> port;

   serverSocket = std::make_unique<ServerSocket>();

   if (true == serverSocket->init(IpProtocol::IPV4, TxProtocol::TCP))
   {
      std::cout << "Server socket initialized\n";
   }
   else
   {
      std::cout << "Cannot initialiaze a socket\n";
      std::cout << "Error: " << winsockManager->getErrorMessage() << "\n";
      winsockManager->cleanup();
      return -1;
   }

   if (true == serverSocket->bind(nullptr, port))
   {
      std::cout << "Server socket bound [" << "INADDR_ANY" << ", " << port << "]\n";
   }
   else
   {
      std::cout << "Cannot bind socket server [" << "INADDR_ANY" << ", " << port << "]\n";
      std::cout << "Error: " << winsockManager->getErrorMessage() << "\n";
      serverSocket->close();
      std::cout << "Server socket closed" << "\n";
      winsockManager->cleanup();
      return -1;
   }

   if (true == serverSocket->listen(BACKLOG))
   {
      std::cout << "Server socket started listening [max " << BACKLOG << " connections]\n";
   }
   else
   {
      std::cout << "Cannot start listening socket server\n";
      std::cout << "Error: " << winsockManager->getErrorMessage() << "\n";
      serverSocket->close();
      std::cout << "Server socket closed" << "\n";
      winsockManager->cleanup();
      return -1;
   }

   ClientSocket* clientSocket = serverSocket->accept();

   std::cout << "GetLocalAddressIP: " << clientSocket->getLocalAddressIp() << "\n";
   std::cout << "GetServerAddressIP: " << clientSocket->getServerAddressIp() << "\n";
   std::cout << "GetPort: " << clientSocket->getPort() << "\n";
   std::cout << "GetLocalPort: " << clientSocket->getLocalPort() << "\n";

   if (clientSocket != nullptr)
   {
      std::cout << "New client is connected with Server\n";
   }
   else
   {
      std::cout << "Error occurred when new client tried to connect with server\n";
   }

   do
   {
      if ((bytesReceived = clientSocket->recv(recvBuff, RECV_BUFF_SIZE)) > 0)
      {
         std::cout << "Output from client: " << recvBuff << "\n";
         if (true == clientSocket->send(recvBuff, bytesReceived))
         {
            std::cout << "Send to client: " << recvBuff << "\n";
         }
      }
      else if(bytesReceived == 0)
      {
         std::cout << "Client disconnected\n";
      }
      else
      {
         std::cout << "Error: " << winsockManager->getErrorMessage() << "\n";
      }

   } while (bytesReceived > 0);

   clientSocket->close();
   std::cout << "Client socket closed" << "\n";
   serverSocket->close();
   std::cout << "Server socket closed" << "\n";
   winsockManager->cleanup();
   return 0;
}