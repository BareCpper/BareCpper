cmake_minimum_required(VERSION 3.15)

project( BareCpper )

add_library( BareCpper INTERFACE )

target_include_directories( BareCpper
  INTERFACE
	${CMAKE_CURRENT_LIST_DIR}/include
)


target_compile_features( BareCpper 
INTERFACE
    cxx_constexpr
    cxx_std_14
    cxx_variadic_templates
)

#set_property(TARGET BareCpper PROPERTY CXX_STANDARD_REQUIRED 11)

#target_sources( BareCpper 
#    INTERFACE 
#        ${CMAKE_CURRENT_LIST_DIR}/BareCpper.cpp
#)


add_subdirectory(examples)