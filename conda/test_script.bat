@echo on

echo Running Windows test...

if not exist "%PREFIX%/Library/bin/fuzzymath.dll" exit 1
if not exist "%PREFIX%/Library/lib/fuzzymath.lib" exit 1
if not exist "%PREFIX%/Library/lib\fuzzymath_static.lib" exit 1
if not exist "%PREFIX%/Library/lib/cmake/FuzzyMath/FuzzyMathConfig.cmake" exit 1
if not exist "%PREFIX%/Library/include/FuzzyMath/FuzzyNumber.h" exit 1
