/*
 * Copyright (C) 2016 Canonical, Ltd.
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 3, as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranties of MERCHANTABILITY,
 * SATISFACTORY QUALITY, or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef UNITY_MENU_REGISTRY_H
#define UNITY_MENU_REGISTRY_H

#include <QObject>
#include <QScopedPointer>

class IoUnity8MenuRegistrarInterface;
class QDBusObjectPath;
class QDBusServiceWatcher;

class UnityMenuRegistry : public QObject
{
    Q_OBJECT
public:
    UnityMenuRegistry(QObject* parent = nullptr);
    virtual ~UnityMenuRegistry();

    static UnityMenuRegistry *instance();

    void registerApplicationMenu(pid_t pid, QDBusObjectPath menuObjectPath, const QString &service);
    void unregisterApplicationMenu(pid_t pid, QDBusObjectPath menuObjectPath);

    void registerSurfaceMenu(const QString &surfaceId, QDBusObjectPath menuObjectPath, const QString &service);
    void unregisterSurfaceMenu(const QString &surfaceId, QDBusObjectPath menuObjectPath);

    bool isConnected() const { return m_connected; }

Q_SIGNALS:
    void serviceChanged();

private Q_SLOTS:
    void serviceOwnerChanged(const QString &serviceName, const QString& oldOwner, const QString &newOwner);

private:
    QScopedPointer<QDBusServiceWatcher> m_serviceWatcher;
    QScopedPointer<IoUnity8MenuRegistrarInterface> m_interface;
    bool m_connected;
};

#endif // UNITY_MENU_REGISTRY_H
