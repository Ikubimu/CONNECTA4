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
    languageLabel = new QLabel("Select Language:", this);
    languageComboBox = new QComboBox(this);
    languageComboBox->addItem("English");
    languageComboBox->addItem("Español");
    languageComboBox->addItem("Français");

    darkModeLabel = new QLabel("Enable Dark Mode:", this);
    darkModeCheckBox = new QCheckBox(this);

    saveButton = new QPushButton("Save Settings", this);

    // Crear el layout y agregar los elementos
    layout = new QVBoxLayout(this);
    layout->addWidget(languageLabel);
    layout->addWidget(languageComboBox);
    layout->addWidget(darkModeLabel);
    layout->addWidget(darkModeCheckBox);
    layout->addWidget(saveButton);

    // Conectar los botones con las funciones correspondientes
    connect(languageComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeLanguage(int)));
    connect(darkModeCheckBox, SIGNAL(toggled(bool)), this, SLOT(toggleDarkMode(bool)));
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

    // Mensaje de confirmación
    QMessageBox::information(this, "Settings Saved", "Your settings have been saved successfully.");
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

void SettingsUser::toggleDarkMode(bool checked)
{
    // Activar o desactivar el modo oscuro
    if (checked) {
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
}
