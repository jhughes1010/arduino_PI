//Configuration and compile settings
<<<<<<< HEAD
#define NAME "Searcher"
#define VERSION "v1.1 - 2022"
#define LCD
=======
#define NAME "Searcher v1.0.1"
//#define LCD
>>>>>>> 02070b9dd176571c209c8045c58e2f7138a03c32


#define DEBUG 1

#if DEBUG == 1
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#else
#define debug(x)
#define debugln(x)
#endif
