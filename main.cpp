#include "qdebug.h"
#include <QApplication>
#include <QCommandLineParser>
#include <QDesktopWidget>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QScreen>
#include <iostream>

#ifdef _WIN32
#include "qtimer.h"
#include <Tlhelp32.h>
#include <process.h>
#include <winbase.h>
#include <windows.h>
#endif

#ifdef _WIN32

HWND hWnd;
LPSTR strTitle;

void hideAllWindows() {
  if (GetForegroundWindow() != hWnd) {

    /*
HWND hwndW = GetNextWindow(hWnd, GW_HWNDNEXT);

    if (hwndW != NULL)
    {
        LPSTR str;

        GetWindowTextA(hwndW,str,15);

        if (str == strTitle)
        {
            return;
        }
    }

    for (HWND hwndW = GetTopWindow(NULL); hwndW != NULL; hwndW =
                                                         GetNextWindow(hwndW,
GW_HWNDNEXT))
    {
        if (hwndW == hWnd)
            continue;
        /*
    if (!IsWindowVisible(hwndW))
        continue;

        int length = GetWindowTextLength(hwndW);
        if (length == 0)
            continue;

        ShowWindow(hwndW, SW_HIDE);
    }

    //*/
    SetForegroundWindow(hWnd);
  }
  ShowWindow(hWnd, SW_SHOWMAXIMIZED);
}

void showAllWindows() {
  for (HWND hwndW = GetTopWindow(NULL); hwndW != NULL;
       hwndW = GetNextWindow(hwndW, GW_HWNDNEXT)) {
    if (!IsWindowVisible(hwndW))
      continue;

    int length = GetWindowTextLength(hwndW);
    if (length == 0)
      continue;
    ShowWindow(hwndW, SW_RESTORE);
  }
}
#endif

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  const auto screens = app.screens();

  QMessageLogger logger;

  QCommandLineParser cmdParser;
  cmdParser.addHelpOption();
  QCommandLineOption withoutCloseBtnOpt(
      "without-close-button",
      "Lancer l'application sans bouton de fermeture (l'option est ignorée si "
      "utilisée avec le mode POLICY)");
  cmdParser.addOption(withoutCloseBtnOpt);

  QCommandLineOption modeOpt(
      "mode", "Lancer l'application dans le mode spécifié (MESSAGE ou POLICY)",
      "mode");
  cmdParser.addOption(modeOpt);

  cmdParser.addPositionalArgument("url",
                                  "L'url à charger (adresse ou fichier)");

  cmdParser.process(app);

  bool withoutCloseBtn = cmdParser.isSet(withoutCloseBtnOpt);

  QStringList argsList = cmdParser.positionalArguments();

  if (argsList.size() < 1) {
    std::cerr << cmdParser.helpText().toStdString();
    exit(1);
  }

  QString url = argsList.at(0);
  qDebug() << url;

  QQmlApplicationEngine engine;

  engine.addImportPath("qrc:/qml/"); /* Insert relative path to your
                                            import directory here */

  const QUrl qmlUrl(QStringLiteral("qrc:/qml/main.qml"));

#ifdef _WIN32
  auto timer = new QTimer();
  QObject::connect(timer, &QTimer::timeout, [] { hideAllWindows(); });
  timer->setInterval(500);
#endif

  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreated, &app,
      [&qmlUrl](QObject *obj, const QUrl &objUrl) {
        if (!obj && qmlUrl == objUrl)
          QCoreApplication::exit(-1);
#ifdef _WIN32
        if (QQuickWindow *window =
                qobject_cast<QWindow *>(engine.rootObjects().at(0)))
          hWnd = window->winId();

        timer->start();
#endif
      },
      Qt::QueuedConnection);
  engine.rootContext()->setContextProperty("urlToLoad", url);
  engine.rootContext()->setContextProperty("withoutCloseButton",
                                           withoutCloseBtn);

  logger.debug() << "just before start";

  engine.load(qmlUrl);

  return app.exec();
}
