#include <cstdint>

typedef int8_t	i8;
typedef int16_t	i16;
typedef int32_t	i32;
typedef int64_t	i64;

typedef uint8_t		u8;
typedef uint16_t	u16;
typedef uint32_t	u32;
typedef uint64_t	u64;

#if __cplusplus > 202002L // Fixed width floats are only support in C++23 onwards
#include <stdfloat>

#if __STDCPP_FLOAT16_T__ == 1
typedef std::float16_t f16;
#else
typedef float f16;
#pragma message "16-bit float type unsupported, falling back to float"
#endif

#if __STDCPP_FLOAT32_T__ == 1
typedef std::float32_t f32;
#else
typedef float f32;
#pragma message "32-bit float type unsupported, falling back to float"
#endif

#if __STDCPP_FLOAT64_T__ == 1
typedef std::float64_t f64;
#else
typedef double f64;
#pragma message "64-bit float type unsupported, falling back to double"
#endif

#if __STDCPP_FLOAT128_T__ == 1
typedef std::float128_t f128;
#else
typedef long double f128;
#pragma message "128-bit float type unsupported, falling back to long double"
#endif

#if __STDCPP_BFLOAT16_T__ == 1
typedef std::bfloat16_t bf16;
#else
typedef float bf16;
#pragma message "brain float type unsupported, falling back to float"
#endif
#else // __cplusplus >= 202302L

typedef float f16;
typedef float f32;
typedef double f64;
typedef long double f128;
typedef float bf16;

#endif // __cplusplus >= 202302L
