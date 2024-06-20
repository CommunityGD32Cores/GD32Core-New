#ifdef GD32F30x
#ifdef GD32F30X_CL
#include "startup_gd32f30x_cl.S"
#endif
#ifdef GD32F30X_HD
#include "startup_gd32f30x_hd.S"
#endif
#ifdef GD32F30X_XD
#include "startup_gd32f30x_xd.S"
#endif
#elif defined(GD32F3x0)
#include "startup_gd32f3x0.S"
#elif defined(GD32F1x0)
#include "startup_gd32f1x0.S"
#elif defined(GD32F10X_CL)
#include "startup_gd32f10x_cl.S"
#elif defined(GD32F10X_HD)
#include "startup_gd32f10x_hd.S"
#elif defined(GD32F10X_MD)
#include "startup_gd32f10x_md.S"
#elif defined(GD32F10X_XD)
#include "startup_gd32f10x_xd.S"
#elif defined(GD32E23x)
#include "startup_gd32e23x.S"
#elif defined(GD32E50X_HD)
#include "startup_gd32e50x_hd.S"
#elif defined(GD32E50X_XD)
#include "startup_gd32e50x_xd.S"
#elif defined(GD32E50X_CL)
#include "startup_gd32e50x_cl.S"
#elif defined(GD32E508)
#include "startup_gd32e508.S"
#elif defined(GD32EPRT)
#include "startup_gd32eprt.S"
#else
#error "Unknown chip series!"
#endif
