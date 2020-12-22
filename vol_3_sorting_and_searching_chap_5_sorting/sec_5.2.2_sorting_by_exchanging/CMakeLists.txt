# sec_5.2.2_sorting_by_exchanging/CMakeLists.txt

cmake_minimum_required(VERSION 3.17)

get_filename_component(COMPNAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${COMPNAME})

add_executable(algorithm_b_bubble_sort algorithm_b_bubble_sort.c)
add_executable(algorithm_m_merge_exchange algorithm_m_merge_exchange.c)

add_executable(algorithm_q_quicksort algorithm_q_quicksort.c)
add_executable(algorithm_r_radix_exchange_sort algorithm_r_radix_exchange_sort.c)
add_executable(algorithm_r_radix_exchange_sort.recursive algorithm_r_radix_exchange_sort.recursive.c)

if(CMAKE_C_COMPILER_ID MATCHES GNU)

  target_compile_options(algorithm_b_bubble_sort PRIVATE -g -Wall -Werror -O0 -std=c18)
  target_compile_options(algorithm_m_merge_exchange PRIVATE -g -Wall -Werror -O0 -std=c18)
  target_compile_options(algorithm_q_quicksort PRIVATE -g -Wall -Werror -O0 -std=c18)
  target_compile_options(algorithm_r_radix_exchange_sort PRIVATE -g -Wall -Werror -O0 -std=c18)
  target_compile_options(algorithm_r_radix_exchange_sort.recursive PRIVATE -g -Wall -Werror -O0 -std=c18)

elseif(CMAKE_C_COMPILER_ID MATCHES MSVC)

  target_compile_options(algorithm_b_bubble_sort PRIVATE -Wall -WX -Od)
  target_compile_options(algorithm_m_merge_exchange PRIVATE -Wall -WX -Od)

elseif(CMAKE_C_COMPILER_ID MATCHES Clang)

  target_compile_options(algorithm_b_bubble_sort PRIVATE -g -Wall -Werror -O0 -std=c18)
  target_compile_options(algorithm_m_merge_exchange PRIVATE -g -Wall -Werror -O0 -std=c18)

endif()

