set( HEADERS
    gui/cardbaseproxymodel.h
    gui/cardview.h
    gui/deckmodel.h
    gui/deckmodel_p.h
    gui/mainwindow.h
    gui/manacostdelegate.h
    iohandlers/idatabaseiohandler.h
#    iohandlers/simplexmlhandler.h
#    iohandlers/simplexmliohandler.h
    iohandlers/xmliohandler.h
    iohandlers/xmlhandler.h
    card.h
    cardbase.h
    config.h
    deck.h
    game.h
    pictureloader.h
    pictureloader_p.h
   )

set( SOURCES
    gui/cardbaseproxymodel.cpp
    gui/cardview.cpp
    gui/deckmodel.cpp
    gui/mainwindow.cpp
    gui/manacostdelegate.cpp
#    iohandlers/simplexmlhandler.cpp
#    iohandlers/simplexmliohandler.cpp
    iohandlers/xmliohandler.cpp
    iohandlers/xmlhandler.cpp
    card.cpp
    cardbase.cpp
    config.cpp
    deck.cpp
    game.cpp
    main.cpp
    pictureloader.cpp
   )

set( FORMS
    gui/mainwindow.ui
   )

set( RESOURCES resources.qrc )

qt4_wrap_cpp( MOC_SOURCES ${HEADERS} )
qt4_wrap_ui( UI_HEADERS ${FORMS} )
qt4_add_resources( QRC_SOURCES ${RESOURCES} )

include_directories( ${QT_INCLUDE_DIR} )
include_directories( ${CMAKE_CURRENT_BINARY_DIR} )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

add_executable( CCGO WIN32 MACOSX_BUNDLE ${HEADERS} ${UI_HEADERS} ${SOURCES} ${MOC_SOURCES} ${QRC_SOURCES} )
target_link_libraries( CCGO ${QT_LIBRARIES} )

if( MSVC )
   set_target_properties( CCGO PROPERTIES LINK_FLAGS /ENTRY:"mainCRTStartup" )
endif( MSVC )
