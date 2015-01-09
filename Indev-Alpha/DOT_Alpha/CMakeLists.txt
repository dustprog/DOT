add_subdirectory(Interface)
add_subdirectory(Managers)


add_library(DOT_Alpha STATIC $<TARGET_OBJECTS:DOT_Managers> $<TARGET_OBJECTS:DOT_Interfaces> )
