#include "NamedPipe.h"

NamedPipe::NamedPipe( const std::string & pipename, bool create ) : _isCreator( create ),
   _connectorToCreatorPipeName( "\\\\.\\pipe\\" + pipename + "1" ),
   _creatorToConnectorPipeName( "\\\\.\\pipe\\" + pipename + "2" )
{
   if( _isCreator )
   {
      _connectorToCreatorPipe = CreateNamedPipe( _connectorToCreatorPipeName.c_str(),
         PIPE_ACCESS_INBOUND,
         PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
         1,
         1024 * 16,
         1024 * 16,
         NMPWAIT_USE_DEFAULT_WAIT,
         NULL );
      _creatorToConnectorPipe = CreateNamedPipe( _creatorToConnectorPipeName.c_str(),
         PIPE_ACCESS_OUTBOUND,
         PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
         1,
         1024 * 16,
         1024 * 16,
         NMPWAIT_USE_DEFAULT_WAIT,
         NULL );
   }
   else
   {
      _connectorToCreatorPipe = CreateFile( _connectorToCreatorPipeName.c_str(),
         GENERIC_WRITE,
         0,
         NULL,
         OPEN_EXISTING,
         0,
         NULL );
      _creatorToConnectorPipe = CreateFile( _creatorToConnectorPipeName.c_str(),
         GENERIC_READ ,
         0,
         NULL,
         OPEN_EXISTING,
         0,
         NULL );
   }
   if( _connectorToCreatorPipe == INVALID_HANDLE_VALUE || _creatorToConnectorPipe == INVALID_HANDLE_VALUE )
   {
      throw std::exception( "Cannot create or open pipe" );
   }
}

NamedPipe::~NamedPipe()
{
   if( _isCreator )
   {
      DisconnectNamedPipe( _connectorToCreatorPipe );
      DisconnectNamedPipe( _creatorToConnectorPipe );
   }
   else
   {
      CloseHandle( _connectorToCreatorPipe );
      CloseHandle( _creatorToConnectorPipe );
   }
}

void NamedPipe::creatorPrepareRecv()
{
   if( _isCreator )
   {
      ConnectNamedPipe( _connectorToCreatorPipe, NULL );
      ConnectNamedPipe( _creatorToConnectorPipe, NULL );
   }
}

std::string NamedPipe::recvData()
{
   DWORD dwRead = 0;
   if( ReadFile( ( _isCreator ) ? _connectorToCreatorPipe : _creatorToConnectorPipe, _buffer, 4096, &dwRead, NULL ) )
   {
      _buffer[dwRead] = '\0';
      return std::string( _buffer );
   }
   if( GetLastError() == ERROR_BROKEN_PIPE )
   {
      throw std::exception( "Pipe terminated" );
   }
   return "";
}

void NamedPipe::sendData( std::string data )
{
   DWORD written = 0;
   BOOL bSuccess = WriteFile( ( _isCreator ) ? _creatorToConnectorPipe : _connectorToCreatorPipe, data.c_str(), (DWORD)data.length() + 1, &written, NULL );
   if( written = 0 || !bSuccess )
   {
      throw std::exception( "Cannot send data" );
   }
}
