# sec_5.2.4_sorting_by_merging/CMakeLists.txt

cmake_minimum_required(VERSION 3.17)

get_filename_component(COMPNAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${COMPNAME})

add_executable(algorithm_m_two_way_merge algorithm_m_two_way_merge.c)
add_executable(algorithm_n_natural_two_way_merge_sort algorithm_n_natural_two_way_merge_sort.c)
add_executable(algorithm_s_straight_two_way_merge_sort algorithm_s_straight_two_way_merge_sort.c)
add_executable(algorithm_l_list_merge_sort algorithm_l_list_merge_sort.c)

if(CMAKE_C_COMPILER_ID MATCHES GNU)

  target_compile_options(algorithm_m_two_way_merge PRIVATE -g -Wall -Werror -O0 -std=c18)
  target_compile_options(algorithm_n_natural_two_way_merge_sort PRIVATE -g -Wall -Werror -O0 -std=c18)
  target_compile_options(algorithm_s_straight_two_way_merge_sort PRIVATE -g -Wall -Werror -Wextra -O0 -std=c18)
  target_compile_options(algorithm_l_list_merge_sort PRIVATE -g -Wall -Werror -Wextra -O0 -std=c18)

elseif(CMAKE_C_COMPILER_ID MATCHES MSVC)

  target_compile_options(algorithm_m_two_way_merge PRIVATE -Wall -WX -Od)
  target_compile_options(algorithm_n_natural_two_way_merge_sort PRIVATE -Wall -WX -Od)

elseif(CMAKE_C_COMPILER_ID MATCHES Clang)

  target_compile_options(algorithm_m_two_way_merge PRIVATE -g -Wall -Werror -O0 -std=c18)
  target_compile_options(algorithm_n_natural_two_way_merge_sort PRIVATE -g -Wall -Werror -O0 -std=c18)

endif()

