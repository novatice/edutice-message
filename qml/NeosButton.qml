import QtQml
import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import AvenirFonts 1.0
import Theme 1.0

Button {
    id: root
    property bool disabled: false
    property string color: Theme.secondaryColor
    property string tooltip
    property bool secondary : false
    font: AvenirFonts.bold.deriveFont(30)
    height : 654654654

    states :[
        State{
            name: "Secondary"
            when: secondary

            PropertyChanges {
                target: backgroundRectangle;
                color: Theme.primaryTextColor;
                border.color: Theme.secondaryColor
                border.width: 1
            }
            PropertyChanges {
                target: focusRectangle
                focusColor: Theme.secondaryColor
            }
            PropertyChanges {
                target: buttonText
                color: Theme.secondaryColor
            }
        },
        State {
            name: "Disabled"
            when: disabled
            PropertyChanges {
                target: backgroundRectangle
                color:"gray"
            }
            PropertyChanges {
                target: root
                activeFocusOnTab: false
                focusPolicy: Qt.NoFocus
            }
        },
        State {
            name: "Pressed"
            when: pressed
            PropertyChanges {
                target: backgroundRectangle
                border.color: "white"
            }
        }
    ]

    background: Rectangle {
        id: backgroundRectangle
        color: root.color
        radius: 5
        border.color: "transparent"
    }

    //visual rectangle to show on which button focus is
    Rectangle {
        id: focusRectangle
        property string focusColor : Theme.focusColor
        width: backgroundRectangle.width - 5
        height: backgroundRectangle.height - 5
        x: 2.5
        y: 2.5
        radius: 4
        color: "transparent"
        border.color: "transparent"
        border.width: 6
    }

    onActiveFocusChanged: {
        if (activeFocus) {
            if (focusReason == Qt.TabFocusReason || focusReason == Qt.BacktabFocusReason) {
                focusRectangle.border.color = focusRectangle.focusColor;
            }
        } else {
            focusRectangle.border.color = "transparent";
        }
    }

    Keys.onPressed: event => {
        if (event.key === Qt.Key_Space || event.key === Qt.Key_Return) {
            console.log("Action triggered by keyboard!");
            event.accepted = true;
            root.clicked();
        }
    }

    activeFocusOnTab: true

    contentItem: RowLayout {
        width: root.width
        height: root.height

        Image {
            source: icon.source
            sourceSize.width: root.icon.width
            sourceSize.height: root.icon.height
        }

        Text {
            id: buttonText
            Layout.fillHeight: true
            Layout.fillWidth: true
            text: root.text
            font: root.font
            padding: 5
            color: Theme.primaryTextColor
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
                this.cursorShape = Qt.ArrowCursor;
            } else {
                this.cursorShape = Qt.PointingHandCursor;
            }
        }

        onPressed: mouse => {
            mouse.accepted = root.disabled;
        }

        onClicked: mouse => {
            mouse.accepted = root.disabled;
        }
    }

    ToolTip.visible: this.hovered && this.tooltip
    ToolTip.text: tooltip
    ToolTip.toolTip.font: AvenirFonts.regular.font
}
