set(SRC
    World.cpp
)

add_library(DOT_Managers OBJECT ${SRC})


if(ENABLE_TESTS)
add_subdirectory(tests)
endif()
