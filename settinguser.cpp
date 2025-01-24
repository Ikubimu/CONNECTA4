#include "settinguser.h"
#include <QMessageBox>
#include <QDebug>
#include <QApplication>
#include <QTranslator>
#include <QStyleFactory>
#include <QMouseEvent>
#include "language.h"

SettingsUser::SettingsUser(QWidget *parent)
    : QWidget(parent)
{
    // Crear los elementos de la interfaz
    languageLabel = new QLabel(Labels::select_language, this);
    languageComboBox = new QComboBox(this);
    languageComboBox->addItem("Español");
    languageComboBox->addItem("English");
    languageComboBox->addItem("Français");

    darkModeLabel = new QLabel(Labels::enable_dark_mode, this);
    darkModeCheckBox = new QCheckBox(this);

    saveButton = new QPushButton(Labels::save_settings, this);

    // Crear el layout y agregar los elementos
    layout = new QVBoxLayout(this);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(10);
    layout->addWidget(languageLabel);
    layout->addWidget(languageComboBox);
    layout->addWidget(darkModeLabel);
    layout->addWidget(darkModeCheckBox);
    layout->addWidget(saveButton);

    this->setAutoFillBackground(true);
    QPalette palette = this->palette();
    //palette.setColor(QPalette::Window, Qt::white);   //no le pongo color porq si no al cambiar de modo se lia
    this->setPalette(palette);

    // Conectar las señales y slots
    connect(languageComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeLanguage(int)));
    connect(saveButton, &QPushButton::clicked, this, &SettingsUser::saveSettings);

    // Filtrar eventos para cerrar con clics externos
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog | Qt::WindowStaysOnTopHint);
    this->installEventFilter(this);

    // Inicializar en modo claro
    applyLightMode();
}

SettingsUser::~SettingsUser()
{
}

void SettingsUser::saveSettings()
{
    QString selectedLanguage = languageComboBox->currentText();
    bool isDarkMode = darkModeCheckBox->isChecked();

    int idx = languageComboBox->currentIndex();

    emit languagechoosed(idx);

    qDebug() << "Language selected: " << selectedLanguage;
    qDebug() << "Dark Mode: " << (isDarkMode ? "Enabled" : "Disabled");

    if (isDarkMode) {
        applyDarkMode();
    } else {
        applyLightMode();
    }

    languageLabel->setText(Labels::select_language);


    darkModeLabel->setText(Labels::enable_dark_mode);


    saveButton->setText(Labels::save_settings);


    QMessageBox::information(this, Labels::settings_saved, Labels::succesful_saved_settings);
    this->close(); // Cierra el widget después de guardar
}



void SettingsUser::mousePressEvent(QMouseEvent *event)
{
    if (!this->rect().contains(event->pos())) {
        this->close(); // Cerrar si es clic externo
    }
    QWidget::mousePressEvent(event);
}

bool SettingsUser::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::WindowDeactivate) {
        this->close(); // Cerrar si la ventana pierde foco
        return true;
    }
    return QWidget::eventFilter(watched, event);
}

void SettingsUser::applyDarkMode()
{
    qApp->setStyle(QStyleFactory::create("Fusion"));
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(42, 42, 42));
    darkPalette.setColor(QPalette::AlternateBase, QColor(66, 66, 66));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::Highlight, QColor(0, 120, 215));
    darkPalette.setColor(QPalette::HighlightedText, Qt::white);
    qApp->setPalette(darkPalette);
}

void SettingsUser::applyLightMode()
{
    qApp->setStyle(QStyleFactory::create("Windows"));
    qApp->setPalette(qApp->style()->standardPalette());
}
