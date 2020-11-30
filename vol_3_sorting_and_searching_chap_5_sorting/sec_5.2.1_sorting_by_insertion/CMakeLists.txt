# sec_5.2.1_sorting_by_insertion/CMakeLists.txt

cmake_minimum_required(VERSION 3.17)

get_filename_component(COMPNAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${COMPNAME})

add_executable(algorithm_s_straight_insertion_sort algorithm_s_straight_insertion_sort.c)
add_executable(algorithm_d_shellsort algorithm_d_shellsort.c)

if(CMAKE_C_COMPILER_ID MATCHES GNU)

  target_compile_options(algorithm_s_straight_insertion_sort PRIVATE -g -Wall -Werror -O0 -std=c18)
  target_compile_options(algorithm_d_shellsort PRIVATE -g -Wall -Werror -O0 -std=c18)


elseif(CMAKE_C_COMPILER_ID MATCHES MSVC)

  target_compile_options(algorithm_s_straight_insertion_sort PRIVATE -Wall -WX -Od)
  target_compile_options(algorithm_d_shellsort PRIVATE -Wall -WX -Od)

elseif(CMAKE_C_COMPILER_ID MATCHES Clang)

  target_compile_options(algorithm_s_straight_insertion_sort PRIVATE -g -Wall -Werror -O0 -std=c18)
  target_compile_options(algorithm_d_shellsort PRIVATE -g -Wall -Werror -O0 -std=c18)

endif()

