#include "Process.h"

ProcessViaCreateProcess::ProcessViaCreateProcess( std::string commandline, HANDLE inhandle, HANDLE outhandle )
{
   STARTUPINFO startupInfo;
   ZeroMemory( &_process, sizeof( PROCESS_INFORMATION ) );
   ZeroMemory( &startupInfo, sizeof( STARTUPINFO ) );
   startupInfo.cb = sizeof( STARTUPINFO );
   startupInfo.hStdError = outhandle;
   startupInfo.hStdOutput = outhandle;
   startupInfo.hStdInput = inhandle;
   startupInfo.dwFlags = STARTF_USESTDHANDLES;
   BOOL bSuccess = CreateProcess( NULL,
      (char*)commandline.c_str(),
      NULL,
      NULL,
      TRUE,
      0,
      NULL,
      NULL,
      &startupInfo,
      &_process );
   if( !bSuccess )
   {
      throw std::exception( std::string("CreateProcess failed with error: " + std::to_string( (long long)GetLastError() ) ).c_str() );
   }
}

void ProcessViaCreateProcess::waitUntilTerminated()
{
   WaitForSingleObject( _process.hProcess, INFINITE );
}

ProcessViaCreateProcess::~ProcessViaCreateProcess()
{
   CloseHandle( _process.hProcess );
   CloseHandle( _process.hThread );
}

ProcessViaShellExec::ProcessViaShellExec( std::string program, std::string params )
{
   ZeroMemory( &_info, sizeof( SHELLEXECUTEINFOA ) );
   _info.cbSize = sizeof( SHELLEXECUTEINFOA );
   _info.lpVerb = "runas";
   _info.fMask = SEE_MASK_NOCLOSEPROCESS | SEE_MASK_NOASYNC | SEE_MASK_FLAG_NO_UI;
   _info.nShow = 0;
   _info.lpFile = program.c_str();
   _info.lpParameters = params.c_str();
   BOOL success = ShellExecuteEx( &_info );
   if( !success )
   {
      throw std::exception( std::string( "ShellExecuteEx failed with error: " + std::to_string( (long long)GetLastError() ) ).c_str() );
   }
}

void ProcessViaShellExec::waitUntilTerminated()
{
   WaitForSingleObject( _info.hProcess, INFINITE );
}

ProcessViaShellExec::~ProcessViaShellExec()
{
   CloseHandle( _info.hProcess );
}
