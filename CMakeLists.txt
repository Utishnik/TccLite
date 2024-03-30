cmake_minimum_required(VERSION 3.0)

project(Baza_Data_mini)

set(SRC main.cpp lexer.cpp parser.cpp parsing_cmd_arg.cpp tools.cpp debug.cpp inline_tools.cpp)

add_executable(bd ${SRC})
