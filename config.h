//Configuration and compile settings
#define NAME "Searcher"
#define VERSION "v1.1 - 2022"
#define LCD


#define DEBUG 1

#if DEBUG == 1
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#else
#define debug(x)
#define debugln(x)
#endif
