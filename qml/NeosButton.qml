import QtQml
import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import AvenirFonts 1.0

Button {
    property bool disabled: false
    property string color: "#0092CC"
    property string tooltip

    id: root
    background: Rectangle {
        color: root.disabled ? "gray" : root.color
        radius: 5
        border.color: root.pressed ? "white" : "transparent"
    }

    contentItem: RowLayout {
        width: root.width
        height: root.height

        Image {
            source: icon.source
            sourceSize.width: root.icon.width
            sourceSize.height: root.icon.height
        }

        Text {
            Layout.fillHeight: true
            Layout.fillWidth: true
            text: root.text
            font: AvenirFonts.bold.deriveFont(30)
            padding: 5
            color: "white"
            visible: root.text
        }
    }

    icon.color: disabled ? "gray" : "transparent"
    icon.width: 32
    icon.height: 32

    MouseArea {
        id: ma
        cursorShape: root.disabled ? Qt.ArrowCursor : Qt.PointingHandCursor
        enabled: true
        anchors.fill: parent
        hoverEnabled: true

        onEnabledChanged: {
            if (root.disabled) {
                this.cursorShape = Qt.ArrowCursor
            } else {
                this.cursorShape = Qt.PointingHandCursor
            }
        }

        onPressed: (mouse) =>{
            mouse.accepted = root.disabled
        }

        onClicked: (mouse) => {
            mouse.accepted = root.disabled
        }
    }

    ToolTip.visible: this.hovered && this.tooltip
    ToolTip.text: tooltip
    ToolTip.toolTip.font: AvenirFonts.regular.font
}
