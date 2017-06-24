#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QVBoxLayout>
#include <QtWidgets>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void createActions();
    void createMenus();
    void createToolbar();

private:
    // menu bar
    QMenu* fileMenu;
    QMenu* editMenu;
    QMenu* toolMenu;
    QMenu* helpMenu;
    // menu actions
    QAction* newAction;
    QAction* openAction;
    QAction* saveAction;
    QAction* exitAction;
    QAction* optionAction;
    QAction* addSpriteAction;
    QAction* addSmartFolderAction;
    QAction* removeSpriteAction;
    QAction* aboutAction;
    // toolbar
    QToolBar* fileToolbar;
};

#endif // MAINWINDOW_HPP
