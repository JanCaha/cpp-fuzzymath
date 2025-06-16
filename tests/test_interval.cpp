#include <sstream>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "Functions.h"
#include "Interval.h"

#include "gtest_extensions.h"

using namespace FuzzyMath;

class IntervalTests : public ::testing::Test
{
  protected:
    Interval a = Interval( 1, 3 );
    Interval b = Interval( 2, 5 );
    Interval c = Interval( 4, 7 );
    Interval d = Interval( -2, 3 );
    Interval e = Interval( -1, 1 );
    Interval f = Interval();
};

TEST( Interval, Creation )
{
    // default constructor with PreciseFloat
    Interval i = Interval( PreciseFloat( 0 ), PreciseFloat( 1 ) );
    i = Interval( PreciseFloat( 0.1 ), PreciseFloat( 1.1 ) );
    i = Interval( PreciseFloat( "0.1" ), PreciseFloat( "1.1" ) );

    i = Interval( PreciseFloat( -1 ), PreciseFloat( 1 ) );
    i = Interval( PreciseFloat( -1.1 ), PreciseFloat( 1.1 ) );
    i = Interval( PreciseFloat( "-1.1" ), PreciseFloat( "1.1" ) );

    i = Interval( PreciseFloat( -1.1 * 1e15 ), PreciseFloat( 1.1 * 1e15 ) );

    // conversions to PreciseFloat
    i = Interval( 1, 2 );
    i = Interval( 1.1, 2.2 );
    i = Interval( "1.1", "2.2" );
}

TEST( Interval, CreationFail )
{
    EXPECT_THROW( Interval d = Interval( PreciseFloat( "a" ), PreciseFloat( "b" ) ), std::exception );
    EXPECT_THROW( Interval d = Interval( PreciseFloat( "a" ), PreciseFloat( 1 ) ), std::exception );
    EXPECT_THROW( Interval d = Interval( PreciseFloat( 2 ), PreciseFloat( "b" ) ), std::exception );

    EXPECT_THROW( Interval d = Interval( "a", "1" ), std::exception );
    EXPECT_THROW( Interval d = Interval( "1", "b" ), std::exception );
    EXPECT_THROW( Interval d = Interval( "a", "b" ), std::exception );
}

TEST( Interval, ToString )
{
    std::ostringstream oss;
    Interval i = Interval( PreciseFloat( 0 ), PreciseFloat( 1 ) );

    oss << i;
    EXPECT_EQ( oss.str(), "[0, 1]" );

    i = Interval( PreciseFloat( 0.1 ), PreciseFloat( 1.1 ) );

    oss.str( "" );
    oss << i;
    EXPECT_EQ( oss.str(), "[0.1, 1.1]" );

    i = Interval( PreciseFloat( "0.1" ), PreciseFloat( "1.1" ) );

    oss.str( "" );
    oss << i;
    EXPECT_EQ( oss.str(), "[0.1, 1.1]" );

    i = Interval( PreciseFloat( "-1.1" ), PreciseFloat( "1.1" ) );

    oss.str( "" );
    oss << i;
    EXPECT_EQ( oss.str(), "[-1.1, 1.1]" );

    i = Interval( PreciseFloat( "-1.1e-3" ), PreciseFloat( "1.1e-3" ) );

    oss.str( "" );
    oss << i;
    EXPECT_EQ( oss.str(), "[-0.0011, 0.0011]" );
}

TEST( Interval, Degenerate )
{
    EXPECT_TRUE( Interval( 2, 2 ).is_degenerate() );
    EXPECT_FALSE( Interval( 2, 3 ).is_degenerate() );
}

TEST_F( IntervalTests, Midpoint )
{
    EXPECT_EQ( a.mid_point(), PreciseFloat( 2 ) );
    EXPECT_EQ( b.mid_point(), PreciseFloat( 3.5 ) );
    EXPECT_EQ( e.mid_point(), PreciseFloat( 0 ) );
}

