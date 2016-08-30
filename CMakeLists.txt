cmake_minimum_required(VERSION 3.3)
project(JLexx)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCE_FILES main.cpp JLexx/JLexx.h JLexx/MatchHandler.h RegularExpression/Regex/Regex.h RegularExpression/Regex/Regex.cpp JLexx/RegexToDFABuilder.h RegularExpression/RegexIncludeAll.h JLexx/DFABuilderHelper.h RegularExpression/Utility.cpp RegularExpression/Utility.h JLexx/LexLoader/Parser.h JLexx/FileLoader.h JLexx/LexLoader/Generator.h JLexx/LexLoader/Generator.cpp JLexx/RestBuilder/RestBuilder.h JLexx/RestBuilder/RestGenerator.h JLexx/RestBuilder/RestToken.h JLexx/RestBuilder/Test/Test.h JLexx/RestBuilder/RestGenHelper.h )
add_executable(JLexx ${SOURCE_FILES})