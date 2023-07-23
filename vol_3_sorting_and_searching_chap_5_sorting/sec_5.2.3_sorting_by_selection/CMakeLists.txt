# sec_5.2.3_sorting_by_selection/CMakeLists.txt

cmake_minimum_required(VERSION 3.17)

get_filename_component(COMPNAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${COMPNAME})

add_executable(algorithm_s_straight_selection_sort_zmajeed algorithm_s_straight_selection_sort_zmajeed.c)
add_executable(algorithm_s_straight_selection_sort_andydude algorithm_s_straight_selection_sort_andydude.c)
add_executable(algorithm_h_heap_sort_andydude algorithm_h_heap_sort_andydude.c)

if(CMAKE_C_COMPILER_ID MATCHES GNU)

  target_compile_options(algorithm_s_straight_selection_sort_zmajeed PRIVATE -g -Wall -Werror -O0 -std=c18)
  target_compile_options(algorithm_s_straight_selection_sort_andydude PRIVATE -g -Wall -Werror -O0 -std=c18)
  target_compile_options(algorithm_h_heap_sort_andydude PRIVATE -g -Wall -Werror -O0 -std=c18)

elseif(CMAKE_C_COMPILER_ID MATCHES MSVC)

  target_compile_options(algorithm_s_straight_selection_sort_zmajeed PRIVATE -Wall -WX -Od)
  target_compile_options(algorithm_s_straight_selection_sort_andydude PRIVATE -Wall -WX -Od)
  target_compile_options(algorithm_h_heap_sort_andydude PRIVATE -Wall -WX -Od)

elseif(CMAKE_C_COMPILER_ID MATCHES Clang)

  target_compile_options(algorithm_s_straight_selection_sort_zmajeed PRIVATE -g -Wall -Werror -O0 -std=c18)
  target_compile_options(algorithm_s_straight_selection_sort_andydude PRIVATE -g -Wall -Werror -O0 -std=c18)
  target_compile_options(algorithm_h_heap_sort_andydude PRIVATE -g -Wall -Werror -O0 -std=c18)

endif()

