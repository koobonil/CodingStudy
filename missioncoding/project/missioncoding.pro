TARGET = missioncoding
TEMPLATE = app

!include(../../Boss2D/project/boss2d.pri) {
    error("Couldn't find the boss2d.pri file...")
}

INCLUDEPATH += ../source
HEADERS += ../source/boss_config.h
SOURCES += ../source/classes.cpp
HEADERS += ../source/classes.hpp
SOURCES += ../source/main.cpp
SOURCES += ../source/missioncoding_function.cpp
HEADERS += ../source/missioncoding_function.hpp
SOURCES += ../source/missioncoding_level.cpp
HEADERS += ../source/missioncoding_level.hpp
SOURCES += ../source/missioncoding_list.cpp
HEADERS += ../source/missioncoding_list.hpp
SOURCES += ../source/resource.cpp
HEADERS += ../source/resource.hpp

SOURCES += ../../coding_balls/source/balls.cpp
HEADERS += ../../coding_balls/source/balls.hpp
SOURCES += ../../coding_balls/source/balls_example.cpp
HEADERS += ../../coding_balls/source/balls_example.hpp
SOURCES += ../../coding_balls/source/balls_example_beginner.cpp
SOURCES += ../../coding_balls/source/balls_example_junior.cpp
SOURCES += ../../coding_balls/source/balls_example_senior.cpp

SOURCES += ../../coding_blocks/source/blocks.cpp
HEADERS += ../../coding_blocks/source/blocks.hpp
SOURCES += ../../coding_blocks/source/blocks_example.cpp
HEADERS += ../../coding_blocks/source/blocks_example.hpp
SOURCES += ../../coding_blocks/source/blocks_example_beginner.cpp
SOURCES += ../../coding_blocks/source/blocks_example_junior.cpp
SOURCES += ../../coding_blocks/source/blocks_example_senior.cpp

SOURCES += ../../coding_rains/source/rains.cpp
HEADERS += ../../coding_rains/source/rains.hpp
SOURCES += ../../coding_rains/source/rains_example.cpp
HEADERS += ../../coding_rains/source/rains_example.hpp
SOURCES += ../../coding_rains/source/rains_example_beginner.cpp
SOURCES += ../../coding_rains/source/rains_example_junior.cpp
SOURCES += ../../coding_rains/source/rains_example_senior.cpp

SOURCES += ../../coding_tiles/source/tiles.cpp
HEADERS += ../../coding_tiles/source/tiles.hpp
SOURCES += ../../coding_tiles/source/tiles_example.cpp
HEADERS += ../../coding_tiles/source/tiles_example.hpp
SOURCES += ../../coding_tiles/source/tiles_example_beginner.cpp
SOURCES += ../../coding_tiles/source/tiles_example_junior.cpp
SOURCES += ../../coding_tiles/source/tiles_example_senior.cpp

SOURCES += ../../coding_polygons/source/polygons.cpp
HEADERS += ../../coding_polygons/source/polygons.hpp
SOURCES += ../../coding_polygons/source/polygons_example.cpp
HEADERS += ../../coding_polygons/source/polygons_example.hpp
SOURCES += ../../coding_polygons/source/polygons_example_beginner.cpp
SOURCES += ../../coding_polygons/source/polygons_example_junior.cpp
SOURCES += ../../coding_polygons/source/polygons_example_senior.cpp

SOURCES += ../../coding_bullets/source/bullets.cpp
HEADERS += ../../coding_bullets/source/bullets.hpp
SOURCES += ../../coding_bullets/source/bullets_example.cpp
HEADERS += ../../coding_bullets/source/bullets_example.hpp
SOURCES += ../../coding_bullets/source/bullets_example_beginner.cpp
SOURCES += ../../coding_bullets/source/bullets_example_junior.cpp
SOURCES += ../../coding_bullets/source/bullets_example_senior.cpp

SOURCES += ../../coding_clouds/source/clouds.cpp
HEADERS += ../../coding_clouds/source/clouds.hpp
SOURCES += ../../coding_clouds/source/clouds_example.cpp
HEADERS += ../../coding_clouds/source/clouds_example.hpp
SOURCES += ../../coding_clouds/source/clouds_example_beginner.cpp
SOURCES += ../../coding_clouds/source/clouds_example_junior.cpp
SOURCES += ../../coding_clouds/source/clouds_example_senior.cpp

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
    QMAKE_BUNDLE_DATA += PLIST_RESOURCE
    QMAKE_INFO_PLIST = $$PWD/../common/ios/Info.plist
    PLIST_RESOURCE.files += $$PWD/../common/ios/res/icon.icns
    ios: PLIST_RESOURCE.path = /
    macx: PLIST_RESOURCE.path = /Contents/Resources
}
