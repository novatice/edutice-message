import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import AvenirFonts 1.0

Item {

    visible: true

    property alias text: label.text

    Rectangle {
        anchors.fill: parent
        color: "#3b78bc"
    }

    ColumnLayout {
        spacing: 10
        anchors.centerIn: parent

        Label {
            Layout.alignment: Qt.AlignHCenter
            id: label
            font: AvenirFonts.regular.deriveFont(30)
            color: "white"
        }

        BusyIndicator {
            Layout.alignment: Qt.AlignHCenter
            width: parent.width

            Component.onCompleted: {
                this.contentItem.pen = "transparent"
                this.contentItem.fill = "white"
            }
        }
    }
}
