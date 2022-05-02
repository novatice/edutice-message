#include <QApplication>
#include <mainwindow.h>
#include <QDesktopWidget>
#include <QCommandLineParser>
#include <QScreen>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    const auto screens = app.screens();

    QCommandLineParser cmdParser;
    cmdParser.addHelpOption();
    QCommandLineOption withoutCloseBtnOpt("without-close-button", "Lancer l'application sans bouton de fermeture");
    cmdParser.addOption(withoutCloseBtnOpt);

    cmdParser.addPositionalArgument("url", "L'url à charger (adresse ou fichier)");

    cmdParser.process(app);

    bool withoutCloseBtn = cmdParser.isSet(withoutCloseBtnOpt);

    QStringList argsList = cmdParser.positionalArguments();


    if(argsList.size() < 1) {
        std::cerr << cmdParser.helpText().toStdString();
        exit(1);
    }

    QString url = argsList.at(0);

    for(QScreen *screen : screens)
    {
        MainWindow* w = new MainWindow(withoutCloseBtn);

        w->setWindowState(Qt::WindowFullScreen);

        QRect rec = screen->geometry();

        w->setFixedSize(rec.width(),rec.height());
        w->move(QPoint(rec.x(), rec.y()));

        w->show();
        w->launchWebView(url);
    }

    QFile file(":/stylesheet.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QString(file.readAll());

    app.setStyleSheet(styleSheet);

    return app.exec();
}

