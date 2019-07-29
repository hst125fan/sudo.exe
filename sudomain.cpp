#include <iostream>
#include <string>
#include <thread>
#include "NamedPipe.h"
#include "UnnamedPipe.h"
#include "Process.h"
#include "Stringutils.h"



int server( int argc, char** argv )
{
   Sleep( 100 );
   try
   {
      NamedPipe procComm( std::string( "nvbsudopipe" ), false );
      try
      {
         UnnamedPipe myPipe;
         ProcessViaCreateProcess process( "cmd /c " + StringUtils::argcArgvToCmdString( argc, argv ),
            myPipe.getProcInHandle(),
            myPipe.getProcOutHandle() );
         std::thread t1( [&]( NamedPipe* procComm, UnnamedPipe* myPipe )
         {
            for( ;; )
            {
               try
               {
                  std::string data = procComm->recvData();
                  myPipe->writeToPipe( data );
               }
               catch( std::exception )
               {

               }
            }
         }, &procComm, &myPipe );
         std::thread t2( [&]( NamedPipe* procComm, UnnamedPipe* myPipe )
         {
            for( ;; )
            {
               try
               {
                  std::string data = myPipe->readFromPipe();
                  procComm->sendData( data );
               }
               catch( std::exception )
               {

               }
            }
         }, &procComm, &myPipe );
         process.waitUntilTerminated();
         std::exit( 0 );
      }
      catch( std::exception& e )
      {
         procComm.sendData( std::string( "Internal Error: " ) + e.what() );
         std::exit( 1 );
      }
   }
   catch( std::exception& )
   {
      std::cout << "Call chain detected. " << argv[0] << " may not call itself!";
      std::exit( 0 );
   }
}

int client( int argc, char** argv )
{
   try
   {
      NamedPipe pipe( std::string( "nvbsudopipe" ), true );
      ProcessViaShellExec process( std::string( argv[0] ), StringUtils::argcArgvToCmdString( argc, argv ) );
      pipe.creatorPrepareRecv();
      std::thread t1( [&]( NamedPipe* pipe )
      {
         try
         {
            for( ;; )
            {
               std::string data = pipe->recvData();
               std::cout << data;
            }
         }
         catch( std::exception )
         {

         }
      }, &pipe );
      std::thread t2( [&]( NamedPipe* pipe )
      {
         try
         {
            for( ;; )
            {
               std::string input;
               std::getline( std::cin, input );
               pipe->sendData( input );
            }
         }
         catch( std::exception )
         {

         }
      }, &pipe );
      process.waitUntilTerminated();
      std::exit( 0 );
   }
   catch( std::exception& e )
   {
      std::cout << "Internal Error: " << e.what();
      std::exit( 1 );
   }
}

int main( int argc, char** argv )
{
   //Error case: not enough arguments
   if( argc < 2 )
   {
      std::cout << "No Program to execute. Usage: " << argv[0] << " <program>";
      return 1;
   }

   //Determine if this is a server process - because calling this program as program to execute does not make sense
   std::string a( argv[0] );
   std::string b( argv[1] );
   if( StringUtils::shortenPgmName( a ) == StringUtils::shortenPgmName( b ) )
   {
      if( argc < 3 )
      {
         std::cout << "Second parameter must be a different command or executable" << std::endl;
         return 1;
      }
      return server( argc - 2, &argv[2] );
   }
   else
   {
      return client( argc, argv );
   }
}