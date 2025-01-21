#ifndef SETTINGSUSER_H
#define SETTINGSUSER_H

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QLabel>

class SettingsUser : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsUser(QWidget *parent = nullptr);
    ~SettingsUser();

private slots:
    void saveSettings();  // Guardar las configuraciones
    void changeLanguage(int index);  // Cambiar el idioma seleccionado

private:
    QComboBox *languageComboBox;  // ComboBox para seleccionar el idioma
    QCheckBox *darkModeCheckBox;  // CheckBox para activar/desactivar modo oscuro
    QPushButton *saveButton;      // Botón para guardar los cambios
    QVBoxLayout *layout;          // Layout principal

    QLabel *languageLabel;        // Etiqueta para el combo de idioma
    QLabel *darkModeLabel;        // Etiqueta para el modo oscuro
    QPalette standardPallete;
};

#endif // SETTINGSUSER_H
