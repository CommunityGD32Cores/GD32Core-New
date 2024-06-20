#ifdef GD32F30x
#include "system_gd32f30x.c"
#elif defined(GD32F3x0)
#include "system_gd32f3x0.c"
#elif defined(GD32F1x0)
#include "system_gd32f1x0.c"
#elif defined(GD32F10x)
#include "system_gd32f10x.c"
#elif defined(GD32E23x)
#include "system_gd32e23x.c"
#elif defined(GD32E50X)
#include "system_gd32e50x.c"
#else
#error "Unknown chip series!"
#endif
