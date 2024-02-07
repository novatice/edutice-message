import QtQuick 2.12
import QtQuick.Controls 2.12

import AvenirFonts 1.0

NeosButton {
    id: root

    contentItem: Text {
        text: root.text
        font: AvenirFonts.regular.deriveFont(16)
        padding: 5
        color: "white"
    }

    background: Rectangle {
        anchors.fill: root
        color: "#3b78bc"
        radius: 5
    }
}
