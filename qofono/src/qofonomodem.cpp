/****************************************************************************
**
** Copyright (C) 2013-2015 Jolla Ltd.
** Contact: lorn.potter@jollamobile.com
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
****************************************************************************/

#include "qofonomodem.h"
#include "qofonomanager.h"
#include "ofono_modem_interface.h"

#include <QMap>
#include <QWeakPointer>

typedef QMap<QString,QWeakPointer<QOfonoModem> > ModemMap;
Q_GLOBAL_STATIC(ModemMap, modemMap)

#define SUPER QOfonoObject

class QOfonoModem::Private : public SUPER::ExtData
{
public:
    bool modemPathValid;
    QSharedPointer<QOfonoManager> mgr;
    Private() : modemPathValid(false), mgr(QOfonoManager::instance()) {}
};

QOfonoModem::QOfonoModem(QObject *parent) :
    SUPER(new Private, parent)
{
    QOfonoManager* mgr = privateData()->mgr.data();
    connect(mgr, SIGNAL(availableChanged(bool)), SLOT(checkModemPathValidity()));
    connect(mgr, SIGNAL(modemsChanged(QStringList)), SLOT(checkModemPathValidity()));
    checkModemPathValidity();
}

QOfonoModem::~QOfonoModem()
{
}

QDBusAbstractInterface *QOfonoModem::createDbusInterface(const QString &path)
{
    return new OfonoModem("org.ofono", path, QDBusConnection::systemBus(), this);
}

void QOfonoModem::objectPathChanged(const QString &path, const QVariantMap *properties)
{
    Q_EMIT modemPathChanged(path);
    if (!checkModemPathValidity()) {
        // checkModemPathValidity() didn't do anything because modemPathValid
        // flag hasn't changed. If the modem path has changed from one valid
        // path to another, D-Bus interface has to be re-initialized.
        if (privateData()->modemPathValid) {
            resetDbusInterface(properties);
        }
    }
}

void QOfonoModem::setModemPath(const QString &path)
{
    setObjectPath(path);
}

QString QOfonoModem::modemPath() const
{
    return objectPath();
}

bool QOfonoModem::powered() const
{
    return getBool("Powered");
}

bool QOfonoModem::online() const
{
    return getBool("Online");
}

bool QOfonoModem::lockdown() const
{
    return getBool("Lockdown");
}

bool QOfonoModem::emergency() const
{
    return getBool("Emergency");
}

QString QOfonoModem::name() const
{
    return getString("Name");
}

QString QOfonoModem::manufacturer() const
{
    return getString("Manufacturer");
}

QString QOfonoModem::model() const
{
    return getString("Model");
}

QString QOfonoModem::revision() const
{
    return getString("Revision");
}

QString QOfonoModem::serial() const
{
    return getString("Serial");
}

QString QOfonoModem::type() const
{
    return getString("Type");
}

QStringList QOfonoModem::features() const
{
    return getStringList("Features");
}

QStringList QOfonoModem::interfaces() const
{
    return getStringList("Interfaces");
}

void QOfonoModem::setPowered(bool powered)
{
    setProperty("Powered", powered);
}

void QOfonoModem::setOnline(bool online)
{
    setProperty("Online", online);
}

void QOfonoModem::setLockdown(bool lockdown)
{
    setProperty("Lockdown", lockdown);
}

void QOfonoModem::propertyChanged(const QString &property, const QVariant &value)
{
    SUPER::propertyChanged(property, value);
    if (property == QLatin1String("Online")) {
        Q_EMIT onlineChanged(value.value<bool>());
    } else if (property == QLatin1String("Powered")) {
        Q_EMIT poweredChanged(value.value<bool>());
    } else if (property == QLatin1String("Lockdown")) {
        Q_EMIT lockdownChanged(value.value<bool>());
    } else if (property == QLatin1String("Emergency")) {
        Q_EMIT emergencyChanged(value.value<bool>());
    } else if (property == QLatin1String("Name")) {
        Q_EMIT nameChanged(value.value<QString>());
    } else if (property == QLatin1String("Manufacturer")) {
        Q_EMIT manufacturerChanged(value.value<QString>());
    } else if (property == QLatin1String("Model")) {
        Q_EMIT modelChanged(value.value<QString>());
    } else if (property == QLatin1String("Revision")) {
        Q_EMIT revisionChanged(value.value<QString>());
    } else if (property == QLatin1String("Serial")) {
        Q_EMIT serialChanged(value.value<QString>());
    } else if (property == QLatin1String("Type")) {
        Q_EMIT typeChanged(value.value<QString>());
    } else if (property == QLatin1String("Features")) {
        Q_EMIT featuresChanged(value.value<QStringList>());
    } else if (property == QLatin1String("Interfaces")) {
        Q_EMIT interfacesChanged(value.value<QStringList>());
    }
}

QSharedPointer<QOfonoModem> QOfonoModem::instance(const QString &modemPath)
{
    QSharedPointer<QOfonoModem> modem = modemMap()->value(modemPath);
    if (modem.isNull()) {
        modem = QSharedPointer<QOfonoModem>::create();
        modem->fixObjectPath(modemPath);
        modemMap()->insert(modemPath, QWeakPointer<QOfonoModem>(modem));
    }
    return modem;
}

bool QOfonoModem::isValid() const
{
    return SUPER::isValid() && privateData()->modemPathValid;
}

QOfonoModem::Private* QOfonoModem::privateData() const
{
    return (Private*)SUPER::extData();
}

bool QOfonoModem::checkModemPathValidity()
{
    ValidTracker valid(this);
    bool modemPathValid;
    Private* priv = privateData();
    if (priv->mgr->isValid()) {
        QString path = modemPath();
        modemPathValid = !path.isEmpty() && priv->mgr->modems().contains(path);
    } else {
        modemPathValid = false;
    }
    if (priv->modemPathValid != modemPathValid) {
        priv->modemPathValid = modemPathValid;
        if (modemPathValid) {
            resetDbusInterface();
        } else {
            setDbusInterface(NULL);
        }
        return true;
    } else {
        // Return false to indicate that this function didn't do anything
        // because modemPathValid hasn't changed
        return false;
    }
}
