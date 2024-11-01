#include "Formula.h"
#include<iostream>

Formula::Formula(const std::string& data) : data(data) {

	this->isCalculated = false; //only formulas

}

Cell* Formula::clone() const {

	return new Formula(*this);

}

bool Formula::storeCell(std::ofstream& stream) const {

	stream << this->data;
	return stream.good();

}

void Formula::print()const {

	std::cout << this->data;

}

std::string Formula::getData() const {

	return this->data;

}