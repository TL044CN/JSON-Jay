cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED ON)

# find out wether this project is a submodule or not
if(DEFINED PROJECT_NAME AND NOT "${PROJECT_NAME}" STREQUAL "")
    set(IS_SUBMODULE ON)
else()
    set(IS_SUBMODULE OFF)
endif()

# get the version from git
find_package(Git)
if(GIT_FOUND)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} describe --tags --abbrev=0
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_TAG_VERSION
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(
            COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            RESULT_VARIABLE GIT_SUBMODULE_RESULT
        )
        if(NOT GIT_SUBMODULE_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMODULE_RESULT}")
        endif()
    endif()

else()
    set(GIT_TAG_VERSION "1.0.0")
endif()

# sanitize the version to prevent cmake errors
string(REGEX REPLACE "[^0-9.]" "" SANITIZED_VERSION "${GIT_TAG_VERSION}")

# declare the project
project(JSONJay
    VERSION ${SANITIZED_VERSION}
    DESCRIPTION "A simple Serialization Library for C++ (mainly JSON)"
    LANGUAGES CXX
)

# set the build options
if(IS_SUBMODULE)
    set(${PROJECT_NAME}_BUILD_TESTS OFF CACHE BOOL "Build Tests")
    set(${PROJECT_NAME}_BUILD_DOCS OFF CACHE BOOL "Build Documentation")
else()
    set(${PROJECT_NAME}_BUILD_TESTS ON CACHE BOOL "Build Tests")
    set(${PROJECT_NAME}_BUILD_DOCS ON CACHE BOOL "Build Documentation")
endif()

# print a message when a condition is true
function(print_conditional_status CONDITION MSG_ON MSG_OFF)
    if(${CONDITION})
        message(STATUS ${MSG_ON})
    else()
        message(STATUS ${MSG_OFF})
    endif()
endfunction()

# print the project information
message(STATUS "==========|${PROJECT_NAME}|=======>>")
message(STATUS "Version:      ${PROJECT_VERSION}")
message(STATUS "Git Tag:      ${GIT_TAG_VERSION}")
print_conditional_status(IS_SUBMODULE "Is Submodule: Yes" "Is Submodule: No")
print_conditional_status(${PROJECT_NAME}_BUILD_SHARED
    "Library Type: Shared"
    "Library Type: Static"
)
print_conditional_status(${PROJECT_NAME}_BUILD_TESTS
    "Build Tests:  Yes" 
    "Build Tests:  No"
)
message(STATUS "<<========|${PROJECT_NAME}|=========")

# add the sources
set(${PROJECT_NAME}_SOURCES
    source/Common.cpp
    source/StreamReadinator.cpp
    source/StreamWritinator.cpp
    source/FileStreamReadinator.cpp
    source/FileStreamWritinator.cpp
    source/BufferStreamReadinator.cpp
    source/BufferStreamWritinator.cpp
    source/List.cpp
    source/Object.cpp
)


# add the library target
if(${PROJECT_NAME}_BUILD_SHARED)
    add_library(${PROJECT_NAME} SHARED ${${PROJECT_NAME}_SOURCES})
else()
    add_library(${PROJECT_NAME} STATIC ${${PROJECT_NAME}_SOURCES})
endif()

# add the include directories
target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

# set the project properties
set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    PUBLIC_HEADER "include/JSONJay.hpp"
)

# set the export targets
install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
    PUBLIC_HEADER DESTINATION include
)

# install the headers
install(
    DIRECTORY include/
    DESTINATION include
)

# install the cmake config files
install(EXPORT ${PROJECT_NAME}Targets
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION lib/cmake/${PROJECT_NAME}
)

# build documentation
if(${PROJECT_NAME}_BUILD_DOCS)
find_package(Doxygen)
    if(DOXYGEN_FOUND)
        add_custom_target(documentation
            COMMAND "doxygen"
            WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
            COMMENT "Generating Doxygen Documentation"
            VERBARIM
        )
    else()
        message("Doxygen required to build Doxygen Documentation")
    endif()
endif()

# build tests
if(${PROJECT_NAME}_BUILD_TESTS)
    find_program(LCOV lcov)
    find_program(GENHTML genhtml)

    #find correct coverage system
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        find_program(GCOV gcov)
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        find_program(GCOV llvm-cov)
    endif()
    
    #prepare for coverage report
    if(GCOV AND LCOV AND GENHTML)
        if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
            target_compile_options(${PROJECT_NAME} PRIVATE -fprofile-arcs -ftest-coverage)
        elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
            target_compile_options(${PROJECT_NAME} PRIVATE -fprofile-instr-generate -fcoverage-mapping)
        endif()
    else()
        if(NOT GCOV)
            message(WARNING "gcov not found. No coverage report will be generated.")
        endif()
        if(NOT LCOV)
            message(WARNING "lcov not found. No coverage report will be generated.")
        endif()
        if(NOT GENHTML)
            message(WARNING "genhtml not found. No coverage report will be generated.")
        endif()
    endif()
        
    # tests:
    enable_testing()
    add_subdirectory(tests)
endif()