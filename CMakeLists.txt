cmake_minimum_required(VERSION 3.7)
project(xasm)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c LinkedList.h LinkedList.c Constants.h StringTable.c StringTable.h InstrLookupTable.c InstrLookupTable.h FuncTable.c FuncTable.h SymbolTable.c SymbolTable.h StringUtils.c StringUtils.h SourceLoader.c SourceLoader.h Constants.c LabelTable.c LabelTable.h)
add_executable(xasm ${SOURCE_FILES})