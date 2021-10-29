#include <QApplication>
#include <mainwindow.h>
#include <QDesktopWidget>
#include <QScreen>
#include <iostream>
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    const auto m = app.screens();

    for(int i = 0; i< m.length(); i++)
    {
        MainWindow* w= new MainWindow();

        w->setWindowState(Qt::WindowFullScreen);

        QRect rec = QApplication::desktop()->screenGeometry(i);

        w->setFixedSize(rec.width(),rec.height());
        w->move(QPoint(rec.x(), rec.y()));

        w->show();
        w->launchWebView(argv[1]);
    }

    QFile file(":/stylesheet.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QString(file.readAll());

    app.setStyleSheet(styleSheet);

    return app.exec();
}

