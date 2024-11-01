#include "IntCell.h"
#include<iostream>
#include<string>

IntCell::IntCell(int data) : data(data) {}

Cell* IntCell::clone() const {

    return new IntCell(*this);

}

bool IntCell::storeCell(std::ofstream& stream) const {

    stream << this->data;
    return stream.good();

}

void IntCell::print()const {

    std::cout << this->data;

}

std::string IntCell::getData() const {

    std::string str = std::to_string(this->data);

    return str;
}