#pragma once

#include <ostream>
#include <string>

#include "AlphaCut.h"
#include "FuzzyMembership.h"
#include "FuzzyNumber.h"
#include "FuzzyNumberFactory.h"
#include "Interval.h"
#include "PossibilisticMembership.h"

using namespace FuzzyMath;

// FuzzyNumbers

std::ostream &operator<<( std::ostream &os, const FuzzyMath::FuzzyNumber &fn );

template <typename T>
    requires( std::integral<T> || std::floating_point<T> || std::same_as<T, std::string> ||
              std::is_convertible_v<T, std::string> )
FuzzyNumber operator+( const FuzzyNumber &lhs, T rhs )
{
    return lhs + FuzzyNumberFactory::crisp_number( PreciseFloat( rhs ) );
}

template <typename T>
    requires( std::integral<T> || std::floating_point<T> || std::same_as<T, std::string> ||
              std::is_convertible_v<T, std::string> )
FuzzyNumber operator-( const FuzzyNumber &lhs, T rhs )
{
    return lhs - FuzzyNumberFactory::crisp_number( PreciseFloat( rhs ) );
}

template <typename T>
    requires( std::integral<T> || std::floating_point<T> || std::same_as<T, std::string> ||
              std::is_convertible_v<T, std::string> )
FuzzyNumber operator*( const FuzzyNumber &lhs, T rhs )
{
    return lhs * FuzzyNumberFactory::crisp_number( PreciseFloat( rhs ) );
}

template <typename T>
    requires( std::integral<T> || std::floating_point<T> || std::same_as<T, std::string> ||
              std::is_convertible_v<T, std::string> )
FuzzyNumber operator/( const FuzzyNumber &lhs, T rhs )
{
    return lhs / FuzzyNumberFactory::crisp_number( PreciseFloat( rhs ) );
}

template <typename T>
    requires( std::integral<T> || std::floating_point<T> || std::same_as<T, std::string> ||
              std::is_convertible_v<T, std::string> )
FuzzyNumber operator+( T lhs, const FuzzyNumber &rhs )
{
    return FuzzyNumberFactory::crisp_number( PreciseFloat( lhs ) ) + rhs;
}

template <typename T>
    requires( std::integral<T> || std::floating_point<T> || std::same_as<T, std::string> ||
              std::is_convertible_v<T, std::string> )
FuzzyNumber operator-( T lhs, const FuzzyNumber &rhs )
{
    return FuzzyNumberFactory::crisp_number( PreciseFloat( lhs ) ) - rhs;
}

template <typename T>
    requires( std::integral<T> || std::floating_point<T> || std::same_as<T, std::string> ||
              std::is_convertible_v<T, std::string> )
FuzzyNumber operator*( T lhs, const FuzzyNumber &rhs )
{
    return FuzzyNumberFactory::crisp_number( PreciseFloat( lhs ) ) * rhs;
}

template <typename T>
    requires( std::integral<T> || std::floating_point<T> || std::same_as<T, std::string> ||
              std::is_convertible_v<T, std::string> )
FuzzyNumber operator/( T lhs, const FuzzyNumber &rhs )
{
    return FuzzyNumberFactory::crisp_number( PreciseFloat( lhs ) ) / rhs;
}

// Interval

std::ostream &operator<<( std::ostream &os, const Interval &i );

template <typename T>
    requires( std::integral<T> || std::floating_point<T> || std::same_as<T, std::string> ||
              std::is_convertible_v<T, std::string> )
Interval operator+( T lhs, const Interval &rhs )
{
    return Interval( PreciseFloat( lhs ) ) + rhs;
}

template <typename T>
    requires( std::integral<T> || std::floating_point<T> || std::same_as<T, std::string> ||
              std::is_convertible_v<T, std::string> )
Interval operator-( T lhs, const Interval &rhs )
{
    return Interval( PreciseFloat( lhs ) ) - rhs;
}

template <typename T>
    requires( std::integral<T> || std::floating_point<T> || std::same_as<T, std::string> ||
              std::is_convertible_v<T, std::string> )
Interval operator*( T lhs, const Interval &rhs )
{
    return Interval( PreciseFloat( lhs ) ) * rhs;
}

template <typename T>
    requires( std::integral<T> || std::floating_point<T> || std::same_as<T, std::string> ||
              std::is_convertible_v<T, std::string> )
Interval operator/( T lhs, const Interval &rhs )
{
    return Interval( PreciseFloat( lhs ) ) / rhs;
}

template <typename T>
    requires( std::integral<T> || std::floating_point<T> || std::same_as<T, std::string> ||
              std::is_convertible_v<T, std::string> )
Interval operator+( const Interval &lhs, T rhs )

{
    return lhs + Interval( PreciseFloat( rhs ) );
}

template <typename T>
    requires( std::integral<T> || std::floating_point<T> || std::same_as<T, std::string> ||
              std::is_convertible_v<T, std::string> )
Interval operator-( const Interval &lhs, T rhs )
{
    return lhs - Interval( PreciseFloat( rhs ) );
}

template <typename T>
    requires( std::integral<T> || std::floating_point<T> || std::same_as<T, std::string> ||
              std::is_convertible_v<T, std::string> )
Interval operator*( const Interval &lhs, T rhs )
{
    return lhs * Interval( PreciseFloat( rhs ) );
}

template <typename T>
    requires( std::integral<T> || std::floating_point<T> || std::same_as<T, std::string> ||
              std::is_convertible_v<T, std::string> )
Interval operator/( const Interval &lhs, T rhs )
{
    return lhs / Interval( PreciseFloat( rhs ) );
}

// AlphaCut

std::ostream &operator<<( std::ostream &os, const AlphaCut &ac );

// FuzzyMembership

std::ostream &operator<<( std::ostream &os, const FuzzyMembership &m );

// PossibilisticMembership
std::ostream &operator<<( std::ostream &os, const PossibilisticMembership &pm );
