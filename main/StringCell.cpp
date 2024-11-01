#include "StringCell.h"
#include<iostream>

StringCell::StringCell(const std::string& data) : data(data) {

    for (size_t i = 0; i < data.length(); i++) { // remove " or \ of data because of print function in table
        if (data[i] == '\\' && data[i + 1] == '\\') {
            this->clear += '\\';
            i += 2; 
        }
        if (data[i] == '\\' && data[i + 1] == '"')
            this->clear += '"';
        if (data[i] != '"' && data[i] != '\\')
            this->clear += data[i];
    }

}

Cell* StringCell::clone() const {

    return new StringCell(*this);

}

bool StringCell::storeCell(std::ofstream& stream) const {

    stream << this->data;
    return stream.good();
    
}

void StringCell::print()const {

    std::cout << this->data;

}

std::string StringCell::getData() const {

    return this->clear;

}