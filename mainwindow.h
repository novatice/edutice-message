#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#ifdef _WIN32
#include <windows.h>
#include <process.h>
#include <winbase.h>
#include <Tlhelp32.h>
#include <QWebEngineView>
#endif

#ifdef linux
    #include <QWebView>
#endif

#include <QMainWindow>
#include <QPushButton>
#include <QBoxLayout>
#include <QFileInfo>
#include <QTimer>
#include <QCoreApplication>
#include <QApplication>
#include <QDesktopWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void launchWebView(QString src);
    ~MainWindow();
private slots:
    #ifdef _WIN32
        void hideAllWindows();
    #endif
    void OnClicked();
private:

    QTimer* timer;
    Ui::MainWindow *ui;
    QVBoxLayout *mainLayout;
};
#endif // MAINWINDOW_H
