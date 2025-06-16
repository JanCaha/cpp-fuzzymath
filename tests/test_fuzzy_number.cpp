#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cmath>
#include <limits>

#include <Functions.h>
#include <FuzzyNumber.h>
#include <FuzzyNumberFactory.h>

#include "gtest_extensions.h"

using namespace FuzzyMath;

class FuzzyNumbersTests : public ::testing::Test
{
  protected:
    FuzzyNumber fn_a = FuzzyNumberFactory::triangular( 1, 2, 3 );
    FuzzyNumber fn_b = FuzzyNumberFactory::triangular( 2, 3, 4 );
    FuzzyNumber fn_c = FuzzyNumberFactory::triangular( -1, 0, 1 );
    FuzzyNumber fn_d = FuzzyNumberFactory::trapezoidal( 1, 2, 3, 4 );
    FuzzyNumber fn_e = FuzzyNumberFactory::triangular( 1, 2, 3, 6 );
};

TEST( FuzzyNumbersTests_Standalone, CreationFail )
{
    std::set<AlphaCut> alphas_cuts_1 = { AlphaCut( 0, Interval( 1, 1 ) ) };
    std::set<AlphaCut> alphas_cuts_2 = { AlphaCut( 0.1, Interval( 1, 1 ) ), AlphaCut( 1, Interval( 1.5, 1.5 ) ) };
    std::set<AlphaCut> alphas_cuts_3 = { AlphaCut( 0, Interval( 1, 1 ) ), AlphaCut( 0.5, Interval( 1.5, 1.5 ) ) };
    std::set<AlphaCut> alphas_cuts_4 = { AlphaCut( 0, Interval( 1, 1 ) ), AlphaCut( 1, Interval( 1.5, 1.5 ) ) };

    EXPECT_THROW( FuzzyNumber{ alphas_cuts_1 }, std::invalid_argument );
    EXPECT_THROW( FuzzyNumber{ alphas_cuts_2 }, std::invalid_argument );
    EXPECT_THROW( FuzzyNumber{ alphas_cuts_3 }, std::invalid_argument );
    EXPECT_THROW( FuzzyNumber{ alphas_cuts_4 }, std::invalid_argument );
}

TEST_F( FuzzyNumbersTests, AlphaCut )
{
    EXPECT_EQ( fn_a.alpha_cut( 0 ).interval(), Interval( 1, 3 ) );
    EXPECT_EQ( fn_a.alpha_cut( 0.25 ).interval(), Interval( 1.25, 2.75 ) );
    EXPECT_EQ( fn_a.alpha_cut( 0.5 ).interval(), Interval( 1.5, 2.5 ) );
    EXPECT_EQ( fn_a.alpha_cut( 0.75 ).interval(), Interval( 1.75, 2.25 ) );
    EXPECT_EQ( fn_a.alpha_cut( 1 ).interval(), Interval( 2, 2 ) );

    EXPECT_THROW( fn_a.alpha_cut( -0.1 ), std::out_of_range );
    EXPECT_THROW( fn_a.alpha_cut( 1.1 ), std::out_of_range );
}

TEST_F( FuzzyNumbersTests, GetAlphaCutsValues )
{
    std::vector<PreciseFloat> alphas = fn_a.alphas();
    EXPECT_EQ( alphas.size(), 2 );
    EXPECT_EQ( alphas[0], 0 );
    EXPECT_EQ( alphas[1], 1 );

    alphas = fn_e.alphas();
    EXPECT_EQ( alphas.size(), 6 );
    EXPECT_EQ( alphas[0], PreciseFloat( 0 ) );
    EXPECT_EQ( alphas[1], PreciseFloat( "0.2" ) );
    EXPECT_EQ( alphas[2], PreciseFloat( "0.4" ) );
    EXPECT_EQ( alphas[3], PreciseFloat( "0.6" ) );
    EXPECT_EQ( alphas[4], PreciseFloat( "0.8" ) );
    EXPECT_EQ( alphas[5], PreciseFloat( "1" ) );
}

TEST_F( FuzzyNumbersTests, Equality )
{
    EXPECT_EQ( fn_a, FuzzyNumberFactory::triangular( 1, 2, 3 ) );
    EXPECT_EQ( fn_a, fn_a );
    EXPECT_EQ( fn_a, fn_a + FuzzyNumberFactory::crisp_number( 0.0 ) );

    EXPECT_NE( fn_a, fn_b );
    EXPECT_NE( fn_a, -fn_a );
    EXPECT_NE( fn_a, fn_e );
}

TEST_F( FuzzyNumbersTests, Negation )
{
    EXPECT_EQ( -fn_a, FuzzyNumberFactory::triangular( -3, -2, -1 ) );
    EXPECT_EQ( -fn_b, FuzzyNumberFactory::triangular( -4, -3, -2 ) );
    EXPECT_EQ( -fn_c, FuzzyNumberFactory::triangular( -1, 0, 1 ) );
    EXPECT_EQ( -fn_d, FuzzyNumberFactory::trapezoidal( -4, -3, -2, -1 ) );
}

