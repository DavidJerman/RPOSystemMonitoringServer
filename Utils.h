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
        out << "";
        out << text << "\n";
        out.reset();
    }
}

#endif //SYSTEMMONITORINGSERVER_UTILS_H
