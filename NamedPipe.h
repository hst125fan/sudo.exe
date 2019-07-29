#pragma once
#include <string>
#include <Windows.h>

class NamedPipe
{
   private:
      std::string _connectorToCreatorPipeName;
      std::string _creatorToConnectorPipeName;
      HANDLE _connectorToCreatorPipe;
      HANDLE _creatorToConnectorPipe;
      bool _isCreator;
      char _buffer[4096];
   public:
      NamedPipe( const std::string& pipename, bool create );
      ~NamedPipe();
      void creatorPrepareRecv();
      std::string recvData();
      void sendData( std::string data );
};