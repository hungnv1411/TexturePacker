#include "MainWindow.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget* centralWidget = new QWidget();
    setCentralWidget(centralWidget);

    QWidget *topFiller = new QWidget;
    topFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QWidget *bottomFiller = new QWidget;
    bottomFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(5);
    layout->addWidget(topFiller);
    layout->addWidget(bottomFiller);
    centralWidget->setLayout(layout);

    createActions();
    createMenus();
    createToolbar();

    setWindowTitle(tr("TexturePacker"));
    setMinimumSize(800, 600);
    resize(1024, 768);
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
    editMenu->addAction(addSmartFolderAction);
    editMenu->addAction(removeSpriteAction);

    toolMenu = menuBar()->addMenu(tr("&Tools"));
    toolMenu->addAction(optionAction);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAction);
}

void MainWindow::createToolbar() {
    fileToolbar = this->addToolBar(tr("File"));
    fileToolbar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    fileToolbar->setMovable(false);

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






