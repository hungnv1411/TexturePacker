#include "MainWindow.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
//    QWidget* centralWidget = new QWidget();
//    setCentralWidget(centralWidget);

//    QWidget *topFiller = new QWidget;
//    topFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

//    QWidget *bottomFiller = new QWidget;
//    bottomFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

//    QVBoxLayout *layout = new QVBoxLayout;
//    layout->setMargin(5);
//    layout->addWidget(topFiller);
//    layout->addWidget(bottomFiller);
//    centralWidget->setLayout(layout);

    createContentView();
    createActions();
    createMenus();
    createToolbar();
    createStatusbar();
    createSpritesTreeview();
    createExporterSettingPane();

    setWindowTitle(tr("TexturePacker"));

    readSettings();
}

MainWindow::~MainWindow()
{

}

void MainWindow::createActions() {
    // menu file
    newAction = new QAction(tr("&New project"), this);
    newAction->setShortcuts(QKeySequence::New);
    newAction->setStatusTip(tr("Create a new project"));
    newAction->setIcon(QIcon(":/res/ic_new.png"));
    newAction->setIconVisibleInMenu(true);
    newAction->setIconText(tr("New project"));

    openAction = new QAction(tr("&Open project"), this);
    openAction->setShortcuts(QKeySequence::Open);
    openAction->setStatusTip(tr("Open project"));
    openAction->setIcon(QIcon(":/res/ic_browse.png"));
    openAction->setIconVisibleInMenu(true);
    openAction->setIconText(tr("Open project"));

    saveAction = new QAction(tr("&Save project"), this);
    saveAction->setShortcuts(QKeySequence::Save);
    saveAction->setStatusTip(tr("Save project"));
    saveAction->setIcon(QIcon(":/res/ic_save_project.png"));
    saveAction->setIconVisibleInMenu(true);
    saveAction->setIconText(tr("Save project"));

    exitAction = new QAction(tr("&Exit"), this);
    exitAction->setShortcuts(QKeySequence::Quit);
    exitAction->setStatusTip(tr("Exit"));

    // menu edit
    addSpriteAction = new QAction(tr("&Add sprites"), this);
    addSpriteAction->setStatusTip(tr("Add sprites"));
    addSpriteAction->setIcon(QIcon(":/res/ic_add_sprite.png"));
    addSpriteAction->setIconVisibleInMenu(true);
    addSpriteAction->setIconText(tr("Add sprites"));

    removeSpriteAction = new QAction(tr("&Remove sprites"), this);
    removeSpriteAction->setStatusTip(tr("Remove sprites"));
    removeSpriteAction->setIcon(QIcon(":/res/ic_remove_sprite.png"));
    removeSpriteAction->setIconVisibleInMenu(true);
    removeSpriteAction->setIconText(tr("Remove sprites"));

    addSmartFolderAction = new QAction(tr("&Add smart folder"), this);
    addSmartFolderAction->setStatusTip(tr("Add smart folder"));
    addSmartFolderAction->setIcon(QIcon(":/res/ic_add_smart_folder.png"));
    addSmartFolderAction->setIconVisibleInMenu(true);
    addSmartFolderAction->setIconText(tr("Add smart folder"));

    // menu tool
    optionAction = new QAction(tr("&Options"), this);
    optionAction->setStatusTip(tr("Options"));
    optionAction->setIcon(QIcon(":/res/ic_setting.png"));
    optionAction->setIconVisibleInMenu(true);
    optionAction->setIconText(tr("Options"));

    // menu help
    aboutAction = new QAction(tr("&About"), this);
    aboutAction->setStatusTip(tr("About"));
}

void MainWindow::createMenus() {
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(addSpriteAction);
    editMenu->addAction(removeSpriteAction);
    editMenu->addAction(addSmartFolderAction);

    toolMenu = menuBar()->addMenu(tr("&Tools"));
    toolMenu->addAction(optionAction);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAction);
}

void MainWindow::createToolbar() {
    fileToolbar = this->addToolBar(tr("File"));
    fileToolbar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    fileToolbar->setMovable(false);
    fileToolbar->setObjectName("mainToolbar");

    fileToolbar->addAction(newAction);
    fileToolbar->addAction(openAction);
    fileToolbar->addAction(saveAction);

    QWidget* empty = new QWidget();
    empty->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    empty->setFixedWidth(50);
    fileToolbar->addWidget(empty);

    fileToolbar->addAction(addSpriteAction);
    fileToolbar->addAction(removeSpriteAction);
    fileToolbar->addAction(addSmartFolderAction);

    empty = new QWidget();
    empty->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    fileToolbar->addWidget(empty);

    fileToolbar->addAction(optionAction);
}

void MainWindow::createStatusbar() {
    statusBar = new QStatusBar();
    this->setStatusBar(statusBar);

    QLabel* emptyLabel = new QLabel(tr("Empty"));
    statusBar->addWidget(emptyLabel);
}

void MainWindow::createSpritesTreeview() {
    QDockWidget *dock = new QDockWidget(tr("Sprites"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea);
    dock->setFeatures(QDockWidget::DockWidgetFeature::NoDockWidgetFeatures);
    addDockWidget(Qt::LeftDockWidgetArea, dock);
    dock->setObjectName("spritesDock");

    QTreeWidget *treeWidget = new QTreeWidget(dock);
    treeWidget->setColumnCount(1);
    dock->setWidget(treeWidget);

    QList<QTreeWidgetItem *> items;
    for (int i = 0; i < 10; ++i)
        items.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString("item: %1").arg(i))));
    treeWidget->insertTopLevelItems(0, items);
}

void MainWindow::createExporterSettingPane() {
    QDockWidget *dock = new QDockWidget(tr("Settings"), this);
    dock->setAllowedAreas(Qt::RightDockWidgetArea);
    dock->setFeatures(QDockWidget::DockWidgetFeature::NoDockWidgetFeatures);
    addDockWidget(Qt::RightDockWidgetArea, dock);
    dock->setObjectName("exporterSettingsDock");

    QListWidget* settingWidget = new QListWidget(dock);
    dock->setWidget(settingWidget);
}

void MainWindow::createContentView() {
    contentScrollArea = new QScrollArea();
    contentScrollArea->setBackgroundRole(QPalette::Dark);
    setCentralWidget(contentScrollArea);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (requestSaveProject()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::readSettings() {
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    const QByteArray geometry = settings.value("geometry", QByteArray()).toByteArray();
    if (geometry.isEmpty()) {
        const QRect availableGeometry = QApplication::desktop()->availableGeometry(this);
        resize(availableGeometry.width() * 0.75, availableGeometry.height() * 0.75);
        move((availableGeometry.width() - width()) / 2, (availableGeometry.height() - height()) / 2);
    } else {
        restoreGeometry(geometry);
    }
    restoreState(settings.value("windowState").toByteArray());
}

void MainWindow::writeSettings() {
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
}

bool MainWindow::requestSaveProject() {
    const QMessageBox::StandardButton ret = QMessageBox::warning(this, QCoreApplication::applicationName(),
                               tr("The document has been modified.\n"
                                  "Do you want to save your changes?"),
                               QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    switch (ret) {
    case QMessageBox::Save:
        return saveProject();
    case QMessageBox::Cancel:
        return false;
    default:
        break;
    }
    return true;
}

bool MainWindow::saveProject() {
    return true;
}






