import QtQuick
import QtQuick.Window
import QtQuick.Layouts
import QtQuick.Controls.Basic
import QtQuick.Dialogs
import QtQuick.Controls.Universal
import QtQml
import QtWebEngine
import AvenirFonts 1.0

Window {
    width: Screen.width
    height: Screen.height
    visible: true
    flags: Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint | Qt.WindowMaximized
    visibility: Qt.WindowMaximized
    Component.onCompleted: {
        console.log(Screen.width, Screen.height)
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

            NeosWebEngine {
                webView.onLoadingChanged: function (request) {
                    if (request.status === WebEngineView.LoadSucceededStatus) {
                        loadingScreen.visible = false
                        reloadingLabel.visible = false
                    } else if (request.status === WebEngineView.LoadFailedStatus) {
                        reloadingLabel.visible = true
                    }
                }
            }
            NeosWaitingScreen{
                id: loadingScreen
                anchors.fill: parent
                text: "Chargement du message en cours..."
            }
            Label {
                id: reloadingLabel
                anchors.bottom: parent.bottom
                visible: false

                width: parent.width

                background: Rectangle {
                    anchors.fill: parent
                    color: Theme.warningColor
                }

                text: "Une erreur est survenue, rechargement dans 10 secondes"
                horizontalAlignment: Qt.AlignHCenter
                color: "white"
                font: AvenirFonts.regular.deriveFont(24)
            }
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
