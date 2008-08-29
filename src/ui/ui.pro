#Configuration
TEMPLATE = app
TARGET = videoplayer
DESTDIR = ../../build
DEPENDPATH += .
INCLUDEPATH += ../../tmp

UI_DIR = ../../tmp
MOC_DIR = ../../tmp
OBJECTS_DIR = ../../tmp
RCC_DIR = ../../tmp

QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter -Wall
QT -= xml sql svg

# Include special application config


# external library
# including external headers.

CONFIG(designer) {
INCLUDEPATH += $$[QT_INSTALL_HEADERS]/neux
LIBS += -L$$[QT_INSTALL_PLUGINS]/designer/
} else {
INCLUDEPATH += /${TOOLCHAIN_USR_INSTALL}/include/neux
LIBS += -L${TOOLCHAIN_USR_INSTALL}/lib
}

LIBS += -lneux -lgui

# application resource files
RESOURCES += ../../resources/videoplayer.qrc

# add translation file
TRANSLATIONS+=

# application form files
FORMS += ../forms/nvideoplayer.ui

# application header files
HEADERS +=	nvideoplayer.h \
		nvplayerapp.h
	  

# application source files
SOURCES +=	nvideoplayer.cpp \
		nvplayerapp.cpp \	
		main.cpp

# Install
CONFIG(designer) {
} else {
rootfs.path = /${ROOTFS}/fs/usr/local/bin
rootfs.files = ../../build/*
INSTALLS += rootfs
}

# Clear

QMAKE_CLEAN += ../../build/* ../../tmp/* ./Makefile ./*~
