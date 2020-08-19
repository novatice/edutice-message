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

        //if (argc<1) return app.exec();

        w->setWindowState(Qt::WindowFullScreen);
        //QRect rec = m[i]->availableGeometry();

        QRect rec = QApplication::desktop()->screenGeometry(i);

        w->setFixedSize(rec.width(),rec.height());
        w->move(QPoint(rec.x(), rec.y()));

        w->show();
        w->launchWebView(argv[1]);
    }

    /*
    QFile file(QCoreApplication::applicationDirPath() + "/htmlInject.html");
       if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
           return -2;
    QTextStream in(&file);
    QString html;
    html = in.readAll();
    file.close();

    QString afl = QCoreApplication::applicationDirPath() + "/htmlInject.html";
    std::cout << afl.toStdString() << std::endl;
    */

    return app.exec();
}

