#pragma once

#include <Windows.h>
#include <string>

class UnnamedPipe
{
   private:
      HANDLE _inRdHandle;
      HANDLE _inWrHandle;
      HANDLE _outRdHandle;
      HANDLE _outWrHandle;
      CHAR chBuf[4096];
   public:
      UnnamedPipe();
      ~UnnamedPipe();
      HANDLE getProcInHandle();
      HANDLE getProcOutHandle();
      void writeToPipe( std::string data );
      std::string readFromPipe();
};