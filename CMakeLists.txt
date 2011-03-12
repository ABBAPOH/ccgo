project( CCGO )

cmake_minimum_required( VERSION 2.6 )

find_package( Qt4 REQUIRED )
set( QT_USE_QTXML TRUE )
set( QT_USE_QTSQL TRUE )
set( QT_USE_QTDECLARATIVE TRUE )
set( QT_USE_QTNETWORK TRUE )

include( ${QT_USE_FILE} )

add_definitions( ${QT_DEFINITIONS} )
add_definitions( -DUNICODE )
add_definitions( -Wall --pedantic )

set( LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin )
set( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin )

#-DCMAKE_BUILD_TYPE=Debug
set( CMAKE_BUILD_TYPE Debug )
#set( CMAKE_BUILD_TYPE Release )

if(MSVC)
    set(CMAKE_SHARED_LIBRARY_PREFIX "lib")
endif(MSVC)

add_subdirectory( src )
#add_subdirectory( resources )
