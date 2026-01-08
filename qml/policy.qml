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

    flags: Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint
    Timer {
        interval: duration * 1000
        onTriggered: {
            Qt.quit(0)
        }
        Component.onCompleted: {
            this.start()
        }
    }

    Connections {
        target: policyAgreement

        ignoreUnknownSignals: false

        function onAgreed() {
            Qt.exit(1)
        }

        function onAgreedError() {
            waitingPopup.close()
            agreeErrorDialog.open()
        }

        function onDisagreed() {
            Qt.exit(2)
        }
    }

    NeosDialog {
        id: agreeErrorDialog
        title: "Charte d'utilisation"
        text: "Une erreur est survenue lors de l'enregistrement de votre choix, veuilllez réessayer"

        onAccepted: {
            this.close()
        }
    }

    NeosDialog {
        id: disagreeDialog
        title: "Charte d'utilisation"
        withCancelButton: true
        text: "En refusant la charte d'utilisation vous ne pourrez pas ouvrir votre session et vous serez redirigé vers l'écran de connexion.\n\n Souhaitez-vous vraiment refuser la charte d'utilisation ?"

        acceptText: "Refuser"

        onAccepted: {
            waitingPopup.open()
            policyAgreement.disagree()
        }

        onCanceled: {
            this.close()
        }
    }

    NeosDialog {
        id: quitDialog
        title: "Charte d'utilisation"
        withCancelButton: true
        text: "Souhaitez vous quitter la charte d'utilisation ? \n\n Vous serez redirigé vers l'écran de connexion"

        acceptText: "Quitter"

        onAccepted: {
            Qt.exit(0)
        }

        onCanceled: {
            this.close()
        }
    }

    Popup {
        id: waitingPopup

        width: parent.width
        height: parent.height

        anchors.centerIn: parent

        modal: true
        closePolicy: Popup.NoAutoClose

        background: Rectangle {
            color: "red"
        }

        // Popup has default padding, remove it
        padding: 0

        NeosWaitingScreen {
            anchors.fill: parent
            text: "Envoi de votre choix en cours"
        }
    }

    ColumnLayout {
        spacing: 0
        width: parent.width
        height: parent.height

        Item {
            Layout.fillWidth: true

            height: 60

            Rectangle {
                color: "whitesmoke"
                anchors.fill: parent
            }

            RowLayout {
                anchors.fill: parent

                Spacer {}

                NeosButton {
                    color: "red"
                    Layout.rightMargin: 10

                    height: 40
                    width: 40
                    icon.source: "qrc:/close.png"
                    onClicked: {
                        quitDialog.open()
                    }
                }
            }
        }

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            NeosWebEngine {
                id: webengine
                anchors.fill: parent

                webView.onLoadingChanged: function (request) {
                    if (request.status === WebEngineView.LoadSucceededStatus) {
                        loadingScreen.visible = false
                        reloadingLabel.visible = false
                    } else if (request.status === WebEngineView.LoadFailedStatus) {
                        reloadingLabel.visible = true
                    }
                }
            }

            NeosWaitingScreen {
                id: loadingScreen
                anchors.fill: parent
                text: "Chargement de la charte en cours..."
            }

            Label {
                id: reloadingLabel
                anchors.bottom: parent.bottom
                visible: false

                width: parent.width

                background: Rectangle {
                    anchors.fill: parent
                    color: "#F89345"
                }

                text: "Une erreur est survenue, rechargement dans 10 secondes"
                horizontalAlignment: Qt.AlignHCenter
                color: "white"
                font: AvenirFonts.regular.deriveFont(24)
            }
        }

        Item {
            Layout.fillWidth: true
            height: 60

            Rectangle {
                color: "whitesmoke"
                anchors.fill: parent
            }

            RowLayout {
                anchors.fill: parent
                spacing: 10

                Spacer {}

                NeosButton {
                    text: "Refuser"
                    disabled: webengine.loading || policyAgreement.isRunning
                    onClicked: {
                        disagreeDialog.open()
                    }
                }

                NeosButton {
                    Layout.rightMargin: 10
                    color: "green"
                    text: "Accepter"
                    disabled: webengine.loading || policyAgreement.isRunning
                    onClicked: {
                        waitingPopup.open()
                        policyAgreement.agree()
                    }
                }
            }
        }
    }
}
