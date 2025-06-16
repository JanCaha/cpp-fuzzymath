#include "FuzzyNumberFactory.h"
#include "AlphaCut.h"

using namespace FuzzyMath;

FuzzyNumber FuzzyNumberFactory::triangular( PreciseFloat minimum, PreciseFloat kernel, PreciseFloat maximum,
                                            int number_of_cuts )
{
    if ( !( minimum <= kernel && kernel <= maximum ) )
    {
        throw std::invalid_argument(
            "The fuzzy number is invalid. The structure needs to be `minimum` <= `kernel` <= `maximum`." );
    }

    if ( number_of_cuts < 2 )
    {
        throw std::invalid_argument( "Number of cuts must be at least 2." );
    }

    std::set<AlphaCut> alpha_cuts;

    if ( number_of_cuts == 2 )
    {
        alpha_cuts.emplace( 0, Interval( minimum, maximum ) );
        alpha_cuts.emplace( 1, Interval( kernel ) );
    }
    else
    {
        std::vector<PreciseFloat> alphas = FuzzyNumber::alpha_cut_values( number_of_cuts );

        size_t i = 0;

        for ( PreciseFloat alpha : alphas )
        {
            if ( alpha == 0 )
            {
                alpha_cuts.emplace( 0, Interval( minimum, maximum ) );
            }
            else if ( alpha == 1 )
            {
                alpha_cuts.emplace( 1, Interval( kernel, kernel ) );
            }
            else
            {
                PreciseFloat int_min = ( ( kernel - minimum ) / ( number_of_cuts - 1 ) ) * i + minimum;
                PreciseFloat int_max = maximum - ( ( maximum - kernel ) / ( number_of_cuts - 1 ) ) * i;
                alpha_cuts.emplace( alpha, Interval( int_min, int_max ) );
            }
        }
    }
    return FuzzyNumber( alpha_cuts );
}

FuzzyNumber FuzzyNumberFactory::triangular( std::string minimum, std::string kernel, std::string maximum,
                                            int number_of_cuts )
{
    return triangular( PreciseFloat( minimum ), PreciseFloat( kernel ), PreciseFloat( maximum ), number_of_cuts );
}

FuzzyNumber FuzzyNumberFactory::trapezoidal( PreciseFloat minimum, PreciseFloat kernel_minimum,
                                             PreciseFloat kernel_maximum, PreciseFloat maximum, int number_of_cuts )
{
    if ( !( minimum <= kernel_minimum && kernel_minimum <= kernel_maximum && kernel_maximum <= maximum ) )
    {
        throw std::invalid_argument(
            "The fuzzy number is invalid. The structure needs to be `minimum` <= `kernel_minimum` "
            "<= `kernel_maximum` <= `maximum`." );
    }

    if ( number_of_cuts < 2 )
    {
        throw std::invalid_argument( "Number of cuts must be at least 2." );
    }

    std::set<AlphaCut> alpha_cuts;

    if ( number_of_cuts == 2 )
    {
        alpha_cuts.emplace( 0, Interval( minimum, maximum ) );
        alpha_cuts.emplace( 1, Interval( minimum, maximum ) );
    }
    else
    {
        std::vector<PreciseFloat> alphas = FuzzyNumber::alpha_cut_values( number_of_cuts );

        size_t i = 0;

        for ( PreciseFloat alpha : alphas )
        {
            if ( alpha == 0 )
            {
                alpha_cuts.emplace( 0, Interval( minimum, maximum ) );
            }
            else if ( alpha == 1 )
            {
                alpha_cuts.emplace( 1, Interval( kernel_minimum, kernel_maximum ) );
            }
            else
            {
                PreciseFloat int_min = ( ( kernel_minimum - minimum ) / ( number_of_cuts - 1 ) ) * i + minimum;
                PreciseFloat int_max = maximum - ( ( maximum - kernel_maximum ) / ( number_of_cuts - 1 ) ) * i;
                alpha_cuts.emplace( alpha, Interval( int_min, int_max ) );
            }
        }
    }

    return FuzzyNumber( alpha_cuts );
}

FuzzyNumber FuzzyNumberFactory::trapezoidal( std::string minimum, std::string kernel_minimum,
                                             std::string kernel_maximum, std::string maximum, int number_of_cuts )
{
    return trapezoidal( PreciseFloat( minimum ), PreciseFloat( kernel_minimum ), PreciseFloat( kernel_maximum ),
                        PreciseFloat( maximum ), number_of_cuts );
}

FuzzyNumber FuzzyNumberFactory::crisp_number( PreciseFloat value )
{
    std::set<AlphaCut> alpha_cuts;

    alpha_cuts.emplace( 0, Interval( value ) );
    alpha_cuts.emplace( 1, Interval( value ) );

    return FuzzyNumber( alpha_cuts );
}

FuzzyNumber FuzzyNumberFactory::crisp_number( std::string value ) { return crisp_number( PreciseFloat( value ) ); }