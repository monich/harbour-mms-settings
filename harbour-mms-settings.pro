TARGET = harbour-mms-settings

QT += dbus xml
CONFIG += link_pkgconfig
CONFIG += sailfishapp
PKGCONFIG += mlite5
QMAKE_CXXFLAGS += -Wno-unused-parameter

SOURCES += \
    src/main.cpp \
    src/ConfigValue.cpp \
    src/MmsEngine.cpp

HEADERS += \
    src/MmsEngine.h \
    src/ConfigDebug.h \
    src/ConfigValue.h

OTHER_FILES += \
    qml/main.qml \
    qml/cover/*.qml \
    qml/pages/*.qml \
    qml/cover/*.svg \
    qml/images/* \
    rpm/harbour-mms-settings.changes \
    rpm/harbour-mms-settings.spec \
    translations/*.ts \
    harbour-mms-settings.desktop

# Icons
TARGET_ICON_ROOT = /usr/share/icons/hicolor

icon86.files = icons/86x86/$${TARGET}.png
icon86.path = $$TARGET_ICON_ROOT/86x86/apps
INSTALLS += icon86

icon108.files = icons/108x108/$${TARGET}.png
icon108.path = $$TARGET_ICON_ROOT/108x108/apps
INSTALLS += icon108

icon128.files = icons/128x128/$${TARGET}.png
icon128.path = $$TARGET_ICON_ROOT/128x128/apps
INSTALLS += icon128

icon256.files = icons/256x256/$${TARGET}.png
icon256.path = $$TARGET_ICON_ROOT/256x256/apps
INSTALLS += icon256

# qofono
QOFONO_SRC=qofono/src
QOFONO_PLUGIN=qofono/plugin
QOFONO_DBUS=$$QOFONO_SRC/dbus

INCLUDEPATH += $$QOFONO_SRC $$QOFONO_PLUGIN

SOURCES += \
    $$QOFONO_SRC/qofonodbustypes.cpp \
    $$QOFONO_SRC/qofonomanager.cpp \
    $$QOFONO_SRC/qofonomodem.cpp \
    $$QOFONO_SRC/qofonomodeminterface.cpp \
    $$QOFONO_SRC/qofonoobject.cpp \
    $$QOFONO_SRC/qofonosimmanager.cpp \
    $$QOFONO_SRC/qofonosimwatcher.cpp \
    $$QOFONO_PLUGIN/qofonosimlistmodel.cpp

HEADERS += \
    $$QOFONO_SRC/qofonomanager.h \
    $$QOFONO_SRC/qofonomodem.h \
    $$QOFONO_SRC/qofonomodeminterface.h \
    $$QOFONO_SRC/qofonoobject.h \
    $$QOFONO_SRC/qofonosimmanager.h \
    $$QOFONO_SRC/qofonosimwatcher.h \
    $$QOFONO_PLUGIN/qofonosimlistmodel.h

DBUS_INTERFACES += ofono_manager
ofono_manager.files = $$QOFONO_DBUS/ofono_manager.xml
ofono_manager.header_flags = -N -c OfonoManager -i dbustypes.h
ofono_manager.source_flags = -N -c OfonoManager

DBUS_INTERFACES += ofono_modem
ofono_modem.files = $$QOFONO_DBUS/ofono_modem.xml
ofono_modem.header_flags = -N -c OfonoModem
ofono_modem.source_flags = -N -c OfonoModem

DBUS_INTERFACES += ofono_simmanager
ofono_simmanager.files = $$QOFONO_DBUS/ofono_simmanager.xml
ofono_simmanager.header_flags = -N -c OfonoSimManager
ofono_simmanager.source_flags = -N -c OfonoSimManager

# to disable building translations every time, comment out the
# following CONFIG line
CONFIG += sailfishapp_i18n
TRANSLATIONS += \
    translations/harbour-mms-settings.ts \
    translations/harbour-mms-settings-ru.ts
