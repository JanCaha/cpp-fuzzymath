#pragma once

#include <algorithm>
#include <ostream>
#include <stdexcept>

#include "FuzzyMath.h"
#include "Types.h"

namespace FuzzyMath
{

    class DLL_API FuzzyMembership
    {
      public:
        FuzzyMembership( PreciseFloat v );
        FuzzyMembership( std::string v );

        FuzzyMembership();

        bool operator==( const FuzzyMembership &other ) const;
        bool operator!=( const FuzzyMembership &other ) const;
        bool operator<( const FuzzyMembership &other ) const;
        bool operator<=( const FuzzyMembership &other ) const;
        bool operator>( const FuzzyMembership &other ) const;
        bool operator>=( const FuzzyMembership &other ) const;

        std::string to_string() const;

        long double as_double() const;

      private:
        PreciseFloat value = PreciseFloat( 0 );
    };

} // namespace FuzzyMath
