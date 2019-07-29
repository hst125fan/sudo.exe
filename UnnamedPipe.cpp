#include "UnnamedPipe.h"
#include <exception>

UnnamedPipe::UnnamedPipe()
{
   SECURITY_ATTRIBUTES saAttr;
   saAttr.nLength = sizeof( SECURITY_ATTRIBUTES );
   saAttr.bInheritHandle = TRUE;
   saAttr.lpSecurityDescriptor = NULL;
   if( !CreatePipe( &_outRdHandle, &_outWrHandle, &saAttr, 0 ) )
   {
      throw std::exception( "OutPipe failed!" );
   }
   if( !SetHandleInformation( _outRdHandle, HANDLE_FLAG_INHERIT, 0 ) )
   {
      throw std::exception( "OutPipe inherited!" );
   }
   if( !CreatePipe( &_inRdHandle, &_inWrHandle, &saAttr, 0 ) )
   {
      throw std::exception( "InPipe failed!" );
   }
   if( !SetHandleInformation( _inWrHandle, HANDLE_FLAG_INHERIT, 0 ) )
   {
      throw std::exception( "InPipe inherited!" );
   }
}

UnnamedPipe::~UnnamedPipe()
{
   CloseHandle( _inRdHandle );
   CloseHandle( _inWrHandle );
   CloseHandle( _outRdHandle );
   CloseHandle( _outWrHandle );
}

HANDLE UnnamedPipe::getProcInHandle()
{
   return _inRdHandle;
}

HANDLE UnnamedPipe::getProcOutHandle()
{
   return _outWrHandle;
}

void UnnamedPipe::writeToPipe( std::string data )
{
   DWORD written = 0;
   BOOL bSuccess = WriteFile( _inWrHandle, data.c_str(), (DWORD)data.length() + 1, &written, NULL );
   if( !bSuccess )
   {
      throw std::exception( "Writing to pipe failed!" );
   }
}

std::string UnnamedPipe::readFromPipe()
{
   DWORD dwRead;
   BOOL bSuccess = ReadFile( _outRdHandle, chBuf, 4096, &dwRead, NULL );
   chBuf[dwRead] = '\0';
   if( !bSuccess || dwRead == 0 )
   {
      throw std::exception( "Reading from pipe failed" );
   }
   return std::string( chBuf );
}
