set(SRC
    World.cpp
)
add_library(DOT_Managers OBJECT ${SRC})
#add_library(DOT_Py SHARED DOT_Py.h)
#target_link_libraries(DOT_Py DOT_Alpha ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})
#set_target_properties( DOT_Py PROPERTIES PREFIX "" )
