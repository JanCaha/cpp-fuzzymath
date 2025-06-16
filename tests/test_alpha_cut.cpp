#include <algorithm>
#include <set>
#include <sstream>
#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "AlphaCut.h"
#include "Functions.h"

#include "gtest_extensions.h"

using namespace FuzzyMath;

TEST( AlphaCut, CreationFail )
{
    EXPECT_THROW( AlphaCut( PreciseFloat( -0.1 ), Interval( PreciseFloat( 1 ), PreciseFloat( 2 ) ) ),
                  std::out_of_range );
    EXPECT_THROW( AlphaCut( PreciseFloat( 1.1 ), Interval( PreciseFloat( 1 ), PreciseFloat( 2 ) ) ),
                  std::out_of_range );
    EXPECT_THROW( AlphaCut( PreciseFloat( 0.5 ), Interval() ), std::invalid_argument );
}

TEST( AlphaCut, Creation )
{
    AlphaCut a( PreciseFloat( 0.5 ), Interval( PreciseFloat( 1 ), PreciseFloat( 2 ) ) );
    EXPECT_EQ( a.alpha(), PreciseFloat( 0.5 ) );
    EXPECT_EQ( a.interval(), Interval( PreciseFloat( 1 ), PreciseFloat( 2 ) ) );
}

// comparison by alpha value
TEST( AlphaCut, Comparison )
{
    AlphaCut a( PreciseFloat( 0.5 ), Interval( PreciseFloat( 1 ), PreciseFloat( 2 ) ) );
    AlphaCut b( PreciseFloat( 0.7 ), Interval( PreciseFloat( 2 ), PreciseFloat( 3 ) ) );
    AlphaCut c( PreciseFloat( 0.5 ), Interval( PreciseFloat( 1.5 ), PreciseFloat( 2.5 ) ) );

    EXPECT_TRUE( a < b );
    EXPECT_FALSE( a > b );

    EXPECT_TRUE( b > a );
    EXPECT_FALSE( b < a );

    auto res = a.operator<=>( c );
    EXPECT_EQ( res, std::weak_ordering::equivalent );
    res = a.operator<=>( b );
    EXPECT_EQ( res, std::weak_ordering::less );
    res = b.operator<=>( a );
    EXPECT_EQ( res, std::weak_ordering::greater );
}

// sorting by alpha value
TEST( AlphaCut, Sorting )
{
    std::vector<AlphaCut> cuts;
    cuts.push_back( AlphaCut( PreciseFloat( 0.8 ), Interval( PreciseFloat( 1 ), PreciseFloat( 2 ) ) ) );
    cuts.push_back( AlphaCut( PreciseFloat( 0.3 ), Interval( PreciseFloat( 1 ), PreciseFloat( 2 ) ) ) );
    cuts.push_back( AlphaCut( PreciseFloat( 0.5 ), Interval( PreciseFloat( 1.5 ), PreciseFloat( 2.5 ) ) ) );
    cuts.push_back( AlphaCut( PreciseFloat( 0.8 ), Interval( PreciseFloat( 5 ), PreciseFloat( 5 ) ) ) );

    std::sort( cuts.begin(), cuts.end() );

    EXPECT_EQ( cuts[0].alpha(), PreciseFloat( 0.3 ) );
    EXPECT_EQ( cuts[1].alpha(), PreciseFloat( 0.5 ) );
    EXPECT_EQ( cuts[2].alpha(), PreciseFloat( 0.8 ) );
    EXPECT_EQ( cuts[3].alpha(), PreciseFloat( 0.8 ) );

    EXPECT_EQ( cuts[0].interval(), Interval( PreciseFloat( 1 ), PreciseFloat( 2 ) ) );
    EXPECT_EQ( cuts[1].interval(), Interval( PreciseFloat( 1.5 ), PreciseFloat( 2.5 ) ) );
    EXPECT_EQ( cuts[2].interval(), Interval( PreciseFloat( 1 ), PreciseFloat( 2 ) ) );
    EXPECT_EQ( cuts[3].interval(), Interval( PreciseFloat( 5 ), PreciseFloat( 5 ) ) );
}

// sorting by alpha value
TEST( AlphaCut, Set )
{
    std::set<AlphaCut> cuts;
    cuts.insert( AlphaCut( "0.8", Interval( 1, 2 ) ) );
    cuts.insert( AlphaCut( "0.3", Interval( 1, 2 ) ) );
    cuts.insert( AlphaCut( "0.5", Interval( "1.5", "2.5" ) ) );
    cuts.insert( AlphaCut( "0.8", Interval( 5 ) ) );

    EXPECT_EQ( cuts.find( AlphaCut( "0.3" ) )->alpha(), PreciseFloat( "0.3" ) );
    EXPECT_EQ( cuts.find( AlphaCut( "0.5" ) )->alpha(), PreciseFloat( "0.5" ) );
    EXPECT_EQ( cuts.find( AlphaCut( "0.8" ) )->alpha(), PreciseFloat( "0.8" ) );

    EXPECT_EQ( cuts.find( AlphaCut( "0.3" ) )->interval(), Interval( 1, 2 ) );
    EXPECT_EQ( cuts.find( AlphaCut( "0.5" ) )->interval(), Interval( "1.5", "2.5" ) );
    EXPECT_EQ( cuts.find( AlphaCut( "0.8" ) )->interval(), Interval( 1, 2 ) );
}
