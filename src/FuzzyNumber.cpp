#include <algorithm>
#include <iterator>
#include <set>
#include <stdexcept>

#include "FuzzyMembership.h"
#include "FuzzyNumber.h"
#include "FuzzyNumberFactory.h"

using namespace FuzzyMath;

FuzzyNumber::FuzzyNumber( const std::set<AlphaCut> &alpha_cuts )
{
    if ( alpha_cuts.size() < 2 )
    {
        throw std::invalid_argument( "FuzzyNumber must have at least two alpha cuts" );
    }

    if ( alpha_cuts.begin()->alpha() != PreciseFloat( 0 ) )
    {
        throw std::invalid_argument( "FuzzyNumber must start with alpha cut 0" );
    }

    if ( std::prev( alpha_cuts.end() )->alpha() != PreciseFloat( 1 ) )
    {
        throw std::invalid_argument( "FuzzyNumber must end with alpha cut 1" );
    }

    AlphaCut previous = *alpha_cuts.begin();

    for ( const AlphaCut current : alpha_cuts )
    {
        if ( current.alpha() == PreciseFloat( 0 ) )
        {
            continue;
        }

        if ( !previous.contains( current ) )
        {
            throw std::invalid_argument( "Alpha cut intervals must be nested" );
        }
    }

    m_alpha_cuts = alpha_cuts;
}

size_t FuzzyNumber::size() const { return m_alpha_cuts.size(); }

const AlphaCut FuzzyNumber::alpha_cut( const PreciseFloat &alpha ) const
{
    if ( alpha < PreciseFloat( 0 ) || alpha > PreciseFloat( 1 ) )
    {
        throw std::out_of_range( "alpha must be in the range [0, 1]" );
    }

    auto it = m_alpha_cuts.find( AlphaCut( alpha ) );
    if ( it != m_alpha_cuts.end() )
    {
        return *it;
    }

    auto upper = m_alpha_cuts.lower_bound( AlphaCut( alpha ) );
    auto lower = std::prev( upper );

    PreciseFloat t = ( alpha - lower->alpha() ) / ( upper->alpha() - lower->alpha() );

    PreciseFloat min_value = lower->interval().min() + t * ( upper->interval().min() - lower->interval().min() );
    PreciseFloat max_value = lower->interval().max() + t * ( upper->interval().max() - lower->interval().max() );

    return AlphaCut( alpha, Interval( min_value, max_value ) );
}

PreciseFloat FuzzyNumber::min() const { return m_alpha_cuts.begin()->interval().min(); }

PreciseFloat FuzzyNumber::max() const { return m_alpha_cuts.begin()->interval().max(); }

Interval FuzzyNumber::support() const { return m_alpha_cuts.begin()->interval(); }

Interval FuzzyNumber::kernel() const { return std::prev( m_alpha_cuts.end() )->interval(); }

PreciseFloat FuzzyNumber::kernel_min() const { return kernel().min(); }

PreciseFloat FuzzyNumber::kernel_max() const { return kernel().max(); }

std::vector<PreciseFloat> FuzzyNumber::alpha_cut_values( size_t number_of_cuts )
{
    std::vector<PreciseFloat> alphas;

    size_t i = 0;

    while ( i <= number_of_cuts - 1 )
    {
        alphas.push_back( PreciseFloat( i ) / ( number_of_cuts - 1 ) );
        i += 1;
    }
    return alphas;
}

std::vector<PreciseFloat> FuzzyNumber::alphas() const
{
    std::vector<PreciseFloat> alphas;
    alphas.reserve( m_alpha_cuts.size() );
    for ( const AlphaCut &cut : m_alpha_cuts )
    {
        alphas.push_back( cut.alpha() );
    }
    return alphas;
}

std::vector<PreciseFloat> FuzzyNumber::union_of_alpha_cuts( FuzzyNumber &other ) const
{
    std::vector<PreciseFloat> other_alphas = other.alphas();

    std::vector<PreciseFloat> merged_alphas = alphas();
    merged_alphas.insert( merged_alphas.end(), other_alphas.begin(), other_alphas.end() );

    std::sort( merged_alphas.begin(), merged_alphas.end() );
    auto last = std::unique( merged_alphas.begin(), merged_alphas.end() );
    merged_alphas.erase( last, merged_alphas.end() );

    return merged_alphas;
}

