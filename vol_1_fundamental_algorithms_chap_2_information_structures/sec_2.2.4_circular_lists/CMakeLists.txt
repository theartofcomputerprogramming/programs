# sec_2.2.4_circular_lists/CMakeLists.txt

cmake_minimum_required(VERSION 3.17)

get_filename_component(COMPNAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${COMPNAME})

add_library(polynomial STATIC polynomial.c)
target_include_directories(polynomial PRIVATE .)

add_executable(algorithm_a_addition_of_polynomials algorithm_a_addition_of_polynomials.c)
target_link_libraries(algorithm_a_addition_of_polynomials PRIVATE polynomial)

add_executable(algorithm_m_multiplication_of_polynomials algorithm_m_multiplication_of_polynomials.c)
target_link_libraries(algorithm_m_multiplication_of_polynomials PRIVATE polynomial)

if(CMAKE_C_COMPILER_ID MATCHES GNU)

  target_compile_definitions(algorithm_a_addition_of_polynomials PRIVATE _DEFAULT_SOURCE)
  target_compile_options(algorithm_a_addition_of_polynomials PRIVATE -g -Wall -Werror -O0 -std=c18)

elseif(CMAKE_C_COMPILER_ID MATCHES MSVC)

  target_compile_options(algorithm_a_addition_of_polynomials PRIVATE -Wall -WX -Od)

elseif(CMAKE_C_COMPILER_ID MATCHES Clang)

  target_compile_options(algorithm_a_addition_of_polynomials PRIVATE -g -Wall -Werror -O0 -std=c18)

endif()

