import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import AvenirFonts 1.0

Dialog {
    property alias text: contentLabel.text
    property alias acceptText: acceptBtn.text
    property alias cancelText: cancelBtn.text
    property bool withCancelButton: false

    parent: Overlay.overlay
    x: (parent.width - this.width) / 2
    y: parent.height / 2 - this.height / 2

    width: parent.width / 4

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

    header: Label {
        text: root.title
        horizontalAlignment: Qt.AlignHCenter

        font: AvenirFonts.bold.deriveFont(38)
        padding: 12
        color: "#3b78bc"
    }

    contentItem: ColumnLayout {
        width: parent.width
        height: parent.height

        id: column
        spacing: 10

        RowLayout {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignCenter

            Label {
                id: contentLabel
                Layout.fillWidth: true
                font: AvenirFonts.regular.deriveFont(16)
                wrapMode: "WordWrap"
                horizontalAlignment: Qt.AlignCenter
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignCenter
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
