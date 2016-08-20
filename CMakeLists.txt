cmake_minimum_required(VERSION 3.3)
project(JLexx)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCE_FILES main.cpp JLexx/JLexx.h JLexx/MatchHandler.h JLexx/RegexToDFABuilder.h RegularExpression/RegexIncludeAll.h JLexx/DFABuilderHelper.h RegularExpression/Utility.cpp RegularExpression/Utility.h)
add_executable(JLexx ${SOURCE_FILES})