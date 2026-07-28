import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import AvenirFonts 1.0
import Theme 1.0

Item {

    visible: true

    property alias text: label.text

    /*
      if we want to apply new graphical chart
gradient: Gradient{
            orientation: Gradient.Vertical
            GradientStop {position:0.0; color: "#B2C8F7"}
            GradientStop {position:0.6; color: "#B2C8F7"}
            GradientStop {position:1.0; color: Theme.primaryTextColor}
    }*/

    Rectangle {
        anchors.fill: parent
        gradient: Gradient{
            orientation: Gradient.Horizontal
            GradientStop {position:0.0; color: Theme.primaryColor}
            GradientStop {position:1.0; color: Theme.secondaryColor}
        }
    }

    ColumnLayout {
        spacing: 10
        anchors.centerIn: parent

        Label {
            Layout.alignment: Qt.AlignHCenter
            id: label
            font: AvenirFonts.regular.deriveFont(30)
            color: Theme.primaryTextColor
        }

        BusyIndicator {
            Layout.alignment: Qt.AlignHCenter
            width: parent.width

            Component.onCompleted: {
                this.contentItem.pen = "transparent"
                this.contentItem.fill = Theme.primaryTextColor
            }
        }
    }
}
