cmake_minimum_required(VERSION 3.1)

project(Client VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

file(GLOB_RECURSE CORE 
    PRIVATE "${PROJECT_SOURCE_DIR}/Core/*.hpp"
    PRIVATE "${PROJECT_SOURCE_DIR}/Core/*.cpp"
)

file(GLOB_RECURSE UI 
    PRIVATE "${PROJECT_SOURCE_DIR}/UI/*.hpp"
    PRIVATE "${PROJECT_SOURCE_DIR}/UI/*.cpp"
)

file(GLOB_RECURSE UTILS 
    PRIVATE "${PROJECT_SOURCE_DIR}/Utils/*.hpp"
    PRIVATE "${PROJECT_SOURCE_DIR}/Utils/*.cpp"
)


add_executable(${PROJECT_NAME} 
    Source.cpp
    ${CORE} 
    ${UI}
    ${UTILS}
)

source_group("Core"                FILES ${CORE})
source_group("UI"                  FILES ${UI})
source_group("Utils"               FILES ${UTILS})

#Add Libraries
target_include_directories(${PROJECT_NAME} 
    PRIVATE "C:/Boost/boost"
    PRIVATE "${PROJECT_SOURCE_DIR}/../common"
    PRIVATE "C:/openssl/include64"
)


target_link_directories(${PROJECT_NAME}
    PRIVATE "C:/Boost/boost/lib"
    PRIVATE "C:/openssl/lib64"
)

target_link_libraries(${PROJECT_NAME} 
    CommonLib
)