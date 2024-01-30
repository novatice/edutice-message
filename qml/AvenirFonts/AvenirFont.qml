import QtQml 2.12
import QtQuick 2.12

QtObject {
    property alias source: loader.source
    property bool bold: false
    property bool italic: false

    readonly property FontLoader loader: FontLoader {
        id: loader
    }

    readonly property FontMetrics metrics: FontMetrics {
        id: metrics
        font.family: loader.name
    }

    readonly property string name: loader.name
    default property font font: Qt.font({
                                            "family": loader.name,
                                            "pixelSize": 12,
                                            "styleName": getStyleName()
                                        })

    function getStyleName() {
        if (bold) {
            return "Bold"
        }

        if (italic) {
            return "Italic"
        }

        return "Regular"
    }

    function deriveFont(pxSize) {
        return Qt.font({
                           "family": this.font.family,
                           "pixelSize": pxSize,
                           "styleName": getStyleName()
                       })
    }
}
