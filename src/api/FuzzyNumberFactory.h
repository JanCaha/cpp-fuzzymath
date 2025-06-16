#pragma once

#include "FuzzyMath.h"
#include "FuzzyNumber.h"
#include "Interval.h"

namespace FuzzyMath
{
    class DLL_API FuzzyNumberFactory
    {
      public:
        static FuzzyNumber triangular( PreciseFloat minimum, PreciseFloat kernel, PreciseFloat maximum,
                                       int number_of_cuts = 2 );
        static FuzzyNumber triangular( std::string minimum, std::string kernel, std::string maximum,
                                       int number_of_cuts = 2 );

        static FuzzyNumber trapezoidal( PreciseFloat minimum, PreciseFloat kernel_minimum, PreciseFloat kernel_maximum,
                                        PreciseFloat maximum, int number_of_cuts = 2 );
        static FuzzyNumber trapezoidal( std::string minimum, std::string kernel_minimum, std::string kernel_maximum,
                                        std::string maximum, int number_of_cuts = 2 );

        static FuzzyNumber crisp_number( PreciseFloat value );
        static FuzzyNumber crisp_number( std::string value );
    };
} // namespace FuzzyMath