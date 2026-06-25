import QtQuick
import QtQuick.Window
import QtQuick.Dialogs
import QtQuick.Controls.Basic
import QtQuick.Layouts

import AvenirFonts 1.0

Dialog {
    property alias text: contentLabel.text
    property alias acceptText: acceptBtn.text
    property alias cancelText: cancelBtn.text
    property bool withCancelButton: false

    parent: Overlay.overlay
    x: (parent.width - this.width) / 2
    y: parent.height / 2 - this.height / 2

    implicitWidth: parent.width / 4

    id: root
    modal: true
    closePolicy: "NoAutoClose"
    // signal onAccepted
    signal canceled

    background: Rectangle {
        border.color: "transparent"
        radius: 5
        anchors.fill: parent
    }

    header: RowLayout {
        width: parent.width

        Label {
            Layout.maximumWidth: root.width
            Layout.alignment: Qt.AlignHCenter

            text: root.title
            wrapMode: "WrapAtWordBoundaryOrAnywhere"

            font: AvenirFonts.bold.deriveFont(38)
            padding: 12
            color: "#3b78bc"
        }
    }

    contentItem: ColumnLayout {
        height: parent.height

        id: column
        spacing: 10

        RowLayout {
            Layout.fillWidth: true

            height: implicitHeight

            Label {
                id: contentLabel
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignHCenter

                font: AvenirFonts.regular.deriveFont(16)
                wrapMode: "WordWrap"
                horizontalAlignment: Qt.AlignHCenter
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignCenter
            height: implicitHeight
            spacing: 30

            NeosDialogButton {
                id: cancelBtn
                text: "Annuler"
                onClicked: {
                    root.canceled()
                    close()
                }
                visible: withCancelButton
            }

            NeosDialogButton {
                id: acceptBtn
                text: "Ok"
                onClicked: {
                    root.accepted()
                    close()
                }
            }
        }
    }
}
