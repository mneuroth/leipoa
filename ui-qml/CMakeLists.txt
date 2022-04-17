cmake_minimum_required(VERSION 3.21)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ------------------------------------------------------------------------------

project(leipoa VERSION 0.1 LANGUAGES CXX)

find_package(Qt6 6.3 COMPONENTS Core Quick REQUIRED)

find_package(OpenSSL REQUIRED)

qt_add_executable(appleipoa
    src/main.cpp
    src/qmlaes.cpp
)

qt_add_qml_module(appleipoa
    URI leipoa
    VERSION 1.0
    QML_FILES views/main.qml
)

set_target_properties(appleipoa PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_compile_definitions(appleipoa
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

message("INCLUDE")
message(${OPENSSL_INCLUDE_DIR})

target_include_directories(appleipoa
    PRIVATE ${OPENSSL_INCLUDE_DIR})

target_link_libraries(appleipoa
    PRIVATE Qt6::Quick ${ANDROID_EXTRA_LIBS})

target_link_libraries(appleipoa
    PRIVATE Qt6::Quick ${EXTRA_LIBS})
