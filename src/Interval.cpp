#include <algorithm>
#include <limits>
#include <ostream>
#include <stdexcept>

#include "Interval.h"

#include <boost/multiprecision/cpp_dec_float.hpp>

using namespace FuzzyMath;

Interval::Interval() : m_interval_value( boost::numeric::interval<PreciseFloat>() )
{
    m_interval_value.set( std::numeric_limits<PreciseFloat>::quiet_NaN(),
                          std::numeric_limits<PreciseFloat>::quiet_NaN() );
}

Interval::Interval( const PreciseFloat &a ) : Interval( a, a ) {}

Interval::Interval( const PreciseFloat &a, const PreciseFloat &b )
    : m_interval_value( boost::numeric::interval<PreciseFloat>( std::min( a, b ), std::max( a, b ) ) )
{
}

Interval::Interval( const std::string &a, const std::string &b )
{
    PreciseFloat val1 = PreciseFloat( a );
    PreciseFloat val2 = PreciseFloat( b );

    m_interval_value = boost::numeric::interval<PreciseFloat>( std::min( val1, val2 ), std::max( val1, val2 ) );
}

Interval::Interval( const std::string &a ) : Interval( a, a ) {}

Interval::Interval( const Interval &other ) { m_interval_value = other.m_interval_value; }

PreciseFloat Interval::min() const { return m_interval_value.lower(); }

PreciseFloat Interval::max() const { return m_interval_value.upper(); }

long double Interval::min_as_double() const { return static_cast<long double>( min().convert_to<long double>() ); }

long double Interval::max_as_double() const { return static_cast<long double>( max().convert_to<long double>() ); }

bool Interval::is_degenerate() const { return min() == max(); }

bool Interval::is_empty() const
{
    if ( boost::math::isnan( min() ) || boost::math::isnan( max() ) )
    {
        return true;
    }

    return boost::numeric::empty( m_interval_value );
}

std::string Interval::to_string() const
{
    std::ostringstream oss;
    oss << "[" << min() << ", " << max() << "]";
    return oss.str();
}

PreciseFloat Interval::width() const { return max() - min(); }

PreciseFloat Interval::mid_point() const { return ( min() + max() ) / PreciseFloat( 2 ); }

bool Interval::contains( const PreciseFloat &x ) const { return min() <= x && x <= max(); }

bool Interval::contains( const Interval &other ) const { return min() <= other.min() && max() >= other.max(); }

bool Interval::intersects( const Interval &other ) const
{
    if ( is_empty() || other.is_empty() )
        return false;

    return !( max() < other.min() || min() > other.max() );
}

Interval Interval::intersection( const Interval &other ) const
{
    if ( !intersects( other ) )
        throw std::domain_error( "Intervals do not intersect" );

    return Interval( std::max( min(), other.min() ), std::min( max(), other.max() ) );
}

Interval Interval::unite( const Interval &other ) const
{
    if ( !intersects( other ) )
        throw std::domain_error( "Intervals do not intersect, cannot construct valid union." );

    return Interval( std::min( min(), other.min() ), std::max( max(), other.max() ) );
}

Interval Interval::union_hull( const Interval &other ) const
{
    if ( is_empty() )
        return other;
    if ( other.is_empty() )
        return *this;

    return Interval( std::min( min(), other.min() ), std::max( max(), other.max() ) );
}

Interval Interval::power( int exponent ) const
{
    if ( is_empty() )
        return Interval();

    if ( exponent == 0 )
        return Interval( PreciseFloat( 1 ) );

    if ( exponent < 0 )
        throw std::domain_error( "Exponent must be non-negative for power operation" );

    PreciseFloat min_power = boost::multiprecision::pow( min(), exponent );
    PreciseFloat max_power = boost::multiprecision::pow( max(), exponent );

    PreciseFloat min_res;
    PreciseFloat max_res;

    if ( ( exponent % 2 ) == 0 )
    {
        if ( min() <= PreciseFloat( 0 ) && max() >= PreciseFloat( 0 ) )
        {
            min_res = std::min( PreciseFloat( 0 ), std::max( min_power, max_power ) );
            max_res = std::max( PreciseFloat( 0 ), std::max( min_power, max_power ) );
        }
        else
        {
            min_res = std::min( min_power, max_power );
            max_res = std::max( min_power, max_power );
        }
    }
    else
    {
        min_res = std::min( min_power, max_power );
        max_res = std::max( min_power, max_power );
    }
    return Interval( min_res, max_res );
}

