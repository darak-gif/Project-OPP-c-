#include "DoubleCell.h"
#include<iostream>
#include<string>

DoubleCell::DoubleCell(double data) : data(data) {}

Cell* DoubleCell::clone() const {

	return new DoubleCell(*this);

}

bool DoubleCell::storeCell(std::ofstream& stream) const {

	stream << this->data;
	return stream.good();

}

void DoubleCell::print()const {

	std::cout << this->data;

}

std::string DoubleCell::getData() const {

	std::string str = std::to_string(this->data);
	for (size_t i = str.size() - 1; str[i] != '.'; i--) {
		if (str[i] == '0') {
			str.erase(i, 1);
		}
	}

	return str;
}