TEST( Interval, Contains )
{
    Interval i = Interval( 1, 5 );
    EXPECT_TRUE( i.contains( 2 ) );
    EXPECT_TRUE( i.contains( 1 ) );
    EXPECT_TRUE( i.contains( 3.59 ) );
    EXPECT_FALSE( i.contains( 0.999999 ) );
    EXPECT_FALSE( i.contains( 5.0001 ) );

    EXPECT_FALSE( i.contains( Interval( 6, 7 ) ) );
    EXPECT_FALSE( i.contains( Interval( 0, 3 ) ) );

    EXPECT_TRUE( i.contains( Interval( 2, 4 ) ) );
    EXPECT_TRUE( i.contains( Interval( 2.1, 4.1 ) ) );
    EXPECT_TRUE( i.contains( Interval( "2", "4" ) ) );
}

TEST_F( IntervalTests, Intersects )
{
    EXPECT_TRUE( a.intersects( b ) );
    EXPECT_TRUE( b.intersects( a ) );
    EXPECT_TRUE( b.intersects( c ) );
    EXPECT_TRUE( c.intersects( b ) );

    EXPECT_FALSE( a.intersects( c ) );
    EXPECT_FALSE( c.intersects( a ) );
}

TEST_F( IntervalTests, Intersection )
{
    ASSERT_EQ( a.intersection( b ), Interval( 2, 3 ) );
    ASSERT_EQ( b.intersection( c ), Interval( 4, 5 ) );

    ASSERT_THROW( a.intersection( c ), std::domain_error );
}

TEST_F( IntervalTests, Unite )
{
    ASSERT_EQ( a.unite( b ), Interval( 1, 5 ) );
    ASSERT_EQ( b.unite( c ), Interval( 2, 7 ) );

    ASSERT_THROW( a.unite( c ), std::domain_error );
}

TEST_F( IntervalTests, Add )
{
    Interval r = a + 1;
    ASSERT_EQ( r.min(), a.min() + 1 );
    ASSERT_EQ( r.max(), a.max() + 1 );

    r = a + 1.5;
    ASSERT_EQ( r.min(), a.min() + 1.5 );
    ASSERT_EQ( r.max(), a.max() + 1.5 );

    r = a + "2.5";
    ASSERT_EQ( r.min(), a.min() + PreciseFloat( "2.5" ) );
    ASSERT_EQ( r.max(), a.max() + PreciseFloat( "2.5" ) );

    r = 1 + a;
    ASSERT_EQ( r.min(), 1 + a.min() );
    ASSERT_EQ( r.max(), 1 + a.max() );

    r = a + b;
    ASSERT_EQ( r.min(), a.min() + b.min() );
    ASSERT_EQ( r.max(), a.max() + b.max() );

    r = a + e;
    ASSERT_EQ( r.min(), a.min() + e.min() );
    ASSERT_EQ( r.max(), a.max() + e.max() );
}

TEST_F( IntervalTests, Sub )
{
    Interval r = a - 1;
    ASSERT_EQ( r.min(), a.min() - 1 );
    ASSERT_EQ( r.max(), a.max() - 1 );

    r = a - 1.5;
    ASSERT_EQ( r.min(), a.min() - 1.5 );
    ASSERT_EQ( r.max(), a.max() - 1.5 );

    r = 1 - a;
    ASSERT_EQ( r.min(), 1 - a.max() );
    ASSERT_EQ( r.max(), 1 - a.min() );

    r = a - b;
    ASSERT_EQ( r.min(), a.min() - b.max() );
    ASSERT_EQ( r.max(), a.max() - b.min() );
}

TEST_F( IntervalTests, Multiply )
{
    Interval r = a * 2;
    ASSERT_EQ( r.min(), a.min() * 2 );
    ASSERT_EQ( r.max(), a.max() * 2 );

    r = 2 * a;
    ASSERT_EQ( r.min(), 2 * a.min() );
    ASSERT_EQ( r.max(), 2 * a.max() );

    r = a * b;
    ASSERT_EQ( r.min(), a.min() * b.min() );
    ASSERT_EQ( r.max(), a.max() * b.max() );
}

