//
// Created by david on 13. 12. 2022.
//

#ifndef SYSTEMMONITORINGSERVER_UTILS_H
#define SYSTEMMONITORINGSERVER_UTILS_H

#include <QString>
#include <QDebug>
#include <chrono>
#include <thread>

enum class Color {
    BLACK,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE
};

namespace Utils {
    void println(const QString &text, Color color) {
        QTextStream out(stdout);
//        color = Color::WHITE; // Temporary
//        switch (color) {
//            case Color::WHITE:
//                system("color 0F");
//                break;
//            case Color::BLACK:
//                system("color 00");
//                break;
//            case Color::RED:
//                system("color 04");
//                break;
//            case Color::GREEN:
//                system("color 02");
//                break;
//            case Color::YELLOW:
//                system("color 06");
//                break;
//            case Color::BLUE:
//                system("color 01");
//                break;
//            case Color::MAGENTA:
//                system("color 05");
//                break;
//            case Color::CYAN:
//                system("color 03");
//                break;
//        }
        out << "";
        out << text << "\n";
        out.reset();
    }
}

#endif //SYSTEMMONITORINGSERVER_UTILS_H
