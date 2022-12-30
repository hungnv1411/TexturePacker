#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QVBoxLayout>
#include <QtWidgets>
#include "TPProject.hpp"
#include "AtlasTextureView.hpp"
#include "packer/ImagePacker.hpp"
#include "exporter/CocosExporter.hpp"

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
    void publishAtlas();

    // packer
private:
    void loadFiles(const QStringList& filePaths);
    void loadDirectory(const QString& directory);
protected:
    void closeEvent(QCloseEvent *event) override;

signals:
    void renderedImage(const QList<QImage>& image);
protected slots:
    void onAddSprites();
    void onAddSmartFolder();
    void removeSelectedSprites();
    void onPublishSpriteSheet();
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
    QAction* publishAction;
    QAction* aboutAction;
    // toolbar
    QToolBar* fileToolbar;
    // status bar
    QStatusBar* statusBar;
    QLabel* statusLabel;
    // content view
    QScrollArea* contentScrollArea;
    AtlasTextureView* atlasPreview;
    // dock sprites
    QTreeWidget* addedSpritesTreeWidget;
    // settings
    QScrollArea* settingScrollArea;

private:
    TPProject projectLoader;
    packer::ImagePackerPtr imagePacker;
    exporter::ExporterPtr atlasExporter;
};

#endif // MAINWINDOW_HPP
