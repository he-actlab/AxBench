gcc -dM -E -mavx - < /dev/null | grep -i avx
clang -dM -E -mavx - < /dev/null | grep -i avx
#define __AVX__ 1
