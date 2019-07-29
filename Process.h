#pragma once
#include <string>
#include <Windows.h>

class ProcessViaCreateProcess
{
   private:
      PROCESS_INFORMATION _process;
   public:
      ProcessViaCreateProcess( std::string commandline, HANDLE inhandle, HANDLE outhandle );
      void waitUntilTerminated();
      ~ProcessViaCreateProcess();
};

class ProcessViaShellExec
{
   private:
      SHELLEXECUTEINFOA _info;
   public:
      ProcessViaShellExec( std::string program, std::string params );
      void waitUntilTerminated();
      ~ProcessViaShellExec();
};