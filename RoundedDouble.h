#ifndef ROUNDEDDOUBLE_H
#define ROUNDEDDOUBLE_H
//#include <cstdint>
#include <iomanip>
#include <stdint.h>

#include "qobject.h"
#include <cmath>
#include <stdexcept>

#include <iostream>
#include <sstream>
#include <string>
//-------------------------------------------------------------------
extern int peredacha_decimalPlaces;
//-------------------------------------------------------------------
class RoundedDouble {
private:
    double value;  // Само значение типа double
//  extern
        int decimalPlaces;  // Количество знаков после запятой
public:

    // Конструктор, который принимает значение и округляет его
    RoundedDouble(double val, int decimalPlaces = //peredacha_decimalPlaces=
   // 4
  //  8
     //             2
              //    3
            //      5
                 // 6
                  7
                  ) // NOTE: округление к 7 цифрам после запятой
        : value(roundToDecimal(val, decimalPlaces)), decimalPlaces(decimalPlaces) {
        std::cout << std::fixed << std::setprecision(7);
        // отладка:
        // std::cout << "Инициализация RoundedDouble: val = " << val
        //           << ", decimalPlaces = " << decimalPlaces
        //           << ", округлённое значение = " << value << std::endl;
    }
 //-------------------------------------------------------------------
 //int   peredacha_decimalPlaces=decimalPlaces;
//-------------------------------------------------------------------
    // Метод для получения строкового представления RoundedDouble
    // std::string toString() const {
    //     std::ostringstream oss;
    //     oss << value;
    //     return oss.str();
    // }
#//-----------------------------------------------------------------------------
            // Метод для округления до заданного количества знаков
    double roundToDecimal(double val,   int decimalPlaces=7
                          ) const {
       // double
           int scale = std::pow(10, decimalPlaces=7);
       // return std::round(val * scale) / scale;
        double      roundedValue = std::round(val * scale) / scale;
           std::cout << std::fixed << std::setprecision(7);
        // отладка:

        // std::cout << "roundToDecimal: val = " << val
        //           << ", decimalPlaces = " << decimalPlaces
        //           << ", scale = " << scale
        //           << ", округлённое значение = " << roundedValue << std::endl;

        return roundedValue;
    }
//-------------------------------------------------------------------
    // Конструктор по умолчанию
    RoundedDouble() : value(0.0000000) {   std::setlocale(LC_ALL, "C");}
    // Устанавливаем локаль "C" (международная)
    // точка - десятичный разделитель
#//----------------------------------------------------------------------------------------------------- \
    // Перегружаем оператор >> для RoundedDouble \
    // Объявляем перегруженный оператор >> как дружественную функцию
    friend std::istream& operator>>(std::istream& is, RoundedDouble& rd);
#//--------------------------------------------------------------------------------------------------------------------------
    // Конструктор с параметром
  //  RoundedDouble(double val) : value(val) {}

    // Метод для округления к ближайшему целому числу
    double roundNearest() const {
        return std::round(value);
    }

    // Метод для округления вниз (к ближайшему целому снизу)
    double roundDown() const {
        return std::floor(value);
    }

    // Метод для округления вверх (к ближайшему целому сверху)
    double roundUp() const {
        return std::ceil(value);
    }

    // Округление до заданного числа знаков после запятой
    double roundToDecimal(int decimalPlaces=
  //  8
   // 4
   //                       2
                         // 3
                          //5
                         // 6
 7
    ) const { // NOTE: явное округление к 8 цифрам после запятой
    //decimalPlaces=8;
        int scale = std::pow(10, decimalPlaces=7);
        return std::round(value * scale) / scale;
    }

    // Перегрузка оператора приведения к типу double
    operator double() const {
        return value;
    }

    // Перегрузка оператора присваивания для удобного использования
    RoundedDouble& operator=(double val) {
        value = val;
        return *this;
    }

    // Перегрузка операторов арифметики для удобного использования
    RoundedDouble operator+(const RoundedDouble& other) const {
        return RoundedDouble(value + other.value);
    }

