cmake_minimum_required (VERSION 3.0)
#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS -std=c++11)
if(APPLE)
    set(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++")
endif(APPLE)
project("langd")

find_package(BISON)
find_package(FLEX)

BISON_TARGET(Parser src/parser/parser.ypp ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp COMPILE_FLAGS "-v")
FLEX_TARGET(Lexer src/parser/lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp)
ADD_FLEX_BISON_DEPENDENCY(Lexer Parser)

set(SRC
        src/main.cpp
        src/parser/ast.hpp
        src/printer.cpp
        src/printer.hpp
        src/semantic/Expression.cpp
        src/semantic/Expression.hpp
        src/semantic/Analyser.cpp
        src/semantic/Analyser.hpp
        src/semantic/SymbolTable.cpp
        src/semantic/SymbolTable.hpp
        src/semantic/SemanticException.cpp
        src/semantic/SemanticException.hpp
        src/semantic/ExpressionVisitor.cpp
        src/semantic/ExpressionVisitor.hpp
        src/semantic/Type.cpp
        src/semantic/Type.hpp src/java/JavaPrinter.cpp src/java/JavaPrinter.hpp src/semantic/TypeVisitor.cpp src/semantic/TypeVisitor.hpp src/semantic/Closure.cpp src/semantic/Closure.hpp)

include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_BINARY_DIR}
)
add_executable(langd
    ${SRC}
    ${BISON_Parser_OUTPUTS}
    ${FLEX_Lexer_OUTPUTS}
)
