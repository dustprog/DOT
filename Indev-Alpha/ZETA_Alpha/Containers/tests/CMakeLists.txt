SET(target_CPP
    basic_tests.cpp
)
SET(target_INCLUDE
)

SET(target_SOURCES
    ${target_CPP}
    ${target_INCLUDE}
)
INCLUDE_DIRECTORIES(..)

ADD_EXECUTABLE(container_tests ${target_SOURCES})
ADD_DEPENDENCIES(container_tests GMOCK_BUILD)
TARGET_LINK_LIBRARIES(container_tests ${GTest_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

add_test(
    NAME ContainerTests
    COMMAND container_tests
)




