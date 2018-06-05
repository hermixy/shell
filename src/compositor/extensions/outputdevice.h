/****************************************************************************
 * This file is part of Liri.
 *
 * Copyright (C) 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * $BEGIN_LICENSE:LGPLv3+$
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * $END_LICENSE$
 ***************************************************************************/

#ifndef LIRI_OUTPUTDEVICE_H
#define LIRI_OUTPUTDEVICE_H

#include <QQmlListProperty>
#include <QPoint>
#include <QSize>
#include <QWaylandCompositorExtension>
#include <QWaylandOutput>

class OutputDevicePrivate;
class ScreenMode;

class OutputDevice : public QWaylandCompositorExtension
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(OutputDevice)
    Q_PROPERTY(QWaylandCompositor *compositor READ compositor WRITE setCompositor NOTIFY compositorChanged)
    Q_PROPERTY(QString uuid READ uuid NOTIFY uuidChanged)
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(QString manufacturer READ manufacturer WRITE setManufacturer NOTIFY manufacturerChanged)
    Q_PROPERTY(QString model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(QString edid READ edid WRITE setEdid NOTIFY edidChanged)
    Q_PROPERTY(QSize physicalSize READ physicalSize WRITE setPhysicalSize NOTIFY physicalSizeChanged)
    Q_PROPERTY(QPoint position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(QWaylandOutput::Subpixel subpixel READ subpixel WRITE setSubpixel NOTIFY subpixelChanged)
    Q_PROPERTY(QWaylandOutput::Transform transform READ transform WRITE setTransform NOTIFY transformChanged)
    Q_PROPERTY(int scaleFactor READ scaleFactor WRITE setScaleFactor NOTIFY scaleFactorChanged)
    Q_PROPERTY(QQmlListProperty<ScreenMode> modes READ screenModes NOTIFY modesChanged)
    Q_PROPERTY(int currentModeIndex READ currentModeIndex WRITE setCurrentModeIndex NOTIFY currentModeIndexChanged)
    Q_PROPERTY(int preferredModeIndex READ preferredModeIndex WRITE setPreferredModeIndex NOTIFY preferredModeIndexChanged)
public:
    explicit OutputDevice();
    OutputDevice(QWaylandCompositor *compositor);
    ~OutputDevice();

    QWaylandCompositor *compositor() const;
    void setCompositor(QWaylandCompositor *compositor);

    QString uuid() const;
    void setUuid(const QString &uuid);

    bool isEnabled() const;
    void setEnabled(bool enabled);

    QString manufacturer() const;
    void setManufacturer(const QString &manufacturer);

    QString model() const;
    void setModel(const QString &model);

    QString edid() const;
    void setEdid(const QString &edid);

    QSize physicalSize() const;
    void setPhysicalSize(const QSize &size);

    QPoint position() const;
    void setPosition(const QPoint &position);

    QWaylandOutput::Subpixel subpixel() const;
    void setSubpixel(const QWaylandOutput::Subpixel &subpixel);

    QWaylandOutput::Transform transform() const;
    void setTransform(const QWaylandOutput::Transform &transform);

    int scaleFactor() const;
    void setScaleFactor(int scale);

    QQmlListProperty<ScreenMode> screenModes();

    int currentModeIndex() const;
    void setCurrentModeIndex(int index);

    int preferredModeIndex() const;
    void setPreferredModeIndex(int index);

    void initialize() override;

    const wl_interface *extensionInterface() const override;

Q_SIGNALS:
    void compositorChanged();
    void uuidChanged();
    void enabledChanged();
    void manufacturerChanged();
    void modelChanged();
    void edidChanged();
    void physicalSizeChanged();
    void positionChanged();
    void subpixelChanged();
    void transformChanged();
    void scaleFactorChanged();
    void modesChanged();
    void currentModeIndexChanged();
    void preferredModeIndexChanged();

private:
    OutputDevicePrivate *const d_ptr;
};

#endif // LIRI_OUTPUTDEVICE_H
