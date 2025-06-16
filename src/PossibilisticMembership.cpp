#include "PossibilisticMembership.h"

namespace FuzzyMath
{
    PossibilisticMembership::PossibilisticMembership( PreciseFloat p, PreciseFloat n )
        : m_possibility( p ), m_necessity( n )
    {
        if ( p < 0.0 || p > 1.0 )
        {
            throw std::out_of_range( "Possibility must be in [0, 1]" );
        }
        if ( n < 0.0 || n > 1.0 )
        {
            throw std::out_of_range( "Necessity must be in [0, 1]" );
        }
    }

    PossibilisticMembership::PossibilisticMembership( std::string p, std::string n )
        : PossibilisticMembership( PreciseFloat( p ), PreciseFloat( n ) )
    {
    }

    PossibilisticMembership::PossibilisticMembership() : m_possibility( 0.0 ), m_necessity( 0.0 ) {}

    bool PossibilisticMembership::operator==( const PossibilisticMembership &other ) const
    {
        return m_possibility == other.m_possibility && m_necessity == other.m_necessity;
    }

    std::string PossibilisticMembership::to_string() const
    {
        std::ostringstream oss;
        oss << "PossibilisticMembership(possibility: " << m_possibility << ", " << m_necessity << ")";
        return oss.str();
    }

    PreciseFloat PossibilisticMembership::possibility() const { return m_possibility; }

    PreciseFloat PossibilisticMembership::necessity() const { return m_necessity; }
} // namespace FuzzyMath
