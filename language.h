#ifndef LANGUAGE_H
#define LANGUAGE_H

#include <QObject>
#include <QString>

namespace Labels {
    inline QString password;
    inline QString username;
    inline QString email{};
    inline QString Send_code2email{};
    inline QString forgot_password{};
    inline QString error{};
    inline QString currently_logged{};
    inline QString wrong_email{};
    inline QString code{};
    inline QString code_number{};
    inline QString insert_code{};
    inline QString verify_code{};
    inline QString code2verify{};
    inline QString wrong_code{};
    inline QString victory{};
    inline QString player_win{};
    inline QString empty_column{};
    inline QString empty_column_phrase{};
    inline QString nickname_optional{};
    inline QString won{};
    inline QString lost{};
    inline QString all{};
    inline QString search{};
    inline QString date{};
    inline QString winner{};
    inline QString loser{};
    inline QString login{};
    inline QString logup_newacc{};
    inline QString forgot_password_dude{};
    inline QString user_restriccion_character{};
    inline QString no_spacer{};
    inline QString password_restriccion_character{};
    inline QString password_mayus{};
    inline QString password_minus{};
    inline QString password_number{};
    inline QString password_special{};
    inline QString non_user{};
    inline QString open_login_page{};
    inline QString open_settings{};
    inline QString succesful_registrer{};
    inline QString points{};
    inline QString player_1{};
    inline QString confirm_password{};
    inline QString birthday{};
    inline QString choose_photo_pc{};
    inline QString registrer{};
    inline QString registrer_page{};
    inline QString select_avatar{};
    inline QString fail_uploading_image{};
    inline QString todifferent_password{};
    inline QString wrong_birthday{};
    inline QString user_already_exist{};
    inline QString try2logup_again{};
    inline QString select_language{};
    inline QString dark_mode{};
    inline QString save_settings{};
    inline QString language_selected{};
    inline QString settings_saved{};
    inline QString succesful_saved_settings{};
    inline QString user_already_logged{};
    inline QString nonvalid_email{};
    inline QString enable_dark_mode{};
};

class Language : public QObject
{
    Q_OBJECT
public:
    static Language& instance(); // Singleton
    static void change_to_tortilla_patata();
    static void change_to_fish_and_chips();
    static void change_to_Mbappe();

signals:
    void languageChanged(); //señal para cuando cambiemos el idioma

private:
    explicit Language(QObject* parent = nullptr);
};

#endif // LANGUAGE_H
