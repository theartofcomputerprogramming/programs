# sec_5.2_internal_sorting/CMakeLists.txt

cmake_minimum_required(VERSION 3.17)

get_filename_component(COMPNAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${COMPNAME})

add_executable(algorithm_c_comparison_counting algorithm_c_comparison_counting.c)
add_executable(exercise_5.2.4 exercise_5.2.4.c algorithm_c_comparison_counting.c)

if(CMAKE_C_COMPILER_ID MATCHES GNU)

  target_compile_definitions(algorithm_c_comparison_counting PRIVATE ALGORITHM_C_COMPARISON_COUNTING_BUILD_MAIN)
  target_compile_options(algorithm_c_comparison_counting PRIVATE -g -Wall -Werror -O0 -std=c18)

  target_compile_options(exercise_5.2.4 PRIVATE -g -Wall -Werror -O0 -std=c18)

elseif(CMAKE_C_COMPILER_ID MATCHES MSVC)

  target_compile_options(algorithm_c_comparison_counting PRIVATE -Wall -WX -Od)

elseif(CMAKE_C_COMPILER_ID MATCHES Clang)

  target_compile_options(algorithm_c_comparison_counting PRIVATE -g -Wall -Werror -O0 -std=c18)

endif()