TEST_F( IntervalTests, Divide )
{
    Interval r = a / 2;
    ASSERT_EQ( r.min(), a.min() / 2 );
    ASSERT_EQ( r.max(), a.max() / 2 );

    r = 2 / a;
    ASSERT_EQ( r.min(), 2 / a.max() );
    ASSERT_EQ( r.max(), 2 / a.min() );

    r = a / b;
    ASSERT_EQ( r.min(), a.min() / b.max() );
    ASSERT_EQ( r.max(), a.max() / b.min() );

    ASSERT_THROW( a / 0.0, std::domain_error );
    ASSERT_THROW( a / d, std::domain_error );
}

TEST_F( IntervalTests, apply_function )
{
    auto square = []( PreciseFloat x ) -> PreciseFloat { return x * x; };

    Interval r = a.apply_function( square, true );

    ASSERT_EQ( r.min(), square( a.min() ) );
    ASSERT_EQ( r.max(), square( a.max() ) );

    auto sin = []( PreciseFloat x ) -> PreciseFloat
    {
        long double value = x.convert_to<long double>();
        return std::sin( value );
    };

    r = b.apply_function( sin );

    ASSERT_NEAR( r.min_as_double(), -1, 1e-7 );
    ASSERT_NEAR( r.max_as_double(), 0.90929742682568171, 1e-7 );

    auto log2 = []( PreciseFloat x ) -> PreciseFloat
    {
        if ( x <= 0 )
            throw std::domain_error( "Logarithm undefined for non-positive values" );
        return std::log2( x.convert_to<long double>() );
    };

    r = a.apply_function( log2, true );
    ASSERT_NEAR( r.min_as_double(), std::log2( a.min_as_double() ), 1e-7 );
    ASSERT_NEAR( r.max_as_double(), std::log2( a.max_as_double() ), 1e-7 );

    auto cos = []( PreciseFloat x ) -> PreciseFloat
    {
        long double value = x.convert_to<long double>();
        return std::cos( value );
    };

    r = b.apply_function( cos );
    ASSERT_NEAR( r.min_as_double(), -0.99999900968820432, 1e-7 );
    ASSERT_NEAR( r.max_as_double(), 0.28366218546322625, 1e-7 );
}

TEST_F( IntervalTests, Negative )
{
    Interval r = -a;
    ASSERT_EQ( r.min(), -a.max() );
    ASSERT_EQ( r.max(), -a.min() );

    r = -d;
    ASSERT_EQ( r.min(), -d.max() );
    ASSERT_EQ( r.max(), -d.min() );
}

TEST_F( IntervalTests, Equality )
{
    EXPECT_EQ( a, a );
    EXPECT_NE( a, b );
    EXPECT_NE( a, c );
    EXPECT_NE( a, d );
}

TEST_F( IntervalTests, LessThan )
{
    EXPECT_TRUE( a < c );
    EXPECT_TRUE( d < c );
    EXPECT_FALSE( a < b );
}

TEST_F( IntervalTests, GreaterThan )
{
    EXPECT_TRUE( c > a );
    EXPECT_TRUE( c > d );
    EXPECT_FALSE( b > a );
}

TEST_F( IntervalTests, IsEmpty )
{
    EXPECT_FALSE( a.is_empty() );
    EXPECT_FALSE( b.is_empty() );
    EXPECT_TRUE( f.is_empty() );
}

TEST_F( IntervalTests, Power )
{
    Interval r = a.power( 2 );
    ASSERT_EQ( r.min(), 1 );
    ASSERT_EQ( r.max(), 9 );

    r = a.power( 3 );
    ASSERT_EQ( r.min(), 1 );
    ASSERT_EQ( r.max(), 27 );

    r = d.power( 2 );
    ASSERT_EQ( r.min(), 0 );
    ASSERT_EQ( r.max(), 9 );

    r = d.power( 3 );
    ASSERT_EQ( r.min(), -8 );
    ASSERT_EQ( r.max(), 27 );
}
