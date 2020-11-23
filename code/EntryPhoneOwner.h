#pragma once

#include <iomanip>
#include "FileDataBase.h"

class EntryPhoneOwner: public FileDataBase<int>::Entry {
private:
    int phone_number;
    string address;

public:
    int read(istream &fin) override {
        fin >> phone_number;
        fin.get();
        getline(fin, address);
        return phone_number;
    }

    void print(ostream &fout) const override {
        fout << setw(8) << setfill('0') << right << phone_number
             << ' ' << address;
    }

    int generate() override {
        phone_number = 99999999-(rand())%100000000;
        address = "г. ";
        string cities[] = {
                "Казань", "Сочи", "Калининград", "Ярославль",
                "Екатеринбург", "Анапа", "Владимир", "Тула",
                "Краснодар", "Воронеж", "Суздаль", "Новосибирск",
                "Кострома", "Самара", "Волгоград", "Геленджик",
                "Тверь", "Саратов", "Красноярск", "Псков",
                "Рязань", "Коломна", "Уфа", "Кисловодск",
                "Смоленск", "Челябинск", "Пятигорск", "Иваново",
                "Калуга", "Пермь", "Тюмень", "Омск", "Новороссийск",
                "Владивосток", "Петрозаводск", "Вологда", "Муром",
                "Астрахань", "Чебоксары"
        };
        address += cities[rand()%39];
        address += ", ул. ";
        string streets[] = {
                "Центральная", "Молодежная", "Школьная", "Лесная",
                "Садовая", "Советская", "Новая", "Набережная",
                "Заречная", "Зеленая", "Ленина", "Пушкина", "Толстого"
        };
        address += streets[rand()%13];
        address += ", дом ";
        int house = 1+rand()%16;
        address += to_string(house);
        address += ", кв. ";
        address += to_string(50*house+1+rand()%50);
        return phone_number;
    }
};
