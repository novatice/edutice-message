import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.3
import QtQuick.Controls.Universal 2.12
import QtQml 2.12
import QtWebEngine 1.8
import AvenirFonts 1.0

Window {
    width: Screen.width
    height: Screen.height
    visible: true
    flags: Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint
    Component.onCompleted: {

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
