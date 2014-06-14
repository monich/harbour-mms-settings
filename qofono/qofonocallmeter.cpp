/****************************************************************************
**
** Copyright (C) 2013 Jolla Ltd.
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

#include "qofonocallmeter.h"
#include "dbus/ofonocallmeter.h"

class QOfonoCallMeterPrivate
{
public:
    QOfonoCallMeterPrivate();
    QString modemPath;
    OfonoCallMeter *callMeter;
    QVariantMap properties;

};

QOfonoCallMeterPrivate::QOfonoCallMeterPrivate() :
    modemPath(QString())
  , callMeter(0)
{
}

QOfonoCallMeter::QOfonoCallMeter(QObject *parent) :
    QObject(parent)
  , d_ptr(new QOfonoCallMeterPrivate)
{
}

QOfonoCallMeter::~QOfonoCallMeter()
{
    delete d_ptr;
}

void QOfonoCallMeter::setModemPath(const QString &path)
{
    if (path == d_ptr->modemPath ||
            path.isEmpty())
        return;

    if (path != modemPath()) {
        if (d_ptr->callMeter) {
            delete d_ptr->callMeter;
            d_ptr->callMeter = 0;
            d_ptr->properties.clear();
        }
        d_ptr->callMeter = new OfonoCallMeter("org.ofono", path, QDBusConnection::systemBus(),this);

        if (d_ptr->callMeter->isValid()) {
            d_ptr->modemPath = path;
            connect(d_ptr->callMeter,SIGNAL(PropertyChanged(QString,QDBusVariant)),
                    this,SLOT(propertyChanged(QString,QDBusVariant)));

            connect(d_ptr->callMeter,SIGNAL(NearMaximumWarning()),this,SIGNAL(nearMaximumWarning()));
            QDBusPendingReply<QVariantMap> reply;
            reply = d_ptr->callMeter->GetProperties();
            reply.waitForFinished();
            d_ptr->properties = reply.value();
            Q_EMIT modemPathChanged(path);
        }
    }
}

QString QOfonoCallMeter::modemPath() const
{
    return d_ptr->modemPath;
}


void QOfonoCallMeter::propertyChanged(const QString& property, const QDBusVariant& dbusvalue)
{
    QVariant value = dbusvalue.variant();
    d_ptr->properties.insert(property,value);

    if (property == QLatin1String("AccumulatedCallMeterMaximum")) {
        Q_EMIT accumulatedCallMeterMaximumChanged(value.value<quint32>());
    } else if (property == QLatin1String("PricePerUnit")) {
        Q_EMIT pricePerUnitChanged(value.value<qreal>());
    }
}

quint32 QOfonoCallMeter::callMeter() const
{
    if (d_ptr->callMeter)
        return d_ptr->properties["CallMeter"].value<quint32>();
    else
        return 0;
}

quint32 QOfonoCallMeter::accumulatedCallMeter() const
{
    if (d_ptr->callMeter)
        return d_ptr->properties["AccumulatedCallMeter"].value<quint32>();
    else
        return 0;
}

quint32 QOfonoCallMeter::accumulatedCallMeterMaximum() const
{
    if (d_ptr->callMeter)
        return d_ptr->properties["AccumulatedCallMeterMaximum"].value<quint32>();
    else
        return 0;
}

void QOfonoCallMeter::setAccumulatedCallMeterMaximum(quint32 max, const QString &password)
{
    if (d_ptr->callMeter) {
        QVariantList arguments;
        arguments << QVariant(max);
        d_ptr->callMeter->SetProperty("AccumulatedCallMeterMaximum",QDBusVariant(arguments), password);
    }
}


qreal QOfonoCallMeter::pricePerUnit() const
{
    if (d_ptr->callMeter)
        return d_ptr->properties["PricePerUnit"].value<qreal>();
    else
        return 0;
}

void QOfonoCallMeter::setPricePerUnit(qreal unit, const QString &password)
{
    if (d_ptr->callMeter) {
        QVariantList arguments;
        arguments << QVariant(unit);
        d_ptr->callMeter->SetProperty("PricePerUnit",QDBusVariant(arguments), password);
    }
}

QString QOfonoCallMeter:: currency() const
{
    if (d_ptr->callMeter)
        return d_ptr->properties["Currency"].value<QString>();
    else
        return QString();
}

void QOfonoCallMeter::reset(const QString &password)
{
    if (d_ptr->callMeter) {
    QDBusPendingReply<> result = d_ptr->callMeter->Reset(password);
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(result, this);
    connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
            SLOT(resetFinished(QDBusPendingCallWatcher*)));
    }
}

bool QOfonoCallMeter::isValid() const
{
    return d_ptr->callMeter->isValid();
}

void QOfonoCallMeter::resetFinished(QDBusPendingCallWatcher *call)
{
    QDBusPendingReply<> reply = *call;
    QOfonoCallMeter::Error error = NoError;
    QString errorString;

    if (reply.isError()) {
        qWarning() << "QOfonoCallMeter::reset() failed:" << reply.error();
        error = errorNameToEnum(reply.error().name());
        errorString = reply.error().name() + " " + reply.error().message();
        Q_EMIT resetComplete(error,errorString);
    }
}

QOfonoCallMeter::Error QOfonoCallMeter::errorNameToEnum(const QString &errorName)
{
    if (errorName == "")
        return NoError;
    else if (errorName == "org.ofono.Error.NotImplemented")
        return NotImplementedError;
    else if (errorName == "org.ofono.Error.InProgress")
        return InProgressError;
    else if (errorName == "org.ofono.Error.InvalidArguments")
        return InvalidArgumentsError;
    else if (errorName == "org.ofono.Error.InvalidFormat")
        return InvalidFormatError;
    else if (errorName == "org.ofono.Error.Failed")
        return FailedError;
    else
        return UnknownError;
}
