cmake_minimum_required(VERSION 3.26)
project(cppgen)

find_package(Boost 1.86 COMPONENTS program_options)

set(CMAKE_CXX_STANDARD "20")
if ("${VCPKG_TARGET_TRIPLET}" STREQUAL "x64-windows-static")
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

add_executable(cppgen
    context.cpp
    context.h
    gram.basic.cpp
    gram.basic.h
    gram.class.cpp
    gram.class.h
    gram.dcl.cpp
    gram.dcl.h
    gram.decl.cpp
    gram.decl.h
    gram.expr.cpp
    gram.expr.h
    gram.lex.cpp
    gram.lex.h
    gram.stmt.cpp
    gram.stmt.h
    main.cpp
    random_utils.h
)

target_link_libraries(cppgen PRIVATE Boost::program_options)
