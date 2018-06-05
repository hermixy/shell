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

#include <QWaylandCompositor>
#include <QtWaylandCompositor/private/qwaylandcompositor_p.h>

#include "declarative/screenmodel.h"
#include "extensions/outputdevice.h"
#include "extensions/outputdevice_p.h"

/*
 * OutputDevicePrivate
 */

OutputDevicePrivate::OutputDevicePrivate()
    : QtWaylandServer::org_kde_kwin_outputdevice()
{
}

int32_t OutputDevicePrivate::toSubpixel() const
{
    switch (subpixel) {
    case QWaylandOutput::SubpixelUnknown:
        return subpixel_unknown;
    case QWaylandOutput::SubpixelNone:
        return subpixel_none;
    case QWaylandOutput::SubpixelHorizontalRgb:
        return subpixel_horizontal_rgb;
    case QWaylandOutput::SubpixelHorizontalBgr:
        return subpixel_horizontal_bgr;
    case QWaylandOutput::SubpixelVerticalRgb:
        return subpixel_vertical_rgb;
    case QWaylandOutput::SubpixelVerticalBgr:
        return subpixel_vertical_bgr;
    }
    abort();
}

int32_t OutputDevicePrivate::toTransform() const
{
    switch (transform) {
    case QWaylandOutput::TransformNormal:
        return transform_normal;
    case QWaylandOutput::Transform90:
        return transform_90;
    case QWaylandOutput::Transform180:
        return transform_180;
    case QWaylandOutput::Transform270:
        return transform_270;
    case QWaylandOutput::TransformFlipped:
        return transform_flipped;
    case QWaylandOutput::TransformFlipped90:
        return transform_flipped_90;
    case QWaylandOutput::TransformFlipped180:
        return transform_flipped_180;
    case QWaylandOutput::TransformFlipped270:
        return transform_flipped_270;
    }
    abort();
}

void OutputDevicePrivate::sendGeometry(wl_resource *resource)
{
    send_geometry(resource,
                  position.x(), position.y(),
                  physicalSize.width(), physicalSize.height(),
                  toSubpixel(), manufacturer, model, toTransform());
}

void OutputDevicePrivate::sendModes(wl_resource *resource)
{
    int index = 0;

    for (ScreenMode *mode : modes) {
        quint32 flags = 0;
        if (currentModeIndex == index)
            flags |= mode_current;
        if (preferredModexIndex == index)
            flags |= mode_preferred;

        send_mode(resource, flags, mode->resolution().width(), mode->resolution().height(),
                  mode->refreshRate(), index);
        index++;
    }
}

void OutputDevicePrivate::updateGeometry()
{
    for (const Resource *resource : resourceMap().values()) {
        sendGeometry(resource->handle);
        send_done(resource->handle);
    }
}

void OutputDevicePrivate::updateScale()
{
    for (const Resource *resource : resourceMap().values()) {
        send_scale(resource->handle, scaleFactor);
        send_done(resource->handle);
    }
}

void OutputDevicePrivate::updateModes()
{
    for (const Resource *resource : resourceMap().values()) {
        sendModes(resource->handle);
        send_done(resource->handle);
    }
}

void OutputDevicePrivate::org_kde_kwin_outputdevice_bind_resource(QtWaylandServer::org_kde_kwin_outputdevice::Resource *resource)
{
    sendGeometry(resource->handle);
    send_scale(resource->handle, scaleFactor);
    send_uuid(resource->handle, uuid);
    send_edid(resource->handle, edid);
    send_enabled(resource->handle, enabled ? enablement_enabled : enablement_disabled);
    sendModes(resource->handle);
    send_done(resource->handle);
}

/*
 * OutputDevice
 */

OutputDevice::OutputDevice()
    : QWaylandCompositorExtension()
    , d_ptr(new OutputDevicePrivate())
{
}

OutputDevice::OutputDevice(QWaylandCompositor *compositor)
    : QWaylandCompositorExtension(compositor)
    , d_ptr(new OutputDevicePrivate())
{
    d_ptr->compositor = compositor;
    QWaylandCompositorPrivate::get(compositor)->addPolishObject(this);
}

OutputDevice::~OutputDevice()
{
    delete d_ptr;
}

QWaylandCompositor *OutputDevice::compositor() const
{
    Q_D(const OutputDevice);
    return d->compositor;
}

void OutputDevice::setCompositor(QWaylandCompositor *compositor)
{
    Q_D(OutputDevice);

    if (d->compositor == compositor)
        return;

    if (d->initialized) {
        qWarning("Setting QWaylandCompositor %p on OutputDevice %p is not supported after OutputDevice has been initialized", compositor, this);
        return;
    }

    if (d->compositor && d->compositor != compositor)
        qWarning("Possible initialization error: moving OutputDevice %p between compositor instances", this);

    d->compositor = compositor;
    setExtensionContainer(d->compositor);
    QWaylandCompositorPrivate::get(compositor)->addPolishObject(this);

    Q_EMIT compositorChanged();
}

QString OutputDevice::uuid() const
{
    Q_D(const OutputDevice);
    return d->uuid;
}

void OutputDevice::setUuid(const QString &uuid)
{
    Q_D(OutputDevice);

    if (d->uuid == uuid)
        return;

    if (d->initialized) {
        qWarning("Setting OutputDevice::uuid after initialization is not possible");
        return;
    }

    d->uuid = uuid;
    Q_EMIT uuidChanged();
}

bool OutputDevice::isEnabled() const
{
    Q_D(const OutputDevice);
    return d->enabled;
}

