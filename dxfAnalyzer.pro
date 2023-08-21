QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dxflib/dl_dxf.cpp \
    dxflib/dl_writer_ascii.cpp \
    dxfreader.cpp \
    interactiveview.cpp \
    main.cpp \
    widget.cpp

HEADERS += \
    dxfreader.h \
    interactiveview.h \
    widget.h\
    dxflib/dl_attributes.h \
    dxflib/dl_codes.h \
    dxflib/dl_creationadapter.h \
    dxflib/dl_creationinterface.h \
    dxflib/dl_dxf.h \
    dxflib/dl_entities.h \
    dxflib/dl_exception.h \
    dxflib/dl_extrusion.h \
    dxflib/dl_global.h \
    dxflib/dl_writer.h \
    dxflib/dl_writer_ascii.h \

FORMS += \
    widget.ui

TRANSLATIONS += \
    dxfAnalyzer_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
