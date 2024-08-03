#include "MainWindow.hpp"
#include "utils/Utils.hpp"
#include <QDebug>
#include "views/DataSettingView.hpp"
#include "views/TextureSettingView.hpp"
#include "views/SpriteSettingView.hpp"
#include "packer/Packer.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    createContentView();
    createActions();
    createMenus();
    createToolbar();
    createStatusbar();
    createSpritesTreeview();
    createExporterSettingPane();
    initConnections();

    setWindowTitle(tr("TexturePacker"));

    readSettings();

    imagePacker = packer::ImagePackerPtr(new packer::ImagePacker);
    atlasExporter = exporter::ExporterPtr(new exporter::CocosExporter);
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

    // publish
    publishAction = new QAction(tr("&Publish sprite sheet"), this);
    publishAction->setStatusTip(tr("Publish sprite sheet"));
    publishAction->setIcon(QIcon(":/res/ic_publish_project.png"));
    publishAction->setIconVisibleInMenu(true);
    publishAction->setIconText(tr("Publish sprite sheet"));

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
    empty->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    empty->setFixedWidth(50);
    fileToolbar->addWidget(empty);

    fileToolbar->addAction(publishAction);

    empty = new QWidget();
    empty->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    fileToolbar->addWidget(empty);

    fileToolbar->addAction(optionAction);

    setUnifiedTitleAndToolBarOnMac(true);
}

void MainWindow::createStatusbar() {
    statusBar = new QStatusBar();
    this->setStatusBar(statusBar);

    statusLabel = new QLabel(tr("Empty"));
    statusBar->addWidget(statusLabel);
}

