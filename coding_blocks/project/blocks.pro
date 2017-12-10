TARGET = blocks
TEMPLATE = app

!include(../../Boss2D/project/boss2d.pri) {
    error("Couldn't find the boss2d.pri file...")
}

INCLUDEPATH += ../source
HEADERS += ../source/boss_config.h
SOURCES += ../source/user.cpp
SOURCES += ../source/blocks.cpp
HEADERS += ../source/blocks.hpp
SOURCES += ../source/main.cpp
SOURCES += ../source/r.cpp
HEADERS += ../source/r.hpp

ASSETS_IMAGE.files += ../assets/image
ASSETS_IMAGE.path = /assets

win32{
    RC_ICONS += ../common/windows/main.ico
}

android{
    INSTALLS += ASSETS_IMAGE
    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/../common/android
}

ios|macx{
    QMAKE_BUNDLE_DATA += ASSETS_IMAGE
    QMAKE_BUNDLE_DATA += MAIN_ICON
    QMAKE_INFO_PLIST = $$PWD/../common/ios/Info.plist
    MAIN_ICON.files += $$PWD/../common/ios/res/icon.icns
    ios: MAIN_ICON.path = /
    macx: MAIN_ICON.path = /Contents/Resources
}