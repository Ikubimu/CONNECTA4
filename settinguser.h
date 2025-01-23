#ifndef SETTINGSUSER_H
#define SETTINGSUSER_H

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QEvent>
#include <QMouseEvent>
#include <QTranslator>
#include "language.h"

class SettingsUser : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsUser(QWidget *parent = nullptr);
    ~SettingsUser();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    void saveSettings();  // Guardar las configuraciones
    void changeLanguage(int index);  // Cambiar el idioma seleccionado

private:
    void applyDarkMode();   // Aplicar modo oscuro
    void applyLightMode();  // Aplicar modo claro

    QComboBox *languageComboBox;  // ComboBox para seleccionar el idioma
    QCheckBox *darkModeCheckBox;  // CheckBox para activar/desactivar modo oscuro
    QPushButton *saveButton;      // Botón para guardar los cambios
    QVBoxLayout *layout;          // Layout principal

    QLabel *languageLabel;        // Etiqueta para el combo de idioma
    QLabel *darkModeLabel;        // Etiqueta para el modo oscuro
};

#endif // SETTINGSUSER_H
