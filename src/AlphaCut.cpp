#include <AlphaCut.h>

using namespace FuzzyMath;

AlphaCut::AlphaCut( const PreciseFloat &alpha )
    : AlphaCut( alpha, Interval( -std::numeric_limits<PreciseFloat>::max(), std::numeric_limits<PreciseFloat>::max() ) )
{
}

AlphaCut::AlphaCut( const PreciseFloat &alpha, const Interval &interval ) : m_alpha( alpha ), m_interval( interval )
{
    if ( alpha < 0 || alpha > 1 )
    {
        throw std::out_of_range( "Alpha value must be in [0, 1]." );
    }
    if ( interval.is_empty() )
    {
        throw std::invalid_argument( "Interval of alpha cut cannot be empty." );
    }
}

PreciseFloat AlphaCut::alpha() const { return m_alpha; }

Interval AlphaCut::interval() const { return m_interval; }

std::string AlphaCut::to_string() const
{
    std::ostringstream oss;
    oss << "AlphaCut(alpha: " << m_alpha << ", interval: " << m_interval.to_string() << ")";
    return oss.str();
}

bool AlphaCut::contains( const AlphaCut &other ) const
{
    if ( m_alpha > other.m_alpha )
    {
        throw std::domain_error( "Cannot check containment: this alpha cut has a higher alpha value than the other." );
    }

    return m_interval.contains( other.m_interval );
}

AlphaCut AlphaCut::apply_function( const std::function<PreciseFloat( const PreciseFloat & )> &func, bool monotone,
                                   size_t number_elements ) const
{
    return AlphaCut( m_alpha, m_interval.apply_function( func, monotone, number_elements ) );
}

bool AlphaCut::operator==( const AlphaCut &other ) const
{
    return m_alpha == other.m_alpha && m_interval == other.m_interval;
}

AlphaCut AlphaCut::operator-() const { return AlphaCut( m_alpha, -m_interval ); }

std::weak_ordering AlphaCut::operator<=>( const AlphaCut &other ) const
{
    if ( m_alpha < other.m_alpha )
        return std::weak_ordering::less;
    if ( m_alpha > other.m_alpha )
        return std::weak_ordering::greater;
    return std::weak_ordering::equivalent;
}
