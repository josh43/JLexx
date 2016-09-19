cmake_minimum_required(VERSION 3.3)
project(JLexx)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")
#JLexx/LexLoader/GenFiles/BridgeHeader.h JLexx/LexLoader/GenFiles/TheRealJLexx.h
set(SOURCE_FILES JLexx/LexLoader/GenFiles/LexMain.cpp  JLexx/MatchHandler.h RegularExpression/Regex/Regex.h RegularExpression/Regex/Regex.cpp JLexx/RegexToDFABuilder.h RegularExpression/RegexIncludeAll.h JLexx/DFABuilderHelper.h RegularExpression/Utility.cpp RegularExpression/Utility.h JLexx/FileLoader.h JLexx/RestBuilder/RestBuilder.h JLexx/RestBuilder/RestGenerator.h JLexx/RestBuilder/RestToken.h JLexx/RestBuilder/Test/Test.h JLexx/RestBuilder/RestGenHelper.h JLexx/LexLoader/LexGenerator.h RegularExpression/Parser/RegularErrorMessage.h RegularExpression/Parser/ExperimentalParser.h)
add_executable(JLexx ${SOURCE_FILES})