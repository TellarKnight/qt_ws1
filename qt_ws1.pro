QT       += core gui\
            network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 \
          qwt
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    obj_thread.cpp

HEADERS += \
    mainwindow.h \
    obj_thread.h

FORMS += \
    mainwindow.ui

INCLUDEPATH += /usr/local/qwt-6.1.6/include \
               /usr/local/include/opencv4 \
               /usr/local/include/opencv4/opencv2

LIBS += -L"/usr/local/qwt-6.1.6/lib" -lqwt \
       /usr/local/lib/libopencv_highgui.so \
       /usr/local/lib/libopencv_core.so    \
       /usr/local/lib/libopencv_imgproc.so \
       /usr/local/lib/libopencv_imgcodecs.so

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
