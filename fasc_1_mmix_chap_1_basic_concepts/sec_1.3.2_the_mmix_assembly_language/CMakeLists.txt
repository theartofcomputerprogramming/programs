# sec_1.3.2_the_mmix_assembly_language/CMakeLists.txt

cmake_minimum_required(VERSION 3.17)

get_filename_component(COMPNAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${COMPNAME})

add_executable(program_m_find_the_maximum program_m_find_the_maximum.c)
add_executable(program_h_hail_the_world program_h_hail_the_world.c)
add_executable(program_p_print_table_of_500_primes program_p_print_table_of_500_primes.c)

if(CMAKE_C_COMPILER_ID MATCHES GNU)

  target_compile_definitions(program_m_find_the_maximum PRIVATE _DEFAULT_SOURCE)
  target_compile_options(program_m_find_the_maximum PRIVATE -g -Wall -Werror -O0 -std=c18)

  target_compile_options(program_h_hail_the_world PRIVATE -g -Wall -Werror -O0 -std=c18)

  target_compile_options(program_p_print_table_of_500_primes PRIVATE -g -Wall -Werror -O0 -std=c18)

elseif(CMAKE_C_COMPILER_ID MATCHES MSVC)

  target_compile_options(program_m_find_the_maximum PRIVATE -Wall -WX -Od)

elseif(CMAKE_C_COMPILER_ID MATCHES Clang)

  target_compile_options(program_m_find_the_maximum PRIVATE -g -Wall -Werror -O0 -std=c18)

endif()

