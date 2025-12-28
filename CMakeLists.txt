cmake_minimum_required(VERSION 3.16)
project(timediff VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

add_library(timediff
    src/timediff.cpp
)

target_include_directories(timediff
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

add_executable(timediff-cli
    cli/main.cpp
)

target_link_libraries(timediff-cli
    timediff
)

# ---------- install library ----------
install(TARGETS timediff
        EXPORT timediffTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# ---------- install cli ----------
install(TARGETS timediff-cli
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# ---------- install headers ----------
install(DIRECTORY include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# ---------- export targets ----------
install(EXPORT timediffTargets
        NAMESPACE timediff::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/timediff
)

# ---------- config file ----------
configure_package_config_file(
    cmake/timediffConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/timediffConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/timediff
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/timediffConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/timediffConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/timediffConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/timediff
)
