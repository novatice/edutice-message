import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.3
import QtQuick.Controls.Universal 2.12
import QtQml 2.12
import QtWebEngine 1.8
import AvenirFonts 1.0

Window {
    width: Screen.width
    height: Screen.height
    visible: true
    flags: Qt.FramelessWindowHint | Qt.Window
    visibility: Qt.WindowFullScreen
    Component.onCompleted: {

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

            WebEngineView {
                property string homeUrl: urlToLoad

                width: parent.width
                height: parent.height

                profile.httpCacheType: WebEngineProfile.NoCache
                profile.persistentCookiesPolicy: WebEngineProfile.NoPersistentCookies
                profile.offTheRecord: true
                profile.persistentStoragePath: "null"
                profile.httpAcceptLanguage: getLocaleAsAcceptLanguage()
                id: webEngine

                function goHome() {
                    url = homeUrl
                }

                function getLocaleAsAcceptLanguage() {
                    const locale = Qt.locale()
                    return locale.name.replace("_", "-")
                }

                onContextMenuRequested: function (request) {
                    request.accepted = true
                }

                onFullScreenRequested: function (request) {
                    request.accept()
                }

                onPrintRequested: function () {}

                onFileDialogRequested: function (request) {
                    request.accepted = true
                    request.dialogReject()
                }

                onNewViewRequested: function (request) {
                    if (request.userInitiated) {
                        webEngine.url = request.requestedUrl
                    }
                }

                onNavigationRequested: function (request) {
                    var urlStr = request.url.toString()
                    console.log("trying to navigate to: ", urlStr)
                    // ignore mailto and other
                    if (!(urlStr.startsWith("http://") || urlStr.startsWith(
                              "https://"))) {
                        request.action = WebEngineNavigationRequest.IgnoreRequest
                    }
                }

                onLoadingChanged: function (request) {
                    console.log("loading: ", request.url, request.errorCode)
                    if (request.status === WebEngineView.LoadFailedStatus) {
                        console.log("loading failed: ", request.errorCode, " ",
                                    request.errorString)
                        reloadingTimer.start()
                    }
                }

                Timer {
                    id: reloadingTimer
                    interval: 5000
                    onTriggered: function () {
                        webEngine.reloadAndBypassCache()
                    }
                }

                url: urlToLoad
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
