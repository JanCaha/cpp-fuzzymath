#pragma once

#include <ostream>
#include <stdexcept>

#include "FuzzyMath.h"
#include "Types.h"

namespace FuzzyMath
{
    class DLL_API PossibilisticMembership
    {
      public:
        PossibilisticMembership( PreciseFloat p, PreciseFloat n );

        PossibilisticMembership( std::string p, std::string n );

        PossibilisticMembership();

        bool operator==( const PossibilisticMembership &other ) const;

        std::string to_string() const;

        PreciseFloat possibility() const;
        PreciseFloat necessity() const;

      private:
        PreciseFloat m_possibility, m_necessity;
    };

} // namespace FuzzyMath
