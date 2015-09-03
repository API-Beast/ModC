cmake_minimum_required(VERSION 2.8)
project(modc)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake-Modules/")

include(EnableCPP11Flag)
ENABLE_CPP11()

find_package(Springbok REQUIRED)

include_directories(${SPRINGBOK_INCLUDE_DIR})

file(GLOB_RECURSE HEADERS *.h *.hpp)

add_executable(modc main.cpp
                    Configuration.cpp ScriptEngine.cpp VirtualMachine.cpp Environment.cpp ModC.cpp
                    Internals/SyntaxTree.cpp Internals/ScriptValue.cpp Internals/Instruction.cpp
                    Steps/1_Tokenize.cpp Steps/2_Parse.cpp Steps/3_Translate.cpp
                    ${HEADERS})

target_link_libraries(modc ${SPRINGBOK_LIBRARY})