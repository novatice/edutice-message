#include <QApplication>
#include <mainwindow.h>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;

    //if (argc<1) return app.exec();

    w.setWindowState(Qt::WindowFullScreen);

    QRect rec = QApplication::desktop()->screenGeometry();
    int width = rec.width();
    int height = rec.height();
    w.setFixedSize(width,height);
    w.show();

    w.launchWebView(argv[1]);

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

