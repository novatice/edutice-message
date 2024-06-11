import QtQml 2.12
import QtQuick 2.0
import QtWebEngine 1.8
import QtWebChannel 1.0
//import "qrc:///qtwebchannel/qwebchannel.js"

Item{
    width: parent.width
    height: parent.height
    WebChannel{
        id:webChannel
    }

    QtObject{
        id: qtJSApi
        objectName: "QtJSApi"
        WebChannel.id: "QtJSApi"

        function quit(){
            Qt.quit()
        }
    }

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

        onNewViewRequested: function (request) {
            if (request.userInitiated) {
                webEngine.url = request.requestedUrl
            }
        }

        onNavigationRequested: function (request) {
            var urlStr = request.url.toString()
            console.log("trying to navigate to: ", urlStr)
            // ignore mailto and other
            if (!(urlStr.startsWith("http://") || urlStr.startsWith("https://"))) {
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
        userScripts: [
                WebEngineScript {
                    injectionPoint: WebEngineScript.DocumentCreation
                    worldId: WebEngineScript.MainWorld
                    name: "QWebChannel"
                    sourceUrl: "qrc:///qtwebchannel/qwebchannel.js"
                }]

        url: urlToLoad
    }
    Component.onCompleted: {
        webChannel.registerObject("qtJSAPI", qtJSApi);
    }
}
