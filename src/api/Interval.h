#pragma once

#include <functional>
#include <ostream>
#include <stdexcept>

#include <boost/numeric/interval.hpp>

#include "FuzzyMath.h"
#include "Types.h"

namespace FuzzyMath
{
    class DLL_API Interval
    {
      public:
        Interval();
        Interval( const PreciseFloat &a, const PreciseFloat &b );
        Interval( const PreciseFloat &a );
        Interval( const std::string &a, const std::string &b );
        Interval( const std::string &a );
        Interval( const Interval &other );

        PreciseFloat min() const;
        PreciseFloat max() const;
        long double min_as_double() const;
        long double max_as_double() const;
        bool is_degenerate() const;
        bool is_empty() const;
        PreciseFloat width() const;
        PreciseFloat mid_point() const;

        std::string to_string() const;

        bool contains( const PreciseFloat &x ) const;
        bool contains( const Interval &other ) const;

        bool intersects( const Interval &other ) const;
        Interval intersection( const Interval &other ) const;
        Interval unite( const Interval &other ) const;
        Interval union_hull( const Interval &other ) const;

        Interval apply_function( const std::function<PreciseFloat( const PreciseFloat & )> &func, bool monotone = false,
                                 size_t number_elements = 1000 ) const;

        Interval power( int exponent ) const;

        Interval operator+( const Interval &other ) const;
        Interval operator-( const Interval &other ) const;
        Interval operator*( const Interval &other ) const;
        Interval operator/( const Interval &other ) const;

        Interval operator-() const;
        bool operator==( const Interval &other ) const;
        bool operator<( const Interval &other ) const;
        bool operator>( const Interval &other ) const;

        bool operator<( const PreciseFloat &other ) const;
        bool operator>( const PreciseFloat &other ) const;

        bool operator<( const std::string &other ) const;
        bool operator>( const std::string &other ) const;

      private:
        boost::numeric::interval<PreciseFloat>
            m_interval_value; // Internal representation using Boost's interval library
    };

} // namespace FuzzyMath
