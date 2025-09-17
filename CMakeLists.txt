cmake_minimum_required(VERSION 3.10)
project(vk-friends)
cmake_policy(SET CMP0167 NEW)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(Boost_USE_MULTITHREADED ON)

include_directories(include vendor vendor/termcolor/include)

link_directories(src)

set(SOURCE_FILES
    src/main.cpp
    #src/graph.cpp
    src/parser.cpp
    #src/recommender.cpp
    src/database.cpp
)


find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Boost COMPONENTS program_options system REQUIRED)
find_package(CURL REQUIRED)

if (Boost_FOUND)
    message(STATUS "Boost found. Version: ${Boost_VERSION}")
    message(STATUS "Boost include dirs: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost libraries: ${Boost_LIBRARIES}")
else()
    message(FATAL_ERROR "Boost not found! Please install Boost or set BOOST_ROOT environment variable.")
endif()

include_directories(/usr/local/include ${OPENSSL_INCLUDE_DIR} ${Boost_INCLUDE_DIR})
if (CURL_FOUND)
    include_directories(${CURL_INCLUDE_DIRS})
    add_definitions(-DHAVE_CURL)
endif()

add_executable(vk-friends ${SOURCE_FILES})

target_link_libraries(vk-friends PRIVATE CURL::libcurl ${CMAKE_THREAD_LIBS_INIT} ${OPENSSL_LIBRARIES} ${Boost_LIBRARIES} ${CURL_LIBRARIES})
