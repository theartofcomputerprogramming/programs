# sec_1.4.2_coroutines/CMakeLists.txt

cmake_minimum_required(VERSION 3.20)

get_filename_component(COMPNAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${COMPNAME})

add_executable(inout_c.coroutinestruct inout_c.coroutinestruct.c)
add_executable(inout_c.onefunc inout_c.onefunc.c)
add_executable(inout_cplusplus inout_cplusplus.cpp)

if(CMAKE_C_COMPILER_ID MATCHES GNU)

  target_compile_options(inout_cplusplus PRIVATE -ggdb3 -Wall -Werror -Wextra -O0 -std=gnu++2b)
  target_compile_options(inout_c.coroutinestruct PRIVATE -ggdb3 -Wall -Werror -Wextra -O0 -std=c18)
  target_compile_options(inout_c.onefunc PRIVATE -ggdb3 -Wall -Werror -Wextra -O0 -std=c18)

elseif(CMAKE_C_COMPILER_ID MATCHES MSVC)

  target_compile_options(inout_c.coroutinestruct PRIVATE -Wall -WX -Od -std:c17)
  target_compile_options(inout_c.onefunc PRIVATE -Wall -WX -Od -std:c17)
# disable vc++ warning 5045 for index range check and spectre mitigation
  target_compile_options(inout_cplusplus PRIVATE -Wall -WX -Od -std:c++latest -wd5045)

elseif(CMAKE_C_COMPILER_ID MATCHES Clang)

  target_compile_options(inout_cplusplus PRIVATE -ggdb3 -Wall -Wextra -O0 -std=gnu++2b -stdlib=libc++ -fcoroutines-ts)
  target_compile_options(inout_c.coroutinestruct PRIVATE -ggdb3 -Wall -Werror -Wextra -O0 -std=c18)
  target_compile_options(inout_c.onefunc PRIVATE -ggdb3 -Wall -Werror -Wextra -O0 -std=c18)

endif()

