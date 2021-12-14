cmake_minimum_required(VERSION 3.16)
project(Bookstore)

set(CMAKE_CXX_STANDARD 20)
add_compile_options(-std=c++2a -g )

# include_directories(
#         ${PROJECT_SOURCE_DIR}/src/
#         # ${PROJECT_SOURCE_DIR}/bin/
# )
# set(src_dir
#         ${PROJECT_SOURCE_DIR}/src/main.cpp
#         # ${PROJECT_SOURCE_DIR}/src/unrolled_linklist.hpp
#         # ${PROJECT_SOURCE_DIR}/src/unrolled_linklist.cpp
#         )
add_executable(code 
${PROJECT_SOURCE_DIR}/src/error.cpp
${PROJECT_SOURCE_DIR}/src/log.cpp
${PROJECT_SOURCE_DIR}/src/filemap.cpp
${PROJECT_SOURCE_DIR}/src/parser.cpp
${PROJECT_SOURCE_DIR}/src/lib.cpp
${PROJECT_SOURCE_DIR}/src/account.cpp
${PROJECT_SOURCE_DIR}/src/main.cpp)
# ${src_dir})

# 添加链接库
#target_link_libraries(code ${PROJECT_SOURCE_DIR}/bin/libUll.a)

# 后缀名为 .a 和 .so 的文件分别为静态链接库和动态链接库