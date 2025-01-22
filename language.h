#ifndef LANGUAGE_H
#define LANGUAGE_H

#include <QObject>
#include <QString>

namespace Labels {
    inline QString Password;
    inline QString Username;
    inline QString Caca;
};

class Language : public QObject
{
    Q_OBJECT
public:
    static Language& instance(); // Singleton
    void change_to_spanish();
    void change_to_english();

signals:
    void languageChanged(); //señal para cuando cambiemos el idioma

private:
    explicit Language(QObject* parent = nullptr);
};

#endif // LANGUAGE_H
