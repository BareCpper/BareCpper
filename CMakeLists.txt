add_library( BareCpper INTERFACE )

target_include_directories( BareCpper
  INTERFACE
	${CMAKE_CURRENT_LIST_DIR}/include
)

#target_sources( BareCpper 
#    INTERFACE 
#        ${CMAKE_CURRENT_LIST_DIR}/BareCpper.cpp
#)
