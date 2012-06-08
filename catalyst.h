#ifndef CATALYST_H
#define CATALYST_H

#ifdef DEBUG_CATALYST
#define DEBUG1(a) printf(a)
#define DEBUG2(a, b) printf(a, b)
#define DEBUG3(a, b, c) printf(a, b, c)
#else
#define DEBUG1(a) 
#define DEBUG2(a, b) 
#define DEBUG3(a, b, c) 
#endif

#endif