void MainWindow::createSpritesTreeview() {
    QDockWidget *dock = new QDockWidget(tr("Sprites"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea);
    dock->setFeatures(QDockWidget::DockWidgetFeature::NoDockWidgetFeatures);
    addDockWidget(Qt::LeftDockWidgetArea, dock);
    dock->setObjectName("spritesDock");

    addedSpritesTreeWidget = new QTreeWidget(dock);
//    addedSpritesTreeWidget->setFocusPolicy(Qt::NoFocus);
//    addedSpritesTreeWidget->setStyleSheet("QTreeWidget::item:selected { \
//                                                background-color: blue; \
//                                          }");
    addedSpritesTreeWidget->setSelectionMode(QAbstractItemView::SelectionMode::ExtendedSelection);
    addedSpritesTreeWidget->setColumnCount(1);
    dock->setWidget(addedSpritesTreeWidget);
    addedSpritesTreeWidget->header()->close();
}

void MainWindow::createExporterSettingPane() {
    QDockWidget *dock = new QDockWidget(tr("Settings"), this);
    dock->setAllowedAreas(Qt::RightDockWidgetArea);
    dock->setFeatures(QDockWidget::DockWidgetFeature::NoDockWidgetFeatures);
    addDockWidget(Qt::RightDockWidgetArea, dock);
    dock->setObjectName("exporterSettingsDock");

    settingScrollArea = new QScrollArea(dock);
    dock->setWidget(settingScrollArea);

    DataSettingView* dataSettingView = new DataSettingView();
    dataSettingView->setFixedHeight(400);

    TextureSettingView* textureSettingView = new TextureSettingView();
    textureSettingView->setFixedHeight(400);

    SpriteSettingView* spriteSettingView = new SpriteSettingView();
    spriteSettingView->setFixedHeight(400);

    QVBoxLayout* scrollLayout = new QVBoxLayout();
    scrollLayout->addWidget(dataSettingView);
    scrollLayout->addWidget(textureSettingView);
    scrollLayout->addWidget(spriteSettingView);
//    scrollLayout->setContentsMargins(0);

    QWidget* scrollWidget = new QWidget();
    scrollWidget->setLayout(scrollLayout);
//    scrollWidget->setFixedSize(1000, 1000);
    settingScrollArea->setWidgetResizable(true);
    settingScrollArea->setWidget(scrollWidget);
}

void MainWindow::createContentView() {
    contentScrollArea = new QScrollArea();
    contentScrollArea->setBackgroundRole(QPalette::Dark);
    setCentralWidget(contentScrollArea);

    atlasPreview = new AtlasTextureView();
    contentScrollArea->setWidgetResizable(true);
    contentScrollArea->setWidget(atlasPreview);
}

void MainWindow::initConnections() {
    connect(addSpriteAction, &QAction::triggered, this, &MainWindow::onAddSprites);
    connect(addSmartFolderAction, &QAction::triggered, this, &MainWindow::onAddSmartFolder);
    connect(removeSpriteAction, &QAction::triggered, this, &MainWindow::removeSelectedSprites);
    connect(addedSpritesTreeWidget, &QTreeWidget::itemClicked, this, &MainWindow::spriteItemClicked);
    connect(publishAction, &QAction::triggered, this, &MainWindow::onPublishSpriteSheet);

    connect(this, &MainWindow::renderedImage, atlasPreview, &AtlasTextureView::updatePixmap);
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
        const QRect availableGeometry = QApplication::primaryScreen()->availableGeometry();
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
    if (!projectLoader.isModified()) return true;
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

void MainWindow::onAddSprites() {
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    QString lastOpenDirectory = settings.value("lastDirectory", "").toString();
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Add sprites"), lastOpenDirectory, tr("Image Files (*.png *.jpg *.jpeg *.gif)"));
    QStringList loadingFilePaths;
    for (int i = 0; i < fileNames.size(); i++) {
        QString path = fileNames[i];
        if (Utils::isImage(path)) {
            loadingFilePaths.append(QDir::toNativeSeparators(path));
        }
    }

    if (loadingFilePaths.size() > 0) {
        QFileInfo fileInfo = QFileInfo(loadingFilePaths[0]);
        lastOpenDirectory = fileInfo.absoluteDir().path();
        settings.setValue("lastDirectory", lastOpenDirectory);
        loadFiles(loadingFilePaths);
    }
}

void MainWindow::onAddSmartFolder() {
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    QString lastOpenDirectory = settings.value("lastDirectory", "").toString();
    QString dir = QFileDialog::getExistingDirectory(this, tr("Select directory"), lastOpenDirectory);
    if(dir.length() > 0) {
        lastOpenDirectory = QDir::toNativeSeparators(dir);
        settings.setValue("lastDirectory", lastOpenDirectory);

        loadDirectory(lastOpenDirectory);
    }
}

void MainWindow::loadDirectory(const QString& directory) {
    packer::ConfigurationsPtr& configs = projectLoader.getConfigurations();
    if (!configs->folders.contains(directory)) {
        configs->folders.append(directory);

        QString folderName = Utils::getFileName(directory);
        QTreeWidgetItem* rootItem = new QTreeWidgetItem(addedSpritesTreeWidget, QStringList(Utils::getFileName(folderName)));
        rootItem->setIcon(0, QIcon(":/res/ic_browse.png"));

        QStringList filePaths = Utils::enumerateContentInDirectory(directory, configs->isRecursive);
        for (int i = 0; i < filePaths.size(); ++i) {
            QString path = filePaths[i];
            packer::Sprite s(path);
            imagePacker->addSprite(s);

            QTreeWidgetItem* item = new QTreeWidgetItem(rootItem, QStringList(Utils::getFileName(path)));
            item->setIcon(0, QIcon(path));
            rootItem->addChild(item);
        }
        rootItem->setExpanded(true);

        updateSpriteSheet();
    }
}

void MainWindow::loadFiles(const QStringList &filePaths) {

    packer::ConfigurationsPtr& configs = projectLoader.getConfigurations();

    QList<QTreeWidgetItem *> items;
    for (int i = 0; i < filePaths.size(); ++i) {
        QString path = filePaths[i];
        if (!configs->files.contains(path)) {
            configs->files.append(path);
            packer::Sprite s(path);
            imagePacker->addSprite(s);

            QTreeWidgetItem* item = new QTreeWidgetItem(addedSpritesTreeWidget, QStringList(Utils::getFileName(path)));
            item->setIcon(0, QIcon(path));
            items.append(item);
        }
    }
    addedSpritesTreeWidget->addTopLevelItems(items);

    updateSpriteSheet();
}

void MainWindow::removeSelectedSprites() {
    QList<QTreeWidgetItem*> selectedItems = addedSpritesTreeWidget->selectedItems();
    for (int i = 0; i < selectedItems.size(); i++) {
        QTreeWidgetItem* item = selectedItems[i];
        int index = addedSpritesTreeWidget->indexOfTopLevelItem(item);
        if (index < 0) {
            QTreeWidgetItem* parent = item->parent();
            parent->removeChild(item);
        } else {
            addedSpritesTreeWidget->takeTopLevelItem(index);
        }
    }

    updateSpriteSheet();
}

void MainWindow::updateSpriteSheet() {
    quint64 area = 0;

    packer::ConfigurationsPtr& configs = projectLoader.getConfigurations();
    imagePacker->pack(configs);

    QList<QImage> textures;
    for(int i = 0; i < imagePacker->bins.size(); i++)
    {
        QImage texture(imagePacker->bins.at(i).width(), imagePacker->bins.at(i).height(), QImage::Format_ARGB32);
        texture.fill(Qt::transparent);
        textures << texture;
    }
    for(int i = 0; i < imagePacker->sprites.size(); i++)
    {
        if(imagePacker->sprites.at(i).pos == QPoint(999999, 999999))
        {
            continue;
        }
        if(imagePacker->sprites.at(i).duplicatedSprite != NULL && configs->merge)
        {
            continue;
        }
        QPoint pos(imagePacker->sprites.at(i).pos.x() + configs->border.left, imagePacker->sprites.at(i).pos.y() + configs->border.top);
        QSize size;
        QRect crop;
        if(!configs->cropThreshold)
        {
            size = imagePacker->sprites.at(i).size;
            crop = QRect(0, 0, size.width(), size.height());
        }
        else
        {
            size = imagePacker->sprites.at(i).crop.size();
            crop = imagePacker->sprites.at(i).crop;
        }
        QImage img = imagePacker->sprites[i].texture;
        if(imagePacker->sprites.at(i).rotated)
        {
            QTransform myTransform;
            myTransform.rotate(90);
            img = img.transformed(myTransform);
            size.transpose();
            crop = QRect(imagePacker->sprites.at(i).size.height() - crop.y() - crop.height(),
                         crop.x(), crop.height(), crop.width());
        }
        if(imagePacker->sprites.at(i).textureId < imagePacker->bins.size())
        {
            QPainter p(&textures.operator [](imagePacker->sprites.at(i).textureId));
            if(configs->extrude)
            {
                QColor color1 = QColor::fromRgba(img.pixel(crop.x(), crop.y()));
                p.setPen(color1);
                p.setBrush(color1);
                if(configs->extrude == 1)
                {
                    p.drawPoint(QPoint(pos.x(), pos.y()));
                }
                else
                {
                    p.drawRect(QRect(pos.x(), pos.y(), configs->extrude - 1, configs->extrude - 1));
                }

                QColor color2 = QColor::fromRgba(img.pixel(crop.x(), crop.y() + crop.height() - 1));
                p.setPen(color2);
                p.setBrush(color2);
                if(configs->extrude == 1)
                {
                    p.drawPoint(QPoint(pos.x(), pos.y() + crop.height() + configs->extrude));
                }
                else
                {
                    p.drawRect(QRect(pos.x(), pos.y() + crop.height() + configs->extrude, configs->extrude - 1, configs->extrude - 1));
                }

                QColor color3 = QColor::fromRgba(img.pixel(crop.x() + crop.width() - 1,
                                                 crop.y()));
                p.setPen(color3);
                p.setBrush(color3);
                if(configs->extrude == 1)
                {
                    p.drawPoint(QPoint(pos.x() + crop.width() + configs->extrude, pos.y()));
                }
                else
                {
                    p.drawRect(QRect(pos.x() + crop.width() + configs->extrude, pos.y(),
                                     configs->extrude - 1, configs->extrude - 1));
                }

                QColor color4 = QColor::fromRgba(img.pixel(crop.x() + crop.width() - 1,
                                                 crop.y() + crop.height() - 1));
                p.setPen(color4);
                p.setBrush(color4);
                if(configs->extrude == 1)
                {
                    p.drawPoint(QPoint(pos.x() + crop.width() + configs->extrude,
                                       pos.y() + crop.height() + configs->extrude));
                }
                else
                {
                    p.drawRect(QRect(pos.x() + crop.width() + configs->extrude,
                                     pos.y() + crop.height() + configs->extrude, configs->extrude - 1,
                                     configs->extrude - 1));
                }

                p.drawImage(QRect(pos.x(), pos.y() + configs->extrude, configs->extrude,
                                  crop.height()), img, QRect(crop.x(), crop.y(), 1, crop.height()));
                p.drawImage(QRect(pos.x() + crop.width() + configs->extrude,
                                  pos.y() + configs->extrude, configs->extrude, crop.height()), img,
                            QRect(crop.x() + crop.width() - 1, crop.y(), 1, crop.height()));

                p.drawImage(QRect(pos.x() + configs->extrude, pos.y(), crop.width(),
                                  configs->extrude), img, QRect(crop.x(), crop.y(), crop.width(), 1));
                p.drawImage(QRect(pos.x() + configs->extrude,
                                  pos.y() + crop.height() + configs->extrude, crop.width(), configs->extrude), img,
                            QRect(crop.x(), crop.y() + crop.height() - 1, crop.width(), 1));

                p.drawImage(pos.x() + configs->extrude, pos.y() + configs->extrude, img, crop.x(),
                            crop.y(), crop.width(), crop.height());
            }
            else
            {
                p.drawImage(pos.x(), pos.y(), img, crop.x(), crop.y(), crop.width(),
                            crop.height());
            }
        }
    }
    for(int i = 0; i < textures.count(); i++)
    {
        area += textures.at(i).width() * textures.at(i).height();
    }
    float percent = (((float)imagePacker->area / (float)area) * 100.0f);
    float percent2 = (float)(((float)imagePacker->neededArea / (float)area) * 100.0f);
    statusLabel->setText(tr("Preview: ") +
                         QString::number(percent) + QString("% filled, ") +
                         (imagePacker->missingImages == 0 ? QString::number(imagePacker->missingImages) +
                          tr(" images missed,") :
                          QString("<font color=red><b>") + QString::number(imagePacker->missingImages) +
                          tr(" images missed,") + "</b></font>") +
                         " " + QString::number(imagePacker->mergedImages) + tr(" images merged, needed area: ")
                         +
                         QString::number(percent2) + "%." + tr(" KBytes: ") + QString::number(
                             area * 4 / 1024));

    emit renderedImage(textures);
}

void MainWindow::newProject() {
    projectLoader = TPProject();
}

void MainWindow::spriteItemClicked(QTreeWidgetItem *item, int column) {
    Q_UNUSED(item);
    Q_UNUSED(column);
}

void MainWindow::onPublishSpriteSheet() {
    publishAtlas();
}

void MainWindow::publishAtlas() {
    packer::ConfigurationsPtr& configs = projectLoader.getConfigurations();
    imagePacker->pack(configs);

    exporter::ConfigurationsPtr exportConfig = exporter::ConfigurationsPtr(new exporter::Configurations);
    exportConfig->dataFilePath = "/Data/GitHub/test.plist";
    exportConfig->outputFormat = "png";
    if (exporter::Export_OK == atlasExporter->exportSpriteSheet(imagePacker, exportConfig)) {
        QMessageBox::information(this, tr("Done"), tr("Export successfull"));
    } else {
        QMessageBox::critical(this, tr("Error"), tr("Export failed"));
    }
}




