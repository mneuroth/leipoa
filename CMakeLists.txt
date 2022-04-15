cmake_minimum_required(VERSION 3.21)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ------------------------------------------------------------------------------

project(leipoa VERSION 0.1.0 LANGUAGES CXX)

function(printVariables)
    # Print variables
    set(PATTERN ".*")
    get_cmake_property(_variableNames VARIABLES)
    list (SORT _variableNames)
    foreach (_variableName ${_variableNames})
        unset(MATCHED)
        string(REGEX MATCH ${PATTERN} MATCHED ${_variableName})
        if (NOT MATCHED)
            continue()
        endif()
        message(STATUS "${_variableName}=${${_variableName}}")
    endforeach()
endfunction()

#printVariables()

if (ANDROID)
    include(${CMAKE_CURRENT_LIST_DIR}/lib/android_openssl/CMakeLists.txt)
else()
    set (EXTRA_LIBS
        ssl crypto
    )
endif()

add_subdirectory(ui-qml)
