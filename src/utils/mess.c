#include "utils/mess.h"

uint64_t next_pow(uint64_t x)
{
    uint64_t ret = 1;
    if (x == 0){ return 1; }
    while (ret < x)
    {
        ret = ret << 1;
    }
    return ret;
}
