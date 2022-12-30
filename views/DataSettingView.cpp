#include "DataSettingView.hpp"
#include "SettingLabel.hpp"

DataSettingView::DataSettingView(QWidget *parent) : SettingPanel(parent)
{
    headerButton->setText(tr("Data"));

    QHBoxLayout* layout = new QHBoxLayout();
    SettingLabel* dataFormatLabel = new SettingLabel(tr("Data format"));
    dataFormatLabel->setAlignment(Qt::AlignRight);
    dataFormatLabel->setFixedWidth(120);
    QStringList supportedFormats = {"cocos2d-iphone", "cocos2d-x", "libGDX"};
    QComboBox* comboBox = new QComboBox();
    comboBox->setEditable(false);
    comboBox->addItems(supportedFormats);
    layout->addWidget(dataFormatLabel);
    layout->addWidget(comboBox);
    mainLayout->addLayout(layout);

    layout = new QHBoxLayout();
    SettingLabel* dataFileLabel = new SettingLabel(tr("Data file"));
    dataFileLabel->setAlignment(Qt::AlignRight);
    dataFileLabel->setFixedWidth(120);
    layout->addWidget(dataFileLabel);
    QLineEdit* textfield = new QLineEdit();
    textfield->setFixedHeight(50);
    layout->addWidget(textfield);
    QPushButton* browseDataFileButton = new QPushButton();
    browseDataFileButton->setStyleSheet("QPushButton{border: none;outline: none;}");
    browseDataFileButton->setIcon(QIcon(":/res/ic_browse.png"));
    browseDataFileButton->setFixedSize(30, 30);
    layout->addWidget(browseDataFileButton);
    mainLayout->addLayout(layout);

    mainLayout->addStretch();
}
