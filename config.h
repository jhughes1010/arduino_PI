//Configuration and compile settings
#define NAME "Searcher v1.0.1"
//#define LCD


#define DEBUG 0

#if DEBUG == 1
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#else
#define debug(x)
#define debugln(x)
#endif
