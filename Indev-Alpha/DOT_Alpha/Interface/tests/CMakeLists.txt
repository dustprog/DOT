SET(target_CPP
    world_tests.cpp
)
SET(target_INCLUDE
    ../World.h
)

SET(target_SOURCES
    ${target_CPP}
    ${target_INCLUDE}
)
INCLUDE_DIRECTORIES(..)

#ADD_EXECUTABLE(world_tests ${target_SOURCES})
#ADD_DEPENDENCIES(world_tests GMOCK_BUILD)
#TARGET_LINK_LIBRARIES(world_tests DOT_Alpha ${GTest_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})


#add_test(
#    NAME WorldTests
#    COMMAND world_tests
#)


