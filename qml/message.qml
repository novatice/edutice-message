import QtQuick
import QtQuick.Window
import QtQuick.Layouts
import QtQuick.Controls.Basic
import QtQuick.Dialogs
import QtQuick.Controls.Universal
import QtQml
import QtWebEngine
import AvenirFonts 1.0

Window {
    width: Screen.width
    height: Screen.height
    visible: true
    flags: Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint | Qt.WindowMaximized
    visibility: Qt.WindowMaximized
    Component.onCompleted: {
        console.log(Screen.width, Screen.height)
    }

    Shortcut {
        sequences: ["Esc", "Return"]
        enabled: true
        onActivated: {
            Qt.quit()
        }
    }

    Universal.theme: Universal.Dark
    Universal.accent: Universal.Violet

    ColumnLayout {
        spacing: 0
        width: parent.width
        height: parent.height

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            NeosWebEngine {}
        }

        Item {
            Layout.fillWidth: true
            implicitHeight: parent.height / 15
            visible: !withoutCloseButton

            Rectangle {
                color: "whitesmoke"
                width: parent.width
                height: parent.height
            }

            RowLayout {
                width: parent.width
                height: parent.height

                NeosButton {
                    Layout.alignment: Qt.AlignCenter

                    horizontalPadding: 60
                    text: "Fermer"

                    onClicked: {
                        Qt.quit()
                    }
                }
            }
        }
    }
}
