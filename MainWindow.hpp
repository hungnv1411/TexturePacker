#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QVBoxLayout>
#include <QtWidgets>
#include "TPProject.hpp"

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
    void createStatusbar();
    void createSpritesTreeview();
    void createExporterSettingPane();
    void createContentView();
    void initConnections();
    void readSettings();
    void writeSettings();
    bool requestSaveProject();
    bool saveProject();
    void updateSpriteSheet();
    void newProject();

    // packer
private:
    void loadFiles(const QStringList& filePaths);
protected:
    void closeEvent(QCloseEvent *event) override;

protected slots:
    void onAddSprites();
    void onAddSmartFolder();
    void removeSelectedSprites();
    void spriteItemClicked(QTreeWidgetItem *item, int column);

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
    // status bar
    QStatusBar* statusBar;
    // content view
    QScrollArea* contentScrollArea;
    // dock sprites
    QTreeWidget* addedSpritesTreeWidget;

private:
    TPProject projectLoader;
    QStringList spriteFileNames;
};

#endif // MAINWINDOW_HPP
