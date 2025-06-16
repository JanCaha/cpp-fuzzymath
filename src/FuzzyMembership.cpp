#include "FuzzyMembership.h"

using namespace FuzzyMath;

FuzzyMembership::FuzzyMembership( std::string v ) : FuzzyMembership( PreciseFloat( v ) ) {}

FuzzyMembership::FuzzyMembership( PreciseFloat v )
{
    if ( v < 0.0 || v > 1.0 )
        throw std::out_of_range( "Membership value must be in [0, 1]" );
}

std::string FuzzyMembership::to_string() const
{
    std::ostringstream oss;
    oss << "FuzzyMembership(" << value << ")";
    return oss.str();
}

long double FuzzyMembership::as_double() const { return value.convert_to<long double>(); }

bool FuzzyMembership::operator==( const FuzzyMembership &other ) const { return value == other.value; }

bool FuzzyMembership::operator!=( const FuzzyMembership &other ) const { return value != other.value; }

bool FuzzyMembership::operator<( const FuzzyMembership &other ) const { return value < other.value; }

bool FuzzyMembership::operator<=( const FuzzyMembership &other ) const { return value <= other.value; }

bool FuzzyMembership::operator>( const FuzzyMembership &other ) const { return value > other.value; }

bool FuzzyMembership::operator>=( const FuzzyMembership &other ) const { return value >= other.value; }