

int printf(const char *format, ...);

#ifdef DEBUG
//#define DBG(x ...)	printf(x)
#define DBG	printf
#define ASSERT(x)	if(!(x)){DBG("\nAssertion '%s' failed in %s:%s#%d!\n",#x,__FILE__,__FUNCTION__,__LINE__);while(1);}
#else
//#define DBG(x ...)
#define DBG
#define ASSERT(x)
#endif

