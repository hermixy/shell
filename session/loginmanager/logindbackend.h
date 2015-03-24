/****************************************************************************
 * This file is part of Hawaii.
 *
 * Copyright (C) 2015 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * Author(s):
 *    Pier Luigi Fiorini
 *
 * $BEGIN_LICENSE:GPL2+$
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * $END_LICENSE$
 ***************************************************************************/

#ifndef LOGINDBACKEND_H
#define LOGINDBACKEND_H

#include <QtCore/QLoggingCategory>
#include <QtDBus/QDBusConnection>

#include "loginmanagerbackend.h"

Q_DECLARE_LOGGING_CATEGORY(LOGIND_BACKEND)

class QDBusInterface;
class QDBusPendingCallWatcher;

class LogindBackend : public LoginManagerBackend
{
    Q_OBJECT
public:
    ~LogindBackend();

    static LogindBackend *create(const QDBusConnection &connection = QDBusConnection::systemBus());

    QString name() const;

    void setIdle(bool value);

    void lockSession();
    void unlockSession();

    void locked();
    void unlocked();

    void switchToVt(int index);

private:
    LogindBackend();

    QDBusInterface *m_interface;
    QString m_sessionPath;
    int m_inhibitFd;

    void setupInhibitors();

private Q_SLOTS:
    void prepareForSleep(bool arg);
    void prepareForShutdown(bool arg);
    void getSession(QDBusPendingCallWatcher *watcher);
};

#endif // LOGINDBACKEND_H