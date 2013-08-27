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

import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0
import Hawaii.Shell.Desktop 0.1
import Hawaii.Shell.Styles 0.1
import FluidUi 0.2 as FluidUi

Item {
    width: 400
    height: 300
    opacity: 0.0

    property var control: Shell.service("VolumeControl")

    Behavior on opacity {
        NumberAnimation { duration: 250 }
    }

    Timer {
        id: timer
        repeat: false
        interval: 2000
        onTriggered: opacity = 0.0
    }

    Binding {
        target: progressBar
        property: "value"
        value: control.muted ? 0 : control.volume
    }

    Binding {
        target: icon
        property: "iconName"
        value: {
            if (control.muted)
                return "audio-volume-muted-symbolic";

            var n = Math.floor(3 * control.volume / 100) + 1;
            if (n < 2)
                return "audio-volume-low-symbolic";
            if (n >= 3)
                return "audio-volume-high-symbolic"
            return "audio-volume-medium-symbolic";
        }
    }

    Connections {
        target: control
        onVolumeChanged: {
            opacity = 1.0;
            timer.restart();
        }
    }

    StyledItem {
        id: styledItem
        anchors.fill: parent
        style: Qt.createComponent("OverlayStyle.qml", styledItem)

        ColumnLayout {
            anchors {
                fill: parent
                margins: 20
            }

            FluidUi.Icon {
                id: icon
                color: "white"
                width: 192
                height: 192

                Layout.alignment: Qt.AlignCenter
            }

            ProgressBar {
                id: progressBar
                minimumValue: 0
                maximumValue: 100

                Behavior on value {
                    NumberAnimation { duration: 250 }
                }

                Layout.fillWidth: true
                Layout.alignment: Qt.AlignVCenter
            }
        }
    }
}
