/****************************************************************************
 * This file is part of Hawaii Shell.
 *
 * Copyright (C) 2013 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * Author(s):
 *    Pier Luigi Fiorini
 *
 * $BEGIN_LICENSE:LGPL2.1+$
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 2.1 of the License, or
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

#include "window.h"
#include "window_p.h"

/*
 * WindowPrivate
 */

WindowPrivate::WindowPrivate()
    : QtWayland::hawaii_window()
    , q_ptr(0)
    , state(Window::Inactive)
{
}

WindowPrivate::~WindowPrivate()
{
    hawaii_window_destroy(object());
}

void WindowPrivate::hawaii_window_title_changed(const QString &title)
{
    Q_Q(Window);

    if (this->title != title) {
        this->title = title;
        Q_EMIT q->titleChanged(this->title);
    }
}

void WindowPrivate::hawaii_window_identifier_changed(const QString &identifier)
{
    Q_Q(Window);

    if (this->identifier != identifier) {
        this->identifier = identifier;
        Q_EMIT q->identifierChanged(this->identifier);
    }
}

void WindowPrivate::hawaii_window_state_changed(int32_t state)
{
    Q_Q(Window);

    if (this->state != wlStateConvert(state)) {
        this->state = wlStateConvert(state);
        Q_EMIT q->stateChanged(this->state);
    }
}

void WindowPrivate::hawaii_window_unmapped()
{
    Q_Q(Window);

    Q_EMIT q->unmapped(q);
    q->deleteLater();
}

/*
 * Window
 */

Window::Window(const QString &title, const QString &identifier, States state, QObject *parent)
    : QObject(parent)
    , d_ptr(new WindowPrivate())
{
    Q_D(Window);
    d->q_ptr = this;
    d->title = title;
    d->identifier = identifier;
    d->state = state;
}

Window::Window(QObject *parent)
    : QObject(parent)
    , d_ptr(new WindowPrivate())
{
    Q_D(Window);
    d->q_ptr = this;
}

Window::~Window()
{
    delete d_ptr;
}

QString Window::title() const
{
    Q_D(const Window);
    return d->title;
}

QString Window::identifier() const
{
    Q_D(const Window);
    return d->identifier;
}

Window::States Window::state() const
{
    Q_D(const Window);
    return d->state;
}

void Window::setState(const States &state)
{
    Q_D(Window);
    d->state = state;
    d->set_state(stateConvert(d->state));
}

void Window::activate()
{
    Q_D(Window);

    if (!(d->state & Window::Active))
        setState(d->state | Window::Active);
}

void Window::minimize()
{
    Q_D(Window);

    if (!(d->state & Window::Minimized))
        setState(d->state | Window::Minimized);
}

void Window::unminimize()
{
    Q_D(Window);

    if (d->state & Window::Minimized)
        setState(d->state & ~Window::Minimized);
}

void Window::maximize()
{
    Q_D(Window);

    if (!(d->state & Window::Maximized))
        setState(d->state | Window::Maximized);
}

void Window::restore()
{
    Q_D(Window);

    if (d->state & Window::Maximized)
        setState(d->state & ~Window::Maximized);
}

#include "moc_window.cpp"