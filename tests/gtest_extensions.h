#pragma once

#include <ostream>

#include "AlphaCut.h"
#include "FuzzyNumber.h"
#include "Interval.h"

namespace FuzzyMath
{
    inline void PrintTo( const Interval &i, std::ostream *os ) { *os << i.to_string(); }

    inline void PrintTo( const FuzzyNumber &fn, std::ostream *os ) { *os << fn.to_string(); }

    inline void PrintTo( const AlphaCut &ac, std::ostream *os ) { *os << ac.to_string(); }
} // namespace FuzzyMath
