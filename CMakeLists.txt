cmake_minimum_required (VERSION 3.5)
#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS -std=c++11)

project("langd")

find_package(BISON)
find_package(FLEX)

BISON_TARGET(Parser src/parser.ypp ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp)
FLEX_TARGET(Lexer src/lexer.l  ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp)
ADD_FLEX_BISON_DEPENDENCY(Lexer Parser)

set(SRC
    src/main.cpp
    src/ast.cpp
    src/printer.cpp
    #src/java_printer.cpp
)

include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_BINARY_DIR}
)
add_executable(langd
    ${SRC}
    ${BISON_Parser_OUTPUTS}
    ${FLEX_Lexer_OUTPUTS}
)
