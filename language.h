#ifndef LANGUAGE_H
#define LANGUAGE_H
#include <QString>

namespace Labels{
    QString Password{};
    QString Username{};
    QString Caca{};
};

class Language
{
public:
    static void change_to_spanish();
    static void change_to_english();
protected:
    Language();
};

#endif // LANGUAGE_H
