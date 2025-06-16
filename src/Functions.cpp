#include "Functions.h"

using namespace FuzzyMath;

std::ostream &operator<<( std::ostream &os, const FuzzyNumber &fn )
{
    os << fn.to_descriptive_string();
    return os;
}

std::ostream &operator<<( std::ostream &os, const Interval &i )
{
    os << i.to_string();
    return os;
}

std::ostream &operator<<( std::ostream &os, const FuzzyMembership &m )
{
    os << m.to_string();
    return os;
}

std::ostream &operator<<( std::ostream &os, const AlphaCut &ac )
{
    os << ac.to_string();
    return os;
}

std::ostream &operator<<( std::ostream &os, const PossibilisticMembership &pm )
{
    os << "(possibility=" << pm.possibility() << ", necessity=" << pm.necessity() << ")";
    return os;
}
