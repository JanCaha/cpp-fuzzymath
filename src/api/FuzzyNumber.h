#pragma once

#include <algorithm>
#include <concepts>
#include <map>
#include <ostream>
#include <set>
#include <stdexcept>

#include "AlphaCut.h"
#include "FuzzyMath.h"
#include "Interval.h"

using namespace FuzzyMath;

namespace FuzzyMath
{

    class DLL_API FuzzyNumber
    {
      public:
        FuzzyNumber( const std::set<AlphaCut> &alpha_cuts );

        size_t size() const;
        const AlphaCut alpha_cut( const PreciseFloat &alpha ) const;
        PreciseFloat min() const;
        PreciseFloat max() const;
        Interval support() const;
        Interval kernel() const;
        PreciseFloat kernel_min() const;
        PreciseFloat kernel_max() const;

        std::vector<PreciseFloat> alphas() const;

        static std::vector<PreciseFloat> alpha_cut_values( size_t number_of_cuts = 2 );

        const std::string to_string() const;
        const std::string to_descriptive_string() const;

        FuzzyNumber apply_function( const std::function<PreciseFloat( const PreciseFloat & )> &func,
                                    bool monotone = false, size_t number_elements = 1000 ) const;

        // Arithmetic
        FuzzyNumber operator-() const;
        FuzzyNumber operator+( const FuzzyNumber &other ) const;
        FuzzyNumber operator-( const FuzzyNumber &other ) const;
        FuzzyNumber operator*( const FuzzyNumber &other ) const;
        FuzzyNumber operator/( const FuzzyNumber &other ) const;

        bool operator==( const FuzzyNumber &other ) const;

        // Membership
        // FuzzyMembership membership( double value ) const;

      private:
        std::set<AlphaCut> m_alpha_cuts;

        std::vector<PreciseFloat> union_of_alpha_cuts( FuzzyNumber &other ) const;
        FuzzyNumber operation( const FuzzyNumber &other,
                               const std::function<AlphaCut( const AlphaCut &, const AlphaCut & )> &op ) const;
    };

} // namespace FuzzyMath
