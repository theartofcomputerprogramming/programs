# sec_5.2.5_sorting_by_distribution/CMakeLists.txt

cmake_minimum_required(VERSION 3.17)

get_filename_component(COMPNAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${COMPNAME})

add_executable(algorithm_r_radix_list_sort algorithm_r_radix_list_sort.c)

if(CMAKE_C_COMPILER_ID MATCHES GNU)

  target_compile_options(algorithm_r_radix_list_sort PRIVATE -g -Wall -Werror -O0 -std=c18)

elseif(CMAKE_C_COMPILER_ID MATCHES MSVC)

  target_compile_options(algorithm_r_radix_list_sort PRIVATE -Wall -WX -Od)

elseif(CMAKE_C_COMPILER_ID MATCHES Clang)

  target_compile_options(algorithm_r_radix_list_sort PRIVATE -g -Wall -Werror -O0 -std=c18)

endif()

