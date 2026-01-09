import QtQml
import QtQuick
import QtWebEngine
import QtWebChannel

Item {

    width: parent.width
    height: parent.height
    WebChannel {
        id: webChannel
    }

    QtObject {
        id: qtJSApi
        objectName: "QtJSApi"
        WebChannel.id: "QtJSApi"

        function quit() {
            Qt.quit()
        }
    }

    WebEngineView {
        property string homeUrl: urlToLoad
        property var authorizedHostnames: [new URL(urlToLoad).hostname]
        width: parent.width
        height: parent.height

        profile.httpCacheType: WebEngineProfile.NoCache
        profile.persistentCookiesPolicy: WebEngineProfile.NoPersistentCookies
        profile.offTheRecord: true
        profile.persistentStoragePath: "null"
        profile.httpAcceptLanguage: getLocaleAsAcceptLanguage()
        id: webEngine

        webChannel: webChannel
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

        onNewWindowRequested: function (request) {
            if (request.userInitiated) {
                webEngine.url = request.requestedUrl
            }
        }

        onNavigationRequested: function (request) {
            var urlStr = request.url.toString()
            console.log("trying to navigate to: ", urlStr)
            var apiScript = {
                "name": "QWebChannel",
                "sourceUrl": "qrc:///qtwebchannel/qwebchannel.js",
                "injectionPoint": WebEngineScript.DocumentCreation,
                "worldId": WebEngineScript.MainWorld
            }
            var urlUrl = new URL(request.url)
            if (request.navigationType === WebEngineNavigationRequest.RedirectNavigation) {
                console.log("Redirect: ", urlUrl.hostname)
                if (!authorizedHostnames.some(
                            hostname => hostname === urlUrl.hostname)) {
                    console.log("Domain not in authorizedUrls adding it")
                    authorizedHostnames.push(urlUrl.hostname)
                    var urlsStr = ""
                    authorizedHostnames.forEach(
                                element => urlsStr += element + "\n")
                    console.log("new array\n", urlsStr)
                }
            } else {
                if (authorizedHostnames.some(
                            hostname => hostname === urlUrl.hostname)) {
                    request.accept()
                } else {
                    console.log("Domain not authorized rejecting")
                    if (request.navigationType
                            === WebEngineNavigationRequest.LinkClickedNavigation) {
                        forbiddenLinkDialog.open()
                    }
                    request.reject()
                }
            }

            webEngine.userScripts.collection = [apiScript]
            // ignore mailto and other
            if (!(urlStr.startsWith("http://") || urlStr.startsWith("https://")
                  || urlStr.startsWith("file://"))) {
                console.log("NavigationRequest blocked")
                request.reject()
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
        onWindowCloseRequested: {
            Qt.quit()
        }

        Timer {
            id: reloadingTimer
            interval: 5000
            onTriggered: function () {
                webEngine.reloadAndBypassCache()
            }
        }
        url: urlToLoad

        NeosDialog {
            id: forbiddenLinkDialog
            title: "Lien non autorisé"
            text: "Vous n'êtes pas autorisé à accéder à ce lien"

            onAccepted: {
                this.close()
            }
        }
    }
    property alias webView: webEngine
    Component.onCompleted: {
        webChannel.registerObject("qtJSAPI", qtJSApi)
    }
}
