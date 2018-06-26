TARGET = tiles
TEMPLATE = app

QT_ADD_PLUGINS_FIREBASE = no
!include(../../Boss2D/project/boss2d.pri) {
    error("Couldn't find the boss2d.pri file...")
}

INCLUDEPATH += ../source
HEADERS += ../source/boss_config.h
SOURCES += ../source/tiles.cpp
HEADERS += ../source/tiles.hpp
SOURCES += ../source/tiles_example.cpp
HEADERS += ../source/tiles_example.hpp
SOURCES += ../source/tiles_example_beginner.cpp
SOURCES += ../source/tiles_example_junior.cpp
SOURCES += ../source/tiles_example_senior.cpp
SOURCES += ../source/main.cpp
SOURCES += ../source/resource.cpp
HEADERS += ../source/resource.hpp

SOURCES += ../../missioncoding/source/classes.cpp
HEADERS += ../../missioncoding/source/classes.hpp

ASSETS_IMAGE.files += ../assets/image
ASSETS_IMAGE.path = /assets

win32{
    RC_ICONS += ../common/windows/main.ico
}

macx{
    QMAKE_BUNDLE_DATA += ASSETS_IMAGE
    QMAKE_INFO_PLIST = $$PWD/../common/macx/Info.plist
    QMAKE_ASSET_CATALOGS += $$PWD/../common/macx/Assets.xcassets
    QMAKE_ASSET_CATALOGS_APP_ICON = AppIcon
}

ios{
    QMAKE_BUNDLE_DATA += ASSETS_IMAGE
    QMAKE_INFO_PLIST = $$PWD/../common/ios/Info.plist
    QMAKE_ASSET_CATALOGS += $$PWD/../common/ios/Assets.xcassets
    QMAKE_ASSET_CATALOGS_APP_ICON = AppIcon
}

android{
    INSTALLS += ASSETS_IMAGE
    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/../common/android
}
