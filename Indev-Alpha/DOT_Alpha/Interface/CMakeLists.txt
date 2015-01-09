set(SRC
    EntityBase.cpp
)

add_library(DOT_Interfaces OBJECT ${SRC})


if(ENABLE_TESTS)
add_subdirectory(tests)
endif()