TEST_F( FuzzyNumbersTests, Addition )
{
    EXPECT_EQ( fn_a + fn_b, FuzzyNumberFactory::triangular( 3, 5, 7 ) );
    EXPECT_EQ( fn_b + fn_a, FuzzyNumberFactory::triangular( 3, 5, 7 ) );
    EXPECT_EQ( fn_a + fn_c, FuzzyNumberFactory::triangular( 0, 2, 4 ) );

    EXPECT_EQ( fn_a + 1, FuzzyNumberFactory::triangular( 2, 3, 4 ) );
    EXPECT_EQ( fn_a + 1.5, FuzzyNumberFactory::triangular( 2.5, 3.5, 4.5 ) );
    EXPECT_EQ( fn_a + "1.5", FuzzyNumberFactory::triangular( 2.5, 3.5, 4.5 ) );

    EXPECT_EQ( 1 + fn_a, FuzzyNumberFactory::triangular( 2, 3, 4 ) );
    EXPECT_EQ( 1.5 + fn_a, FuzzyNumberFactory::triangular( 2.5, 3.5, 4.5 ) );
    EXPECT_EQ( "1.5" + fn_a, FuzzyNumberFactory::triangular( 2.5, 3.5, 4.5 ) );

    EXPECT_THROW( fn_a + "invalid", std::runtime_error );
    EXPECT_THROW( "invalid" + fn_a, std::runtime_error );
}

TEST_F( FuzzyNumbersTests, Substraction )
{
    EXPECT_EQ( fn_a - fn_b, FuzzyNumberFactory::triangular( -3, -1, 1 ) );
    EXPECT_EQ( fn_b - fn_a, FuzzyNumberFactory::triangular( -1, 1, 3 ) );
    EXPECT_EQ( fn_a - fn_c, FuzzyNumberFactory::triangular( 0, 2, 4 ) );

    EXPECT_EQ( fn_a - 1, FuzzyNumberFactory::triangular( 0, 1, 2 ) );
    EXPECT_EQ( fn_a - 1.5, FuzzyNumberFactory::triangular( -0.5, 0.5, 1.5 ) );
    EXPECT_EQ( fn_a - "1.5", FuzzyNumberFactory::triangular( -0.5, 0.5, 1.5 ) );

    EXPECT_EQ( 1 - fn_a, FuzzyNumberFactory::triangular( -2, -1, 0 ) );
    EXPECT_EQ( 1.5 - fn_a, FuzzyNumberFactory::triangular( -1.5, -0.5, 0.5 ) );
    EXPECT_EQ( "1.5" - fn_a, FuzzyNumberFactory::triangular( -1.5, -0.5, 0.5 ) );
}

TEST_F( FuzzyNumbersTests, Division )
{
    EXPECT_EQ( fn_a / 2, FuzzyNumberFactory::triangular( 0.5, 1, 1.5 ) );
    EXPECT_EQ( fn_a / 2.0, FuzzyNumberFactory::triangular( 0.5, 1, 1.5 ) );
    EXPECT_EQ( fn_a / "2", FuzzyNumberFactory::triangular( 0.5, 1, 1.5 ) );

    EXPECT_EQ( fn_a / fn_b,
               FuzzyNumberFactory::triangular( PreciseFloat( "0.25" ), PreciseFloat( 2 ) / PreciseFloat( 3 ),
                                               PreciseFloat( "1.5" ) ) );

    FuzzyNumber a = FuzzyNumberFactory::triangular( 5 / fn_a.max(), 5 / fn_a.kernel_min(), 5 / fn_a.min() );
    EXPECT_EQ( 5 / fn_a, FuzzyNumberFactory::triangular( 5 / fn_a.max(), 5 / fn_a.kernel_min(), 5 / fn_a.min() ) );
    EXPECT_EQ( 5.0 / fn_a, FuzzyNumberFactory::triangular( 5 / fn_a.max(), 5 / fn_a.kernel_min(), 5 / fn_a.min() ) );
    EXPECT_EQ( "5.0" / fn_a, FuzzyNumberFactory::triangular( 5 / fn_a.max(), 5 / fn_a.kernel_min(), 5 / fn_a.min() ) );

    EXPECT_THROW( fn_a / 0, std::domain_error );
}

TEST_F( FuzzyNumbersTests, Function )
{
    FuzzyNumber fn = FuzzyNumberFactory::triangular( -M_PI / 2, 0, M_PI / 2, 11 );

    auto func_cos = []( PreciseFloat x ) -> PreciseFloat { return std::cos( x.convert_to<long double>() ); };

    FuzzyNumber res = fn.apply_function( func_cos );

    EXPECT_NEAR( res.min().convert_to<long double>(), -0.0, 1e-15 );
    EXPECT_NEAR( res.max().convert_to<long double>(), 1.0, 1e-15 );
    EXPECT_NEAR( res.kernel_min().convert_to<long double>(), 1.0, 1e-15 );
    EXPECT_NEAR( res.kernel_max().convert_to<long double>(), 1.0, 1e-15 );

    auto func_sin = []( PreciseFloat x ) -> PreciseFloat { return std::sin( x.convert_to<long double>() ); };

    FuzzyNumber res1 = fn.apply_function( func_sin );

    EXPECT_NEAR( res1.min().convert_to<long double>(), -1.0, 1e-15 );
    EXPECT_NEAR( res1.max().convert_to<long double>(), 1.0, 1e-15 );
    EXPECT_NEAR( res1.kernel_min().convert_to<long double>(), 0.0, 1e-15 );
    EXPECT_NEAR( res1.kernel_max().convert_to<long double>(), 0.0, 1e-15 );

    auto func_pow = []( PreciseFloat x ) -> PreciseFloat { return x * x; };

    FuzzyNumber res2 = fn_a.apply_function( func_pow, true );

    EXPECT_EQ( res2.min(), PreciseFloat( 1 ) );
    EXPECT_EQ( res2.max(), PreciseFloat( 9 ) );
    EXPECT_EQ( res2.kernel_min(), PreciseFloat( 4 ) );
    EXPECT_EQ( res2.kernel_max(), PreciseFloat( 4 ) );
}

int main( int argc, char **argv )
{
    testing::InitGoogleTest( &argc, argv );
    return RUN_ALL_TESTS();
}