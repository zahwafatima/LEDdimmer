// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef IOS_H
#define	IOS_H

#include <xc.h> // include processor files - each processor file is guarded.

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */
void IOinit();
void IOcheck(int CN30flag, int CN0flag);
#endif	/* XC_HEADER_TEMPLATE_H */

