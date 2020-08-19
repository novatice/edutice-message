#include "mainwindow.h"
#include <QTabWidget>
#ifdef _WIN32
    HWND hWnd;
    LPSTR strTitle;
#endif

void hideAllWindows();
void showAllWindows();

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    #ifdef _WIN32
    std::string st = "alertWindows";
    strTitle = const_cast<char *>(st.c_str());
    SetWindowTextA(hWnd,strTitle);
    #endif

    setWindowFlags( Qt::FramelessWindowHint );
    setAttribute( Qt::WA_TranslucentBackground );
    setWindowFlags(Qt::WindowStaysOnTopHint);

    #ifdef _WIN32
        hWnd = (HWND)this->winId();

        timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(hideAllWindows()));
        timer->setInterval(500);
        timer->start();
    #endif

    mainLayout = new QVBoxLayout;
    setCentralWidget(new QWidget);
    centralWidget()->setLayout(mainLayout);



    this->setStyleSheet("background-color: rgba(255,255,255,0.9);");

    //QPixmap pixmap(QCoreApplication::applicationDirPath()+"/cross.png");
    //QIcon ButtonIcon(pixmap);

    QFont mainFont("Georgia",16,QFont::Bold);
/*
    QWidget* wd = new QWidget(centralWidget());
    QPushButton *button = new QPushButton(centralWidget());
    //button->setIcon(ButtonIcon);
    button->setCursor(Qt::PointingHandCursor);
    //button->setFixedSize(100,100);
    button->setFixedSize(300,80);
    //button->setIconSize(QSize(100,100));
    button->setStyleSheet(" QPushButton {background-color: transparent;"
                          "border-style: outset;"
                          "border-width: 1px;"
                          "border-radius: 5px;"
                          "border-color: rgba(0, 0,0, 0.5);"
                          "font: bold 14px;"
                          "padding: 6px; }"
                          " QPushButton:hover {"
                          "     background-color: \"#800092CC\""
                          "}"
                          "QPushButton{font-size: 30px;font-family: mainFont; color :rgba(0, 0,0, 0.8); opacity:0.6}"
            );
    button->setText("J'ai compris");
    button->move(width/2-150, height-200);

    wd->setFixedSize(button->width(),button->height());
    wd->move(button->x(), button->y());
    wd->setStyleSheet("QWidget { background-color : rgba(0, 0,0, 0);}"
                      " button:hover {"
                      "     background-color: white"
                      "}"
            );

    button->raise();
    QWidget* t = new QWidget(centralWidget());


    t->setFixedSize(width,height);
    t->setStyleSheet("background-color: rgba(255,255,255,0.9);");
    QHBoxLayout* t2 = new QHBoxLayout;
    t->setLayout();

    /**/

    #ifdef _WIN32
        view = new QWebEngineView();
     #endif
     #ifdef linux
        view = new QWebView();
    #endif
    mainLayout->addWidget(view, Qt::AlignHCenter);

    QRect geometry = mainLayout->geometry();
    QPushButton *button = new QPushButton();
    button->setText("J'ai compris");
    button->setCursor(Qt::PointingHandCursor);
    button->setStyleSheet(" QPushButton {background-color: transparent;"
                          "border-style: outset;"
                          "border-width: 2px;"
                          "border-color: black;"
                          "font: bold 30px;"
                          "padding: 6px; }"
                          " QPushButton:hover {"
                          "     background-color: \"#0092CC\""
                          "}"
            );
    button->raise();
    mainLayout->addWidget(button, Qt::AlignHCenter);

    button->setFixedHeight(100);
    /**/
    connect(button, SIGNAL(clicked()), this, SLOT(OnClicked()));
}

void MainWindow::launchWebView(QString src)
{
    if (QFileInfo::exists(src))
    {
        //view->load(QUrl::fromLocalFile(QFileInfo(QCoreApplication::applicationDirPath() + "/htmlInject.html").absoluteFilePath()));
        view->load(QUrl::fromLocalFile(QFileInfo(src).absoluteFilePath()));
        view->lower();
    }
    else
        QCoreApplication::exit(-1);
}
void MainWindow::closeEvent(QCloseEvent *event)
{
    QCoreApplication::quit();
}
void MainWindow::OnClicked()
{
   QCoreApplication::quit();
}

MainWindow::~MainWindow()
{
}

#ifdef _WIN32

void MainWindow::hideAllWindows()
{
    if (GetForegroundWindow() != hWnd)
    {

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

        for (HWND hwndW = GetTopWindow(NULL); hwndW != NULL; hwndW = GetNextWindow(hwndW, GW_HWNDNEXT))
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

void showAllWindows()
{
        for (HWND hwndW = GetTopWindow(NULL); hwndW != NULL; hwndW = GetNextWindow(hwndW, GW_HWNDNEXT))
        {
            if (!IsWindowVisible(hwndW))
                continue;

            int length = GetWindowTextLength(hwndW);
            if (length == 0)
                continue;
            ShowWindow(hwndW, SW_RESTORE);
        }
}
#endif

