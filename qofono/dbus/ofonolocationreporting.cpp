/*
 * This file was generated by qdbusxml2cpp version 0.7
 * Command line was: qdbusxml2cpp -c OfonoLocationReporting -p ofonolocationreporting ofono_location_reporting.xml
 *
 * qdbusxml2cpp is Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * This file may have been hand-edited. Look for HAND-EDIT comments
 * before re-generating it.
 */

#include "ofonolocationreporting.h"

/*
 * Implementation of interface class OfonoLocationReporting
 */

OfonoLocationReporting::OfonoLocationReporting(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent)
    : QDBusAbstractInterface(service, path, staticInterfaceName(), connection, parent)
{
}

OfonoLocationReporting::~OfonoLocationReporting()
{
}

