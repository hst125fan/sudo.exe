#include "Stringutils.h"
#include <Windows.h>

std::string& StringUtils::insertEscapeSequence( std::string& input )
{
   for( std::string::iterator iter = input.begin(); iter != input.end(); iter++ )
   {
      if( *iter == '\"' )
      {
         input.insert( iter, '\\' );
         iter++;
      }
   }
   return input;
}

std::string& StringUtils::toCliParameter( std::string& input )
{
   if( std::string::npos != input.find( ' ' ) )
   {
      input = "\"" + input + "\"";
   }
   return input;
}

std::string StringUtils::argcArgvToCmdString( int argc, char** argv )
{
   std::string commandstring;
   bool firstRun = true;
   for( int i = 0; i < argc; i++ )
   {
      std::string subcommandstring = argv[i];
      insertEscapeSequence( subcommandstring );
      toCliParameter( subcommandstring );
      if( firstRun )
      {
         firstRun = false;
      }
      else
      {
         commandstring += " ";
      }
      commandstring += subcommandstring;
   }
   return commandstring;
}

std::string & StringUtils::shortenPgmName( std::string & input )
{
   std::string::iterator cutFromHere = input.begin();
   for( std::string::iterator iter1 = input.begin(); iter1 != input.end(); iter1++ )
   {
      if( *iter1 == '/' || *iter1 == '\\' )
      {
         cutFromHere = iter1;
      }
   }

   if( cutFromHere != input.begin() )
   {
      input.erase( input.begin(), ++cutFromHere );
   }

   std::string::iterator cutToHere = input.end();
   for( std::string::iterator iter2 = input.begin(); iter2 != input.end(); iter2++ )
   {
      if( *iter2 == '.' )
      {
         cutToHere = iter2;
      }
   }

   if( cutToHere != input.end() )
   {
      input.erase( cutToHere, input.end() );
   }

   return input;
}
