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

#ifndef LIRI_OUTPUTDEVICE_P_H
#define LIRI_OUTPUTDEVICE_P_H

#include "outputdevice.h"
#include "qwayland-server-outputdevice.h"

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Liri API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

class OutputDevicePrivate : public QtWaylandServer::org_kde_kwin_outputdevice
{
public:
    OutputDevicePrivate();

    int32_t toSubpixel() const;
    int32_t toTransform() const;

    void sendGeometry(wl_resource *resource);
    void sendModes(wl_resource *resource);
    void updateGeometry();
    void updateScale();
    void updateModes();

    bool initialized = false;
    QWaylandCompositor *compositor = nullptr;
    QString uuid;
    bool enabled = true;
    QString manufacturer = QLatin1String("Unknown");
    QString model = QLatin1String("Unknown");
    QString edid;
    QSize physicalSize;
    QPoint position;
    QWaylandOutput::Subpixel subpixel = QWaylandOutput::SubpixelUnknown;
    QWaylandOutput::Transform transform = QWaylandOutput::TransformNormal;
    int scaleFactor = 1;
    QVector<ScreenMode *> modes;
    int currentModeIndex = 0;
    int preferredModexIndex = 0;

protected:
    void org_kde_kwin_outputdevice_bind_resource(Resource *resource) override;
};

#endif // LIRI_OUTPUTDEVICE_P_H
