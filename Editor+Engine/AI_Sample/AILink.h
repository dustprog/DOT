#ifndef AILINK_HEADER
#define AILINK_HEADER
#include "World.h"
#include "import_export.h"
    #ifdef AILINK_EXPORTS
    #define AILINK DLL_PUBLIC
    #else
    #define AILINK DLL_IMPORT
    #endif

    class runner
    {
    public:
            static AILINK int startup(World* currentworld);
            static AILINK int run(World* currentworld, int* k, int* i, bool* running);
    };

#endif
