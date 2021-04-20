#include <iostream>
#include "NumberWithUnits.hpp"
#include <fstream>
#include <sstream>
#include <map>
#include <string>
#include <math.h>
#include <stdexcept>

using namespace std;

namespace ariel
{
    static map<string, map<string, double>> _convert_table;
    void NumberWithUnits::print_units()
    {
        map<string, map<string, double>>::iterator itr;
        map<string, double>::iterator ptr;
        for (itr = _convert_table.begin(); itr != _convert_table.end(); itr++)
        {
            cout << itr->first << " : ";

            for (ptr = itr->second.begin(); ptr != itr->second.end(); ptr++)
            {
                cout << "{" << ptr->first << ": " << ptr->second << "} ,";
            }
            cout << endl;
        }
    }
    static istream &getAndCheckNextCharIs(istream &input, char expectedChar)
    {
        char actualChar;
        input >> actualChar;
        if (!input)
        {
            return input;
        }
        if (actualChar != expectedChar)
        {
            input.setstate(ios::failbit);
        }
        return input;
    }
    NumberWithUnits::NumberWithUnits()
    {
        this->_num = 0;
        this->_type = "";
    }
    NumberWithUnits::NumberWithUnits(const double &num, const string &type)
    {
        if (_convert_table.find(type) != _convert_table.end())
        {
            _num = num;
            _type = type;
        }
        else
        {
            throw invalid_argument("[" + type + "] unit does not exist at the following convert table");
        }
    }
    void NumberWithUnits::read_units(ifstream &file)
    {
        // map<string, map<string, double>> _convert_table;
        double first_num = ZERO;
        double num_after_convert = ZERO;
        string convert_from;
        string convert_to;
        string equal;

        while (file >> first_num >> convert_from >> equal >> num_after_convert >> convert_to)
        {
            _convert_table[convert_from][convert_to] = num_after_convert;
            _convert_table[convert_to][convert_from] = first_num / num_after_convert;
        }
        calculate_consequence();
    }

    double unit_calculate(const NumberWithUnits &a, const NumberWithUnits &b)
    {
        double factor_unit = ONE;
        if (a.type() == b.type())
        {
            return b.num();
        }

        try
        {
            factor_unit = b.num() * _convert_table.at(b.type()).at(a.type());
            return factor_unit;
        }
        catch (const std::exception &e)
        {
            throw invalid_argument("Units do not match - [" + a.type() + "] cannot be converted to [" + b.type() + "]");
        }
    }

    void NumberWithUnits::calculate_consequence()
    {

        for (auto &first : _convert_table)
        {
            for (auto &second : first.second)
            {
                for (auto &third : _convert_table[second.first])
                {

                    if (_convert_table[third.first].find(first.first) == _convert_table[third.first].end() &&
                        first.first != third.first)
                        ;
                    {
                        _convert_table[first.first][third.first] = _convert_table[second.first][third.first] * _convert_table[first.first][second.first];
                        _convert_table[third.first][first.first] = 1 / _convert_table[first.first][third.first];
                    }
                }
            }
        }
    }
    NumberWithUnits &NumberWithUnits::operator+=(const NumberWithUnits &other)
    {
        _num += unit_calculate(*this, other);
        return *this;
    }
    NumberWithUnits operator+(const NumberWithUnits &a, const NumberWithUnits &b)
    {
        return NumberWithUnits(a.num() + unit_calculate(a, b), a.type());
    }

    NumberWithUnits operator-(const NumberWithUnits &a, const NumberWithUnits &b)
    {
        return NumberWithUnits(a.num() - unit_calculate(a, b), a.type());
    }

    NumberWithUnits &NumberWithUnits::operator-=(const NumberWithUnits &other)
    {
        _num -= unit_calculate(*this, other);
        return *this;
    }
    bool operator<(const NumberWithUnits &a, const NumberWithUnits &b)
    {
        return (a.num() < (unit_calculate(a, b)));
    }
    bool operator>(const NumberWithUnits &a, const NumberWithUnits &b)
    {
        return (a.num() > (unit_calculate(a, b)));
    }
    /* Using older operator to check  if it is true or false */
    bool operator<=(const NumberWithUnits &a, const NumberWithUnits &b)
    {
        return (a == b || a < b);
    }
    /* Using older operator to check  if it is true or false */
    bool operator>=(const NumberWithUnits &a, const NumberWithUnits &b)
    {
        return (a == b || a > b);
    }
    bool operator==(const NumberWithUnits &a, const NumberWithUnits &b)
    {

        return (abs(a.num() - unit_calculate(a, b)) <= TOLERANCE);
    }
    bool operator!=(const NumberWithUnits &a, const NumberWithUnits &b)
    {
        return (!(a == b));
    }
    NumberWithUnits operator*(const double a, const NumberWithUnits &b)
    {
        return NumberWithUnits(a * b.num(), b.type());
    }
    NumberWithUnits operator*(const NumberWithUnits &b, const double a)
    {
        return NumberWithUnits(a * b.num(), b.type());
    }
    ostream &operator<<(ostream &os, const NumberWithUnits &a)
    {
        return (os << a._num << "[" << a._type << "]");
    }

    bool getAndChceckNextStringIs(string &new_type)
    {
        string type;
        u_long index = 0;
        while (index < new_type.length())
        {
            if (new_type[index] == ']')
            {
                new_type = type;
                return true;
            }
            else
            {
                type += new_type[index++];
            }
        }
        return false;
    }
    istream &operator>>(istream &input, NumberWithUnits &a)
    {
        string new_type;
        double new_num = 0;

        // ios::pos_type startPosition = input.tellg();
        if ((!(input >> new_num)) ||
            (!(getAndCheckNextCharIs(input, '['))) ||
            (!(input >> new_type)) ||
            (!(getAndCheckNextCharIs(input, ']'))))
        {
            if (!(getAndChceckNextStringIs(new_type)))
            {
                throw invalid_argument("Bad format exception");
            }
            else
            {
                if (_convert_table.find(new_type) != _convert_table.end())
                {
                    a._num = new_num;
                    a._type = new_type;
                }
                else
                {
                    throw invalid_argument("Bad format exception");
                }
            }
        }
        else
        {
            if (_convert_table.find(new_type) != _convert_table.end())
            {
                a._num = new_num;
                a._type = new_type;
            }
            else
            {
                throw invalid_argument("Bad format exception");
            }
        }

        return input;
    }
}