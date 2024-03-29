cmake_minimum_required(VERSION 3.14)

project(voctrain)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/W4>)
add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/permissive->)
add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/std:c++latest>)
add_compile_definitions($<$<CXX_COMPILER_ID:MSVC>:_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING>)
add_compile_definitions($<$<CXX_COMPILER_ID:MSVC>:_SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING>)
add_compile_definitions($<$<CXX_COMPILER_ID:MSVC>:WIN32_LEAN_AND_MEAN>)
add_compile_options($<$<CXX_COMPILER_ID:GNU,AppleClang,Clang>:-Wall>)
add_compile_options($<$<CXX_COMPILER_ID:GNU,AppleClang,Clang>:-pedantic>)
add_compile_options($<$<CXX_COMPILER_ID:Clang>:-stdlib=libc++>)
add_link_options($<$<CXX_COMPILER_ID:Clang>:-stdlib=libc++>)

set(QT5_BASE_ROOT "" CACHE PATH "Qt5 installation root directory.")
find_package(Qt5 REQUIRED COMPONENTS Widgets
    HINTS ${QT5_BASE_ROOT} $ENV{QT5_ROOT}/5.12.2/msvc2017_64 $ENV{QT5_ROOT}/5.12.0/msvc2017_64
)

set(GHULBUS_BASE_ROOT "" CACHE PATH "GhulbusBase installation root directory.")
if(GHULBUS_BASE_ROOT)
    list(APPEND CMAKE_PREFIX_PATH ${GHULBUS_BASE_ROOT}/cmake)
endif()
find_package(gbBase REQUIRED)


set(VOCTRAIN_SOURCE_DIRECTORY ${PROJECT_SOURCE_DIR})

set(VOCTRAIN_SOURCE_FILES
    ${VOCTRAIN_SOURCE_DIRECTORY}/main.cpp
)

set(VOCTRAIN_HEADER_FILES
)

set(VOCTRAIN_QT_SOURCE_FILES
    ${VOCTRAIN_SOURCE_DIRECTORY}/central_widget.cpp
    ${VOCTRAIN_SOURCE_DIRECTORY}/main_window.cpp
)

set(VOCTRAIN_QT_HEADER_FILES
)

set(VOCTRAIN_QT_MOC_HEADER_FILES
    ${VOCTRAIN_SOURCE_DIRECTORY}/central_widget.hpp
    ${VOCTRAIN_SOURCE_DIRECTORY}/main_window.hpp
)
qt5_wrap_cpp(VOCTRAIN_QT_MOC_SOURCE_FILES ${VOCTRAIN_QT_MOC_HEADER_FILES})

source_group(moc FILES
    ${VOCTRAIN_QT_MOC_SOURCE_FILES}
)

add_executable(voctrain WIN32
    ${VOCTRAIN_SOURCE_FILES}
    ${VOCTRAIN_HEADER_FILES}
    ${VOCTRAIN_QT_SOURCE_FILES}
    ${VOCTRAIN_QT_HEADER_FILES}
    ${VOCTRAIN_QT_MOC_HEADER_FILES}
    ${VOCTRAIN_QT_MOC_SOURCE_FILES}
    ${VOCTRAIN_QT_RESOURCE_FILES}
)
target_include_directories(voctrain PUBLIC ${VOCTRAIN_SOURCE_DIRECTORY})
target_link_libraries(voctrain PUBLIC
    Ghulbus::gbBase
    Qt5::Core
    Qt5::Widgets
)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT voctrain)

if(WIN32)
    function(getPDBForDLL DLL_PATH OUT_VAR)
        get_filename_component(dll_dir ${DLL_PATH} DIRECTORY)
        get_filename_component(dll_we ${DLL_PATH} NAME_WE)
        set(${OUT_VAR} "${dll_dir}/${dll_we}.pdb" PARENT_SCOPE)
    endfunction()


    function(getQt5Dlls QT_TARGET OUT_VAR)
        unset(DLLS)
        get_property(tmp TARGET ${QT_TARGET} PROPERTY IMPORTED_LOCATION_DEBUG)
        list(APPEND DLLS ${tmp})
        getPDBForDLL(${tmp} tmp_pdb)
        if (EXISTS ${tmp_pdb})
            list(APPEND DLLS ${tmp_pdb})
        endif()
        get_property(tmp TARGET ${QT_TARGET} PROPERTY IMPORTED_LOCATION_RELEASE)
        list(APPEND DLLS ${tmp})
        get_property(deps TARGET ${QT_TARGET} PROPERTY IMPORTED_LINK_INTERFACE_LIBRARIES_DEBUG)
        foreach(dep ${deps})
            if(TARGET ${dep})
                getQt5Dlls(${dep} tmp)
                list(APPEND DLLS ${tmp})
            endif()
        endforeach()
        set(result ${${OUT_VAR}})
        list(APPEND result ${DLLS})
        list(REMOVE_DUPLICATES result)
        set(${OUT_VAR} ${result} PARENT_SCOPE)
    endfunction()

    getQt5Dlls(Qt5::Widgets qt_DLLS)

    file(COPY ${qt_DLLS} DESTINATION ${PROJECT_BINARY_DIR})

endif()
