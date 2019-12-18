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
    cxx_std_17 # fold-expressions
    cxx_variadic_templates
)

#target_sources( BareCpper 
#    INTERFACE 
#        ${CMAKE_CURRENT_LIST_DIR}/BareCpper.cpp
#)


add_subdirectory(examples)