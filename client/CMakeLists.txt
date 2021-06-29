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

find_package(OpenSSL REQUIRED)

message("OPENSSL_ROOT_DIR is "  ${OPENSSL_ROOT_DIR})
message("OPENSSL_FOUND is "  ${OPENSSL_FOUND})
message("OPENSSL_INCLUDE_DIR is "  ${OPENSSL_INCLUDE_DIR})
message("OPENSSL_CRYPTO_LIBRARY is "  ${OPENSSL_CRYPTO_LIBRARY})
message("OPENSSL_SSL_LIBRARY is "  ${OPENSSL_SSL_LIBRARY})
message("OPENSSL_LIBRARIES is "  ${OPENSSL_LIBRARIES})
message("OPENSSL_VERSION is "  ${OPENSSL_VERSION})

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
    CommonLib
    ${OPENSSL_LIBRARIES}
)
