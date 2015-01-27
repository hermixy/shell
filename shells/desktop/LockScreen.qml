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

import QtQuick 2.4
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.0
import QtGraphicalEffects 1.0
import Hawaii.Components 1.0 as Components
import Hawaii.Themes 1.0 as Themes

Components.Showable {
    signal unlocked()

    id: root
    showAnimation: OpacityAnimator {
        target: root
        easing.type: Easing.InSine
        duration: Themes.Units.longDuration
        from: 0.0
        to: 1.0
    }
    hideAnimation: OpacityAnimator {
        target: root
        easing.type: Easing.InSine
        duration: Themes.Units.longDuration
        from: 1.0
        to: 0.0
    }
    opacity: 0.0
    visible: true

    QtObject {
        id: __priv

        property string timeFormat
    }

    Image {
        id: picture
        anchors.fill: parent
        source: "/usr/share/backgrounds/gnome/Stones.jpg"
        sourceSize.width: width * 0.75
        sourceSize.height: height * 0.75
        fillMode: Image.PreserveAspectCrop
        cache: false
        visible: false
    }

    FastBlur {
        anchors.fill: picture
        source: picture
        radius: 32
    }

    Timer {
        id: timer
        running: false
        triggeredOnStart: true
        interval: 30000
        onTriggered: {
            var now = new Date();
            timeLabel.text = Qt.formatTime(now, __priv.timeFormat);
            dateLabel.text = Qt.formatDate(now, Locale.LongFormat);
        }
    }

    ColumnLayout {
        anchors.fill: parent

        Item {
            Layout.fillHeight: true
        }

        Label {
            id: timeLabel
            font.pointSize: 42
            color: Themes.Theme.palette.window.textColor
            style: Text.Raised
            styleColor: Themes.Theme.palette.window.textEffectColor

            Layout.alignment: Qt.AlignCenter
        }

        Label {
            id: dateLabel
            font.pointSize: 36
            color: Themes.Theme.palette.window.textColor
            style: Text.Raised
            styleColor: Themes.Theme.palette.window.textEffectColor

            Layout.alignment: Qt.AlignCenter
        }

        Item {
            Layout.fillHeight: true
        }

        Rectangle {
            color: "#80000000"

            TextField {
                id: passwordField
                anchors.centerIn: parent
                placeholderText: qsTr("Password")
                width: Themes.Units.gu(20)
                focus: true
                echoMode: TextInput.Password
                onAccepted: root.unlocked()
            }

            Layout.fillWidth: true
            Layout.preferredHeight: Themes.Units.gu(5)
        }

        Item {
            Layout.fillHeight: true
        }
    }

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.AllButtons
        onClicked: passwordField.forceActiveFocus()
    }

    Component.onCompleted: {
        // Remove seconds from time format
        __priv.timeFormat = Qt.locale().timeFormat(Locale.ShortFormat).replace(/.ss?/i, "");

        // Start timer
        timer.start();

        // Activate password field
        passwordField.forceActiveFocus();
    }
}
