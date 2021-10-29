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


    QFont mainFont("Georgia",16,QFont::Bold);

    view = new QWebEngineView();

    // disable context menu in view
    view->setContextMenuPolicy(Qt::NoContextMenu);

    mainLayout->addWidget(view, Qt::AlignHCenter);

    QWidget *bottomWidget = new QWidget();
    bottomWidget->setObjectName("bottom");

    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomWidget->setLayout(bottomLayout);

    mainLayout->addWidget(bottomWidget);


    QPushButton *button = new QPushButton();
    button->setObjectName("confirmButton");
    button->setText("J'ai compris");
    button->setCursor(Qt::PointingHandCursor);

    button->raise();
    bottomLayout->addWidget(button, 0, Qt::AlignHCenter);

    connect(button, SIGNAL(clicked()), this, SLOT(OnClicked()));
}

void MainWindow::launchWebView(QString src)
{
    if (QFileInfo::exists(src))
        view->load(QUrl::fromLocalFile(QFileInfo(src).absoluteFilePath()));
    else
        view->load(QUrl(src));

    view->lower(); // Lowers the widget to the bottom of the parent widget's stack.

}

void dispose(QWebEngineView *view) {
    delete view;
    QCoreApplication::quit();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    dispose(view);
}
void MainWindow::OnClicked()
{
   dispose(view);
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

