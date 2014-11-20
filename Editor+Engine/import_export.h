#ifndef IMPORT_EXPORT
#define IMPORT_EXPORT
#if defined _WIN32 || defined __CYGWIN__
    #ifdef __GNUC__
      #define DLL_PUBLIC __attribute__ ((dllexport))
      #define DLL_IMPORT __attribute__ ((dllimport))
    #else
      #define DLL_PUBLIC __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
      #define DLL_IMPORT __declspec(dllimport)
    #endif
  #define DLL_LOCAL
#else
  #if __GNUC__ >= 4
    #define DLL_PUBLIC __attribute__ ((visibility ("default")))
    #define DLL_LOCAL  __attribute__ ((visibility ("hidden")))
    #define DLL_IMPORT
  #else
    #define DLL_PUBLIC
    #define DLL_LOCAL
    #define DLL_IMPORT
  #endif
#endif
#endif // IMPORT_EXPORT

