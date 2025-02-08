#include "dialog.h"

#include <QApplication>
#//-----------------------------------------------------------------------------------------------------------------
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>
#include <filesystem>
#include <cctype>

#include <RoundedDouble.h>
namespace fs = std::filesystem;
#//---------------------------------------------------------------------------------------------------------------
// Структура для хранения данных об одной аннотации
struct Annotation {
    std::string label;
    std::string id;
    std::vector<std::pair<RoundedDouble, RoundedDouble>> points;
};
#//-----------------------------------------------------------------------------------------------------------------------
int main(
    int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;
    w.show();
#//---------------------------------------------------------------------------------------------------------- \
    // Проверяем наличие аргумента с путем к основной папке
    if (argc < 2) {
        std::cerr << "Использование: " << argv[0] << " <путь_к_основной_папке>\n";
        return 1;
    }

    fs::path mainFolder(argv[1]);

    // Формируем путь к файлу svg_pathname.txt в основной папке
    fs::path svgPathnameFile = mainFolder / "svg_pathname.txt";
    std::ifstream svgPathFile(svgPathnameFile);
    if (!svgPathFile) {
        std::cerr << "Не удалось открыть файл: " << svgPathnameFile << "\n";
        return 1;
    }

    // Считываем первую строку файла (предполагается, что в файле содержится нужный путь)
    std::string svgPathname;
    std::getline(svgPathFile, svgPathname);
    svgPathFile.close();

    // Вектор для хранения всех аннотаций
    std::vector<Annotation> annotations;

    // Перебираем элементы основной папки – ищем подпапки с числовыми именами
    for (const auto& entry : fs::directory_iterator(mainFolder)) {
        if (entry.is_directory()) {
            std::string folderName = entry.path().filename().string();
            bool isNumeric = true;
            for (char c : folderName) {
                if (!std::isdigit(c)) {
                    isNumeric = false;
                    break;
                }
            }
            if (!isNumeric) continue;  // Пропускаем, если имя не число

            // Для папки с именем, например, "1"
            std::string folderNumber = folderName;
            fs::path folderPath = entry.path();

            // Формируем имя файла putyN.svg (например, puty1.svg)
            std::string putyFileName = "puty" + folderNumber + ".svg";
            fs::path putyFilePath = folderPath / putyFileName;
            std::ifstream putyFile(putyFilePath);
            if (!putyFile) {
                std::cerr << "Не удалось открыть файл: " << putyFilePath << "\n";
                continue;
            }

            // Считываем всё содержимое файла putyN.svg
            std::stringstream putyBuffer;
            putyBuffer << putyFile.rdbuf();
            std::string putyContent = putyBuffer.str();
            putyFile.close();

            // С помощью регулярных выражений извлекаем id и label из содержимого SVG.
            // Предполагается, что нужные атрибуты записаны так, например:
            //    id="path244" и inkscape:label="object_class_1"
//            std::regex idRegex(R"(id="([^"]+)")");
//                std::regex labelRegex(R"(inkscape:label="([^"]+)")");
            std::regex idRegex(R"regex(id="([^"]+)")regex");
            std::regex labelRegex(R"regex(inkscape:label="([^"]+)")regex");

                std::smatch idMatch, labelMatch;
            std::string annotationId, annotationLabel;
            if (std::regex_search(putyContent, idMatch, idRegex)) {
                annotationId = idMatch[1].str();
            } else {
                std::cerr << "Атрибут id не найден в файле: " << putyFilePath << "\n";
                continue;
            }
            if (std::regex_search(putyContent, labelMatch, labelRegex)) {
                annotationLabel = labelMatch[1].str();
            } else {
                std::cerr << "Атрибут label не найден в файле: " << putyFilePath << "\n";
                continue;
            }

            // Считываем координаты из файлов x_coordinates.txt и y_coordinates.txt
            fs::path xCoordPath = folderPath / "x_coordinates.txt";
            fs::path yCoordPath = folderPath / "y_coordinates.txt";
            std::ifstream xFile(xCoordPath);
            std::ifstream yFile(yCoordPath);
            if (!xFile) {
                std::cerr << "Не удалось открыть файл: " << xCoordPath << "\n";
                continue;
            }
            if (!yFile) {
                std::cerr << "Не удалось открыть файл: " << yCoordPath << "\n";
                continue;
            }
            std::vector<RoundedDouble> xCoords, yCoords;
            std::string line;
            // Читаем координаты X (по одной координате на строку)
            while (std::getline(xFile, line)) {
                if (line.empty()) continue;
                try {
                    xCoords.push_back(std::stod(line));
                } catch (...) {
                    std::cerr << "Неверный формат координаты в файле: " << xCoordPath << "\n";
                }
            }
            // Читаем координаты Y
            while (std::getline(yFile, line)) {
                if (line.empty()) continue;
                try {
                    yCoords.push_back(std::stod(line));
                } catch (...) {
                    std::cerr << "Неверный формат координаты в файле: " << yCoordPath << "\n";
                }
            }
            xFile.close();
            yFile.close();

            if (xCoords.size() != yCoords.size()) {
                std::cerr << "Несоответствие количества координат в " << folderPath << "\n";
                continue;
            }

            // Формируем объект аннотации и заполняем его данными
            Annotation ann;
            ann.label = annotationLabel;
            ann.id = annotationId;
            for (size_t i = 0; i < xCoords.size(); ++i) {
                ann.points.push_back({xCoords[i], yCoords[i]});
            }
            annotations.push_back(ann);
        }
    }

    // Формируем итоговую JSON-структуру в виде строки
    std::ostringstream json;
    json << "{\n";
    json << "  \"image\": \"" << svgPathname << "\",\n";
    json << "  \"annotations\": [\n";

    // Перебираем все аннотации
    for (size_t i = 0; i < annotations.size(); ++i) {
        const Annotation& ann = annotations[i];
        json << "    {\n";
        json << "      \"label\": \"" << ann.label << "\",\n";
        json << "      \"points\": [\n";
        // Записываем массив точек
        for (size_t j = 0; j < ann.points.size(); ++j) {
            json << "        [" << ann.points[j].first << ", " << ann.points[j].second << "]";
            if (j + 1 < ann.points.size())
                json << ",";
            json << "\n";
        }
        json << "      ],\n";
        json << "      \"id\": \"" << ann.id << "\"\n";
        json << "    }";
        if (i + 1 < annotations.size())
            json << ",";
        json << "\n";
    }
    json << "  ]\n";
    json << "}\n";

    // Записываем сформированную строку JSON в файл json.JSON в основной папке
    fs::path jsonFilePath = mainFolder / "json.JSON";
    std::ofstream outFile(jsonFilePath);
    if (!outFile) {
        std::cerr << "Не удалось создать файл: " << jsonFilePath << "\n";
        return 1;
    }
    outFile << json.str();
    outFile.close();

    std::cout << "JSON файл успешно создан: " << jsonFilePath << "\n";
    return 0;
#//------------------------------------------------------------------------------------------------------
    return a.exec();
}
