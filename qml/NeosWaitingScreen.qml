import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Styles 1.4
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
