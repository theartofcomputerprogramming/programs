# sec_2.2.3_linked_allocation/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)

get_filename_component(COMPNAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${COMPNAME})

add_executable(algorithm_t_topological_sort algorithm_t_topological_sort.c)

if(CMAKE_C_COMPILER_ID MATCHES GNU)

  target_compile_definitions(algorithm_t_topological_sort PRIVATE _DEFAULT_SOURCE)
  target_compile_options(algorithm_t_topological_sort PRIVATE -g -Wall -Werror -O0 -std=c18)

elseif(CMAKE_C_COMPILER_ID MATCHES MSVC)

# -wd5045 suppress warning C5045 for spectre mitigation
  target_compile_options(algorithm_t_topological_sort PRIVATE -Wall -WX -Od -std:c17 -wd5045)

elseif(CMAKE_C_COMPILER_ID MATCHES Clang)

  target_compile_options(algorithm_t_topological_sort PRIVATE -g -Wall -Werror -O0 -std=c18)

endif()

