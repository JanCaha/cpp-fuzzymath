#pragma once
#include <compare>

#include "FuzzyMath.h"
#include "Interval.h"
#include "Types.h"

namespace FuzzyMath
{
    class DLL_API AlphaCut
    {
      public:
        AlphaCut( const PreciseFloat &alpha, const Interval &interval );
        AlphaCut( const PreciseFloat &alpha );

        template <typename T>
            requires( std::integral<T> || std::floating_point<T> || std::same_as<T, std::string> ||
                      std::is_convertible_v<T, std::string> )
        AlphaCut( const T &alpha ) : AlphaCut( PreciseFloat( alpha ) ){};

        template <typename T>
            requires( std::integral<T> || std::floating_point<T> || std::same_as<T, std::string> ||
                      std::is_convertible_v<T, std::string> )
        AlphaCut( const T &alpha, const Interval &interval ) : AlphaCut( PreciseFloat( alpha ), interval ){};

        std::weak_ordering operator<=>( const AlphaCut &other ) const;
        bool operator==( const AlphaCut &other ) const;

        PreciseFloat alpha() const;
        Interval interval() const;

        std::string to_string() const;

        bool contains( const AlphaCut &other ) const;

        AlphaCut operator-() const;

        AlphaCut apply_function( const std::function<PreciseFloat( const PreciseFloat & )> &func, bool monotone = false,
                                 size_t number_elements = 1000 ) const;

      private:
        PreciseFloat m_alpha;
        Interval m_interval;
    };

} // namespace FuzzyMath