#-------------------------------------------------
#
# Project created by QtCreator 2017-06-23T14:05:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TexturePacker
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        MainWindow.cpp \
    TPProject.cpp \
    utils/Utils.cpp \
    packer/ImagePacker.cpp \
    packer/MaxRects.cpp \
    packer/BinPack.cpp \
    views/DataSettingView.cpp \
    views/TextureSettingView.cpp \
    views/SpriteSettingView.cpp \
    views/SettingPanel.cpp \
    AtlasTextureView.cpp

HEADERS += \
        MainWindow.hpp \
    TPProject.hpp \
    utils/Utils.hpp \
    packer/ImagePacker.hpp \
    packer/Packer.hpp \
    packer/MaxRects.hpp \
    packer/BinPack.hpp \
    views/DataSettingView.hpp \
    views/TextureSettingView.hpp \
    views/SpriteSettingView.hpp \
    views/SettingPanel.hpp \
    AtlasTextureView.hpp

RESOURCES += \
    Resources.qrc
