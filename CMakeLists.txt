add_library( BareCpper INTERFACE )

target_include_directories( BareCpper
  INTERFACE
	${CMAKE_CURRENT_LIST_DIR}/include
)

target_sources( BareCpper 
    INTERFACE 
        ${CMAKE_CURRENT_LIST_DIR}/include/BareCpper/spi_libMPSSE.hpp
)

target_compile_features( BareCpper INTERFACE cxx_constexpr cxx_std_11 )
