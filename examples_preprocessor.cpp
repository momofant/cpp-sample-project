#include <stdio.h>

#define CLAMP(val, minval, maxval) \
    ((val) < (minval) ? (minval) : \
     (val) > (maxval) ? (maxval) : (val))

#if defined (DEBUG)
#   define TRACE(...) printf(__VA_ARGS__);
#else
#   define TRACE(...) ;
#endif

void example7()
{
    TRACE("this message will be shown in DEBUG version only");
}
