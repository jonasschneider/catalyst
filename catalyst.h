#ifndef CATALYST_H
#define CATALYST_H

#ifdef DEBUG_CATALYST
#define DEBUG1(a) printf(a)
#define DEBUG2(a, b) printf(a, b)
#define DEBUG3(a, b, c) printf(a, b, c)
#define DEBUG4(a, b, c, d) printf(a, b, c, d)
#else
#define DEBUG1(a) 
#define DEBUG2(a, b) 
#define DEBUG3(a, b, c) 
#define DEBUG4(a, b, c, d) 
#endif

#endif