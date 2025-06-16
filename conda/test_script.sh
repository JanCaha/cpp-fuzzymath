#!/bin/bash
set -euo pipefail

echo "Running Unix test..."

# Function to check if a file exists
check_file_exists() {
  local filepath="$1"
  if [[ -f "$filepath" ]]; then
    echo "✅ File exists: $filepath"
  else
    echo "❌ Missing file: $filepath"
    exit 1
  fi
}

# existence of files
check_file_exists "$PREFIX/lib/libfuzzymath.so"
check_file_exists "$PREFIX/lib/libfuzzymath.a"
check_file_exists "$PREFIX/lib/cmake/FuzzyMath/FuzzyMathConfig.cmake"
check_file_exists "$PREFIX/include/FuzzyMath/FuzzyNumber.h"

