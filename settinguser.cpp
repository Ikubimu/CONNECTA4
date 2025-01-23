#include "settinguser.h"
#include <QMessageBox>
#include <QDebug>
#include <QApplication>
#include <QTranslator>
#include <QStyleFactory>

SettingsUser::SettingsUser(QWidget *parent)
    : QWidget(parent)
{
    // Crear los elementos de la interfaz
    languageLabel = new QLabel(Labels::select_language, this);
    languageComboBox = new QComboBox(this);
    languageComboBox->addItem(Labels::dark_mode);
    languageComboBox->addItem("Español");
    languageComboBox->addItem("Français");

    darkModeLabel = new QLabel(Labels::enable_dark_mode, this);
    darkModeCheckBox = new QCheckBox(this);

    saveButton = new QPushButton(Labels::save_settings, this);

    // Crear el layout y agregar los elementos
    layout = new QVBoxLayout(this);
    layout->addWidget(languageLabel);
    layout->addWidget(languageComboBox);
    layout->addWidget(darkModeLabel);
    layout->addWidget(darkModeCheckBox);
    layout->addWidget(saveButton);

    QRect geom = geometry();

    this->setVisible(false);
    this->setGeometry(parent->pos().x(), parent->pos().y(), 250, 150); // Posición inicial en la esquina inferior izquierda

    // Establecer un color de fondo sólido para el widget de ajustes
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, Qt::white); // Usar QPalette::Window en lugar de QPalette::Background
    this->setAutoFillBackground(true);
    this->setPalette(palette);


    // Conectar los botones con las funciones correspondientes
    connect(languageComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeLanguage(int)));
    connect(saveButton, &QPushButton::clicked, this, &SettingsUser::saveSettings);
}

SettingsUser::~SettingsUser()
{
    // Limpiar recursos si es necesario
}

void SettingsUser::saveSettings()
{
    // Lógica para guardar la configuración (puedes usar archivos o base de datos)
    QString selectedLanguage = languageComboBox->currentText();
    bool isDarkMode = darkModeCheckBox->isChecked();

    qDebug() << "Language selected: " << selectedLanguage;
    qDebug() << "Dark Mode: " << (isDarkMode ? "Enabled" : "Disabled");

    // Activar o desactivar el modo oscuro
    if (isDarkMode) {
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
    } else {

        qApp->setStyle(QStyleFactory::create("Windows"));
        qApp->setPalette(qApp->style()->standardPalette());
    }

    // Mensaje de confirmación
    QMessageBox::information(this, Labels::settings_saved, Labels::succesful_saved_settings);
}

void SettingsUser::changeLanguage(int index)
{
    // Cambiar el idioma dependiendo de la selección del combo
    QString language = languageComboBox->currentText();

    QTranslator *translator = new QTranslator(this);
    if (language == "Español") {
        translator->load(":/translations/app_es.qm");
    } else if (language == "Français") {
        translator->load(":/translations/app_fr.qm");
    } else {
        // Si es inglés o cualquier otro, no cargar ningún archivo
        QApplication::removeTranslator(translator);
    }

    QApplication::installTranslator(translator);
}
