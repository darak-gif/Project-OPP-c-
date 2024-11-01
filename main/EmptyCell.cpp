#include "EmptyCell.h"
#include<iostream>

Cell* EmptyCell::clone() const {

	return new EmptyCell(*this);

}

bool EmptyCell::storeCell(std::ofstream& stream) const {

	stream << " ";
	return stream.good();

}

void EmptyCell::print()const {

	std::cout << " ";

}

std::string EmptyCell::getData() const {

	return " ";

}