Interval Interval::apply_function( const std::function<PreciseFloat( const PreciseFloat & )> &func, bool monotone,
                                   size_t number_elements ) const
{
    if ( is_empty() )
        return Interval();

    if ( monotone )
    {
        PreciseFloat val1 = func( min() );
        PreciseFloat val2 = func( max() );
        return Interval( std::min( val1, val2 ), std::max( val1, val2 ) );
    }
    else
    {
        if ( number_elements < 2 )
        {
            number_elements = 2; // Ensure at least two points for a valid interval
        }

        std::vector<PreciseFloat> values;
        values.reserve( number_elements + 1 );

        // Generate values between start and end (inclusive)
        for ( size_t i = 0; i <= number_elements; ++i )
        {
            PreciseFloat t = PreciseFloat( i ) / PreciseFloat( number_elements );
            PreciseFloat x = min() + t * ( max() - min() );
            values.push_back( func( x ) );
        }

        auto minmax = std::minmax_element( values.begin(), values.end() );
        return Interval( *minmax.first, *minmax.second );
    }
}

Interval Interval::operator+( const Interval &other ) const
{
    return Interval( min() + other.min(), max() + other.max() );
}

Interval Interval::operator-( const Interval &other ) const
{
    return Interval( min() - other.max(), max() - other.min() );
}

Interval Interval::operator*( const Interval &other ) const
{
    std::vector<PreciseFloat> products = {
        min() * other.min(),
        min() * other.max(),
        max() * other.min(),
        max() * other.max(),
    };
    PreciseFloat lower = *std::min_element( products.begin(), products.end() );
    PreciseFloat upper = *std::max_element( products.begin(), products.end() );
    return Interval( lower, upper );
}

Interval Interval::operator/( const Interval &other ) const
{
    if ( other.contains( PreciseFloat( 0 ) ) )
        throw std::domain_error( "Division by interval containing zero" );

    std::vector<PreciseFloat> products = {
        min() / other.min(),
        min() / other.max(),
        max() / other.min(),
        max() / other.max(),
    };
    PreciseFloat lower = *std::min_element( products.begin(), products.end() );
    PreciseFloat upper = *std::max_element( products.begin(), products.end() );
    return Interval( lower, upper );
}

Interval Interval::operator-() const { return Interval( -m_interval_value.upper(), -m_interval_value.lower() ); }

bool Interval::operator==( const Interval &other ) const { return min() == other.min() && max() == other.max(); }

bool Interval::operator<( const Interval &other ) const { return max() < other.min(); }

bool Interval::operator>( const Interval &other ) const { return min() > other.max(); }

bool Interval::operator<( const PreciseFloat &other ) const { return *this < Interval( other ); }

bool Interval::operator>( const PreciseFloat &other ) const { return *this > Interval( other ); }

bool Interval::operator<( const std::string &other ) const { return *this < Interval( other ); }

bool Interval::operator>( const std::string &other ) const { return *this > Interval( other ); }

Interval operator+( int lhs, const Interval &rhs ) { return Interval( PreciseFloat( lhs ) ) + rhs; }

Interval operator-( int lhs, const Interval &rhs ) { return Interval( PreciseFloat( lhs ) ) - rhs; }

Interval operator*( int lhs, const Interval &rhs ) { return Interval( PreciseFloat( lhs ) ) * rhs; }

Interval operator/( int lhs, const Interval &rhs ) { return Interval( PreciseFloat( lhs ) ) / rhs; }

Interval operator+( double lhs, const Interval &rhs ) { return Interval( PreciseFloat( lhs ) ) + rhs; }

Interval operator-( double lhs, const Interval &rhs ) { return Interval( PreciseFloat( lhs ) ) - rhs; }

Interval operator*( double lhs, const Interval &rhs ) { return Interval( PreciseFloat( lhs ) ) * rhs; }

Interval operator/( double lhs, const Interval &rhs ) { return Interval( PreciseFloat( lhs ) ) / rhs; }

Interval operator+( float lhs, const Interval &rhs ) { return Interval( PreciseFloat( lhs ) ) + rhs; }

Interval operator-( float lhs, const Interval &rhs ) { return Interval( PreciseFloat( lhs ) ) - rhs; }

Interval operator*( float lhs, const Interval &rhs ) { return Interval( PreciseFloat( lhs ) ) * rhs; }

Interval operator/( float lhs, const Interval &rhs ) { return Interval( PreciseFloat( lhs ) ) / rhs; }