void OutputDevice::setEnabled(bool enabled)
{
    Q_D(OutputDevice);

    if (d->enabled == enabled)
        return;

    d->enabled = enabled;
    d->send_enabled(enabled ? d->enablement_enabled : d->enablement_disabled);
    Q_EMIT enabledChanged();
}

QString OutputDevice::manufacturer() const
{
    Q_D(const OutputDevice);
    return d->manufacturer;
}

void OutputDevice::setManufacturer(const QString &manufacturer)
{
    Q_D(OutputDevice);

    if (d->manufacturer == manufacturer)
        return;

    d->manufacturer = manufacturer;
    d->updateGeometry();
    Q_EMIT manufacturerChanged();
}

QString OutputDevice::model() const
{
    Q_D(const OutputDevice);
    return d->model;
}

void OutputDevice::setModel(const QString &model)
{
    Q_D(OutputDevice);

    if (d->model == model)
        return;

    d->model = model;
    d->updateGeometry();
    Q_EMIT modelChanged();
}

QString OutputDevice::edid() const
{
    Q_D(const OutputDevice);
    return d->edid;
}

void OutputDevice::setEdid(const QString &edid)
{
    Q_D(OutputDevice);

    if (d->edid == edid)
        return;

    d->edid = edid;
    d->send_edid(edid);
    d->send_done();
    Q_EMIT edidChanged();
}

QSize OutputDevice::physicalSize() const
{
    Q_D(const OutputDevice);
    return d->physicalSize;
}

void OutputDevice::setPhysicalSize(const QSize &size)
{
    Q_D(OutputDevice);

    if (d->physicalSize == size)
        return;

    d->physicalSize = size;
    Q_EMIT physicalSizeChanged();
}

QPoint OutputDevice::position() const
{
    Q_D(const OutputDevice);
    return d->position;
}

void OutputDevice::setPosition(const QPoint &position)
{
    Q_D(OutputDevice);

    if (d->position == position)
        return;

    d->position = position;
    Q_EMIT positionChanged();
}

QWaylandOutput::Subpixel OutputDevice::subpixel() const
{
    Q_D(const OutputDevice);
    return d->subpixel;
}

void OutputDevice::setSubpixel(const QWaylandOutput::Subpixel &subpixel)
{
    Q_D(OutputDevice);

    if (d->subpixel == subpixel)
        return;

    d->subpixel = subpixel;
    d->updateGeometry();
    Q_EMIT subpixelChanged();
}

QWaylandOutput::Transform OutputDevice::transform() const
{
    Q_D(const OutputDevice);
    return d->transform;
}

void OutputDevice::setTransform(const QWaylandOutput::Transform &transform)
{
    Q_D(OutputDevice);

    if (d->transform == transform)
        return;

    d->transform = transform;
    d->updateGeometry();
    Q_EMIT transformChanged();
}

int OutputDevice::scaleFactor() const
{
    Q_D(const OutputDevice);
    return d->scaleFactor;
}

void OutputDevice::setScaleFactor(int scale)
{
    Q_D(OutputDevice);

    if (d->scaleFactor == scale)
        return;

    d->scaleFactor = scale;
    d->updateScale();
    Q_EMIT scaleFactorChanged();
}

QQmlListProperty<ScreenMode> OutputDevice::screenModes()
{
    auto appendFunc = [](QQmlListProperty<ScreenMode> *prop, ScreenMode *mode) {
        auto output = static_cast<OutputDevice *>(prop->object);
        if (!output->d_func()->initialized)
            output->d_func()->modes.append(mode);
    };
    auto countFunc = [](QQmlListProperty<ScreenMode> *prop) {
        return static_cast<OutputDevice *>(prop->object)->d_func()->modes.count();
    };
    auto atFunc = [](QQmlListProperty<ScreenMode> *prop, int index) {
        return static_cast<OutputDevice *>(prop->object)->d_func()->modes.at(index);
    };
    auto clearFunc = [](QQmlListProperty<ScreenMode> *prop) {
        auto output = static_cast<OutputDevice *>(prop->object);
        if (!output->d_func()->initialized)
            output->d_func()->modes.clear();
    };
    return QQmlListProperty<ScreenMode>(this, this, appendFunc, countFunc, atFunc, clearFunc);
}

int OutputDevice::currentModeIndex() const
{
    Q_D(const OutputDevice);
    return d->currentModeIndex;
}

void OutputDevice::setCurrentModeIndex(int index)
{
    Q_D(OutputDevice);

    if (d->currentModeIndex == index)
        return;

    d->currentModeIndex = index;
    d->updateModes();
    Q_EMIT currentModeIndexChanged();
}

int OutputDevice::preferredModeIndex() const
{
    Q_D(const OutputDevice);
    return d->preferredModexIndex;
}

void OutputDevice::setPreferredModeIndex(int index)
{
    Q_D(OutputDevice);

    if (d->initialized) {
        qWarning("Setting OutputDevice::preferredModeIndex after initialization has no effect");
        return;
    }

    if (d->preferredModexIndex == index)
        return;

    d->preferredModexIndex = index;
    d->updateModes();
    Q_EMIT preferredModeIndexChanged();
}

void OutputDevice::initialize()
{
    Q_D(OutputDevice);

    Q_ASSERT(!d->initialized);

    for (QObject *p = parent(); p != nullptr; p = p->parent()) {
        if (auto c = qobject_cast<QWaylandCompositor *>(p)) {
            setCompositor(c);
            break;
        }
    }

    Q_ASSERT(d->compositor);
    Q_ASSERT(d->compositor->isCreated());

    d->init(d->compositor->display(), d->interfaceVersion());
    d->initialized = true;
}

const wl_interface *OutputDevice::extensionInterface() const
{
    Q_D(const OutputDevice);
    return d->interface();
}