FuzzyNumber FuzzyNumber::operation( const FuzzyNumber &other,
                                    const std::function<AlphaCut( const AlphaCut &, const AlphaCut & )> &op ) const
{
    std::vector<PreciseFloat> merged_alphas = union_of_alpha_cuts( const_cast<FuzzyNumber &>( other ) );
    std::set<AlphaCut> alpha_cuts;

    for ( const PreciseFloat &alpha : merged_alphas )
    {
        alpha_cuts.insert( op( alpha_cut( alpha ), other.alpha_cut( alpha ) ) );
    }

    return FuzzyNumber( alpha_cuts );
}

FuzzyNumber FuzzyNumber::apply_function( const std::function<PreciseFloat( const PreciseFloat & )> &func, bool monotone,
                                         size_t number_elements ) const
{
    std::set<AlphaCut> new_alpha_cuts;

    PreciseFloat width = max() - min();

    for ( const AlphaCut &cut : m_alpha_cuts )
    {
        size_t number_elements_cut = static_cast<size_t>( cut.interval().width() / width * number_elements );

        if ( number_elements_cut < 1 )
        {
            number_elements_cut = 1; // Ensure at least one element per cut
        }

        new_alpha_cuts.insert( cut.apply_function( func, monotone, number_elements_cut ) );
    }

    return FuzzyNumber( new_alpha_cuts );
}

bool FuzzyNumber::operator==( const FuzzyNumber &other ) const
{
    if ( m_alpha_cuts.size() != other.m_alpha_cuts.size() )
        return false;

    return m_alpha_cuts == other.m_alpha_cuts;
}

FuzzyNumber FuzzyNumber::operator-() const
{
    std::set<AlphaCut> alpha_cuts;

    for ( const AlphaCut &cut : m_alpha_cuts )
    {
        alpha_cuts.insert( -cut );
    }

    return FuzzyNumber( alpha_cuts );
}

FuzzyNumber FuzzyNumber::operator+( const FuzzyNumber &other ) const
{
    return operation( other, []( const AlphaCut &a, const AlphaCut &b )
                      { return AlphaCut( a.alpha(), a.interval() + b.interval() ); } );
}

FuzzyNumber FuzzyNumber::operator-( const FuzzyNumber &other ) const
{
    return operation( other, []( const AlphaCut &a, const AlphaCut &b )
                      { return AlphaCut( a.alpha(), a.interval() - b.interval() ); } );
}

FuzzyNumber FuzzyNumber::operator*( const FuzzyNumber &other ) const
{
    return operation( other, []( const AlphaCut &a, const AlphaCut &b )
                      { return AlphaCut( a.alpha(), a.interval() * b.interval() ); } );
}

FuzzyNumber FuzzyNumber::operator/( const FuzzyNumber &other ) const
{
    if ( other.support().contains( PreciseFloat( 0 ) ) )
    {
        throw std::domain_error( "Division by FuzzyNumber containing zero." );
    }

    return operation( other, []( const AlphaCut &a, const AlphaCut &b )
                      { return AlphaCut( a.alpha(), a.interval() / b.interval() ); } );
}

const std::string FuzzyNumber::to_string() const
{
    std::ostringstream oss;
    for ( const AlphaCut &cut : m_alpha_cuts )
    {
        oss << "(" << cut.alpha() << "; " << cut.interval().min() << ", " << cut.interval().max() << ") ";
    }
    return oss.str();
}

const std::string FuzzyNumber::to_descriptive_string() const
{
    std::ostringstream oss;
    oss << "Fuzzy number with support (" << min() << ", " << max() << "), kernel (" << kernel_min() << ", "
        << kernel_max() << ") and " << size() - 2 << " more alpha-cuts.";
    return oss.str();
}