    RoundedDouble operator-(const RoundedDouble& other) const {
        return RoundedDouble(value - other.value);
    }

    RoundedDouble operator*(const RoundedDouble& other) const {
        return RoundedDouble(value * other.value);
    }

    RoundedDouble operator/(const RoundedDouble& other) const {
        return RoundedDouble(value / other.value);
    }

    // Метод для получения значения
    double getValue() const {
        return value;
    }

    // Метод для задания значения
    // void setValue(double val) {
    //     value = val;
    // }
    void setValue(double val) {
        value = roundToDecimal(val, decimalPlaces=7);
    }

//-------------------------------------------------------------------
// Перегрузка оператора деления для деления на int
    RoundedDouble operator/(int other) const {
        if (other == 0) {
            throw std::runtime_error("Division by zero!");
        }
        return RoundedDouble(value / static_cast<double>(other));
    }

    // Перегрузка оператора деления для деления на double
    // RoundedDouble operator/(double other) const {
    //     if (other == 0.0) {
    //         throw std::runtime_error("Division by zero!");
    //     }
    //     return RoundedDouble(value / other);
    // }


//-------------------------------------------------------------------    
    // Перегрузка оператора += для другого RoundedDouble
    RoundedDouble& operator+=(const RoundedDouble& other) {
        this->value += other.value;
        return *this;
    }

//-------------------------------------------------------------------


    // Перегрузка оператора * для int
    RoundedDouble operator*(int other) const {
        return RoundedDouble(this->value * other);
    }

    // Перегрузка оператора * для обратного порядка: int * RoundedDouble
    friend RoundedDouble operator*(int lhs, const RoundedDouble& rhs) {
        return RoundedDouble(lhs * rhs.value);
    }
//-------------------------------------------------------------------
    // Перегрузка оператора *= для другого RoundedDouble
    RoundedDouble& operator*=(const RoundedDouble& other) {
        this->value *= other.value;
        return *this;
    }

    // Перегрузка оператора *= для int
    RoundedDouble& operator*=(int other) {
        this->value *= other;
        return *this;
    }
//-------------------------------------------------------------------

    // Перегрузка оператора ввода (чтение из потока)
    friend QDataStream& operator>>(QDataStream& in, RoundedDouble& rd) {
        in >> rd.value;
        return in;
    }

    // Перегрузка оператора вывода (запись в поток)
    friend QDataStream& operator<<(QDataStream& out, const RoundedDouble& rd) {
        out << rd.value;
        return out;
    }

    friend std::ostream& operator<<(std::ostream& os, const RoundedDouble& rd) {
        os << std::fixed << std::setprecision(rd.decimalPlaces) << rd.value;
        return os;
    }

    // friend std::ostream& operator<<(std::ostream& os, const RoundedDouble& rd) {
    //     os << std::fixed << std::setprecision(7) << rd.value; // Настройка формата вывода
    //     return os;
    // }

    // Перегрузка оператора <<
//    QDataStream& operator<<(QDataStream& out, const RoundedDouble& rd) {
//        out << rd.getValue();
//        return out;
//    }

//-------------------------------------------------------------------
    // Установка decimalPlaces и пересчёт значения
    void setDecimalPlaces(int newDecimalPlaces=7) {
        decimalPlaces = newDecimalPlaces;
        value = roundToDecimal(value, decimalPlaces); // Пересчёт с новым количеством знаков
    }
#//-------------------------------------------------------------
      // Метод для получения строкового представления RoundedDouble
    std::string toString() const {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(7) << value; // decimalPlaces
        return oss.str();
    }
#//--------------------------------------------------------------------------------------
    // std::optional<RoundedDouble> getElement(const std::vector<std::array<RoundedDouble, 2>>& points,
    //                                         size_t vectorIndex, size_t arrayIndex) {
    //     if (vectorIndex < points.size() && arrayIndex < points[vectorIndex].size()) {
    //         return points[vectorIndex][arrayIndex];
    //     }
    //     return std::nullopt; // Возвращаем "пустое" значение
    // }

};
#endif // ROUNDEDDOUBLE_H
