cmake_minimum_required(VERSION 3.5)

# Set extension name here
set(TARGET_NAME duckdb_gemmi)

# DuckDB's extension distribution supports vcpkg. As such, dependencies can be added in ./vcpkg.json and then
# used in cmake with find_package. Feel free to remove or replace with other dependencies.
# Note that it should also be removed from vcpkg.json to prevent needlessly installing it..
find_package(OpenSSL REQUIRED)

include(FetchContent)
FetchContent_Declare(
  gemmi
  GIT_REPOSITORY https://github.com/project-gemmi/gemmi.git
  GIT_TAG        v0.7.5
)
FetchContent_MakeAvailable(gemmi)

# fetch & build w/ zlib-ng statically (for gemmi)
set(FETCH_ZLIB_NG ON)

set(EXTENSION_NAME ${TARGET_NAME}_extension)
set(LOADABLE_EXTENSION_NAME ${TARGET_NAME}_loadable_extension)

project(${TARGET_NAME})
include_directories(src/include)

set(EXTENSION_SOURCES src/duckdb_gemmi_extension.cpp)

build_static_extension(${TARGET_NAME} ${EXTENSION_SOURCES})
build_loadable_extension(${TARGET_NAME} " " ${EXTENSION_SOURCES})

# Link OpenSSL in both the static library as the loadable extension
target_link_libraries(${EXTENSION_NAME} OpenSSL::SSL OpenSSL::Crypto)
target_link_libraries(${LOADABLE_EXTENSION_NAME} OpenSSL::SSL OpenSSL::Crypto)


# header only (CIF parsing only) - NOTE: cannot use keyword form
# target_link_libraries, as duckdb uses the plain form, use include
# directories directly to avoid export issues
get_target_property(GEMMI_INCLUDE_DIRS gemmi::headers INTERFACE_INCLUDE_DIRECTORIES)
target_include_directories(${EXTENSION_NAME} PRIVATE ${GEMMI_INCLUDE_DIRS})
target_include_directories(${LOADABLE_EXTENSION_NAME} PRIVATE ${GEMMI_INCLUDE_DIRS})
target_compile_features(${EXTENSION_NAME} PRIVATE cxx_std_14)
target_compile_features(${LOADABLE_EXTENSION_NAME} PRIVATE cxx_std_14)

install(
  TARGETS ${EXTENSION_NAME}
  EXPORT "${DUCKDB_EXPORT_SET}"
  LIBRARY DESTINATION "${INSTALL_LIB_DIR}"
  ARCHIVE DESTINATION "${INSTALL_LIB_DIR}")
