#pragma once
#include <string>


class StringUtils
{
   public:
      static std::string& insertEscapeSequence( std::string& input );
      static std::string& toCliParameter( std::string& input );
      static std::string argcArgvToCmdString( int argc, char** argv );
      static std::string& shortenPgmName( std::string& input );
};

