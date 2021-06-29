cmake_minimum_required(VERSION 3.1)

project(Server VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

file(GLOB_RECURSE CORE 
    PRIVATE "${PROJECT_SOURCE_DIR}/Core/*.hpp"
    PRIVATE "${PROJECT_SOURCE_DIR}/Core/*.cpp"
)

add_executable(${PROJECT_NAME} 
    Source.cpp
    ${CORE} 
)

source_group("Core"                FILES ${CORE})

find_package(OpenSSL REQUIRED)

#Add Libraries
target_include_directories(${PROJECT_NAME} 
    PRIVATE "C:/Boost/boost"
    PRIVATE "${PROJECT_SOURCE_DIR}/../common"
    PRIVATE ${OPENSSL_INCLUDE_DIR}
)


target_link_directories(${PROJECT_NAME}
    PRIVATE "C:/Boost/boost/lib"
)

target_link_libraries(${PROJECT_NAME} 
    PRIVATE CommonLib
    PRIVATE ${OPENSSL_LIBRARIES}
)
