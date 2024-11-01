#include "Table.h"
#include<fstream>
#include<iostream>

Table::Table(const std::string fileName) {

	std::ifstream file(fileName);
	if (!file.is_open()) {
		file.close();

		std::ofstream newFile(fileName); // create new file with wanted name
		newFile.close();

		file.open(fileName);
		if (!file.is_open())
			throw "The file who has to load the table can NOT be open";

	}

	size_t rows = getRows(file);
	file.seekg(0, std::ios::beg);
	size_t cols = getColumns(file);

	this->cells.resize(rows);
	this->valuesOfCells.resize(rows);

	file.clear();
	file.seekg(0, std::ios::beg);

	std::string buff;
	size_t currRows = 0;
	while (file.good() && currRows < rows) {

		std::getline(file, buff, '\n');

		if (buff.empty()) {
			for (size_t i = 0; i < cols; i++) {
				try {
					this->cells[currRows].push_back(Factory::getFactory().creatCell(""));
				}
				catch (std::exception& ex) {
					
					std::cout << "Problem with data at position : [" << currRows+1 << ";" << i+1 <<"]" << std::endl;
					//this->valuesOfCells.resize(this->cells.size());
					destroy();
					throw; 
				}
			}

		}
		else {

			for (size_t i = 0; i < cols; i++) {

				std::string data;
				size_t j;
				for (j = 0; buff[j] != ',' && buff[j] != '\0'; j++) {
					data += buff[j];
					if (buff[j] == ' ' && buff[0] != '"' && buff[0]!=' ') {
						std::cout << "Problem with data at position : [" << currRows + 1 << ";" << i + 1 << "]" << std::endl;
						//this->valuesOfCells.resize(this->cells.size());
						destroy();
						throw "Missing , !";
					}
				}
				
				buff.erase(0, j);

				if (buff != "\0")
					buff.erase(0, 1);

				if (data[0] != '"')
					removeWhiteSpaces(data);

				try {
					this->cells[currRows].push_back(Factory::getFactory().creatCell(data));
				}
				catch (std::exception& ex) {
					
					std::cout << "Problem with data at position : [" << currRows+1 << ";" << i+1 << "]" << std::endl;
					//this->valuesOfCells.resize(this->cells.size());
					destroy();
					throw;
				}
			}

		}

		currRows++;

	}

	if (!file.eof()) {
		destroy();
		throw "Something went wrong while loading the table";
	}

	file.close();

	FullValueOfCells();

}

Table::Table(const Table& other) {

	this->cells.resize(other.cells.size());
	this->valuesOfCells.resize(other.valuesOfCells.size());

	for (size_t i = 0; i < other.cells.size(); i++) {

		for (size_t j = 0; j < other.cells[i].size(); j++) {
			if (other.cells[i][j]) {
				this->cells[i].push_back(other.cells[i][j]->clone());
				this->valuesOfCells[i].push_back(other.valuesOfCells[i][j]);
			}
			else
				this->cells[i].push_back(nullptr);
		}

	}
}

Table& Table::operator=(const Table& other) {

	if (this != &other) {

		std::vector<std::vector<Cell*>> temp;
		std::vector<std::vector<std::string>> tempValue;

		temp.resize(other.cells.size());
		tempValue.resize(other.valuesOfCells.size());

		for (size_t i = 0; i < other.cells.size(); i++) {

			for (size_t j = 0; j < other.cells[i].size(); j++) {
				if (other.cells[i][j]) {
					temp[i].push_back(other.cells[i][j]->clone());
					tempValue[i].push_back(other.valuesOfCells[i][j]);
				}
				else
					temp[i].push_back(nullptr);
			}

		}

		destroy();
		this->cells = temp;
		this->valuesOfCells = tempValue;

	}

	return *this;
}


Table::~Table() {

	destroy();

}


void Table::print() const {

	for (size_t i = 0; i < this->valuesOfCells.size(); i++) {

		for (size_t j = 0; j < this->valuesOfCells[i].size(); j++) {

			size_t maxLenCol = lenOfLongestStringInTheCol(j);
			size_t addSpaces = maxLenCol - valuesOfCells[i][j].length();

			while (addSpaces) {
				std::cout << " ";
				addSpaces--;
			}
			std::cout << this->valuesOfCells[i][j];
			std::cout << " | ";
		}
		std::cout << std::endl;
	}

}

void Table::edit(Coordinates& pos,std::string& data) {

	size_t posOfSpace = data.find(' ');
	if (posOfSpace!=std::string::npos && data[0] != '"') {
		std::cout << "Invalid data! To many arguments!" << std::endl;
		throw "Attempt to execute an invalid Edit command!\n";
	}

	if (data[0] != '"')
		removeWhiteSpaces(data);
	
	Cell* temp;
	try {
		temp = Factory::getFactory().creatCell(data);
	}
	catch (std::exception& ex) { //Not found the type of cell
		std::cout << "The cell you wanted to edit can Not be edited! Invalid data!" << std::endl;
		throw "Attempt to execute an invalid Edit command!\n";
	}

	bool isResized = false;

	if (pos.row >this->cells.size() || pos.col >this->cells[0].size()) {

		Table tempTable(*this);
		Coordinates oldCol;
		oldCol.row = this->cells.size();
		oldCol.col = this->cells[0].size();
		if (pos.row > tempTable.cells.size()) {
			tempTable.cells.resize(pos.row);
			tempTable.valuesOfCells.resize(pos.row);
			for (size_t i = 0; i < pos.row; i++) {
				tempTable.cells[i].resize(oldCol.col);
				tempTable.valuesOfCells[i].resize(oldCol.col);
			}
			isResized = true;
		}

		if (pos.col>tempTable.cells[0].size() && pos.row<=tempTable.cells.size()) {
			for (size_t i = 0; i < tempTable.cells.size(); i++) {
				tempTable.cells[i].resize(pos.col);
				tempTable.valuesOfCells[i].resize(pos.col);
			}
			isResized = true;
		}
		
		if (isResized) { // New cells are empty
			for (size_t i = 0; i < tempTable.cells.size(); i++) {

				for (size_t j = 0; j < tempTable.cells[i].size(); j++) {

					if (!tempTable.cells[i][j]) {
						try {
							tempTable.cells[i][j] = Factory::getFactory().creatCell("");
							tempTable.valuesOfCells[i][j] = " ";
						}
						catch (...) {
							tempTable.destroy();
						}
					}

				}
			}
		}

		delete tempTable.cells[pos.row-1][pos.col-1];
		tempTable.cells[pos.row-1][pos.col-1] = temp;

		if (tempTable.cells[0][0]) {

			destroy();
			*this = tempTable;
			setFormulasCellNotCalculated(); // First set formulas not calculated so would be calculated again after edit
			FullValueOfCells();

		}

	}
	else {
		delete this->cells[pos.row - 1][pos.col - 1];
		this->cells[pos.row - 1][pos.col - 1] = temp;
		setFormulasCellNotCalculated(); // First set formulas not calculated so would be calculated again after edit
		FullValueOfCells();
	}
}


bool Table::save(std::ofstream& file) const {

	if (!file.good())
		return false;

	for (size_t i = 0; i < cells.size(); i++) {

		for (size_t j = 0; j < cells[i].size(); j++) {

			if (cells[i][j] != nullptr)
				if (!cells[i][j]->storeCell(file))
					return false;

			if (j != cells[i].size() - 1)
				file << ',';

		}
		if (i != cells.size() - 1)
			file << '\n';

	}

	return file.good();
}

size_t Table::lenOfLongestStringInTheCol(const size_t col) const {

	size_t maxLen = 0;
	size_t currLen = 0;

	for (size_t i = 0; i < valuesOfCells.size(); i++) {

		currLen = valuesOfCells[i][col].length();

		if (maxLen < currLen) {
			maxLen = currLen;
		}

	}

	return maxLen;
}

void Table::destroy() {

	for (size_t i = 0; i < this->cells.size(); i++) {
		for (int j = 0; j < this->cells[i].size(); j++) {
			delete this->cells[i][j];
		}
		this->valuesOfCells[i].clear();
	}

	this->cells.clear();
	this->valuesOfCells.clear();

}

void Table::FullValueOfCells() {

	if(valuesOfCells.size()!=cells.size()) // If it comes here after edit not to resize again
		valuesOfCells.resize(cells.size());

	for (size_t i = 0; i < cells.size(); i++) {

		if (valuesOfCells[i].size() != cells[i].size())
			valuesOfCells[i].resize(cells[i].size());

		for (size_t j = 0; j < cells[i].size(); j++) {//first full those that have already been calculated

			if (cells[i][j]->getIsCalculated())
			{

				valuesOfCells[i][j] = cells[i][j]->getData();

			}
		}
	}

	for (size_t i = 0; i < cells.size(); i++) {

		for (size_t j = 0; j < cells[i].size(); j++) {

			if (!cells[i][j]->getIsCalculated()) { // If it is foormula must be calculated

				Coordinates pos;
				pos.row = i;
				pos.col = j;

				calculateFormula(*cells[i][j], pos);

			}
		}
	}


}

bool Table::isNumberInString(const std::string& str) const {

	for (size_t i = 0; i < str.length(); i++) {
		if (!std::isdigit(str[i]) && str[i] != '.' && str[i] != '-' && str[i] != '+')
			return false;
	}

	return true;

}

std::string Table::calculateReferencesFormula(const Coordinates&comeFrom,const Coordinates& firstCell, const Coordinates& secondCell, const std::string& op) {

	std::string newFormula = "=";
	std::string data1;
	std::string data2;

	if (firstCell.row ==comeFrom.row && firstCell.col == comeFrom.col ||firstCell.row >= cells.size() || firstCell.col >= cells[firstCell.row].size()) {
		data1 = "0";
	}
	else {
		data1 = valuesOfCells[firstCell.row][firstCell.col];
	}

	if (secondCell.row == comeFrom.row && secondCell.col == comeFrom.col || secondCell.row >= cells.size() || secondCell.col >= cells[secondCell.row].size()) {
		data2 = "0";
	}
	else {
		 data2 = valuesOfCells[secondCell.row][secondCell.col];
	}
	
	bool isNumFirst = isNumberInString(data1);
	bool isNumSecond = isNumberInString(data2);

	if (!isNumFirst && !isNumSecond) { // if are not digits convert to 0 

		newFormula += "0";
		newFormula += op;
		newFormula += "0";

	}
	else if (!isNumFirst && isNumSecond) {

		newFormula += "0";
		newFormula += op;
		newFormula += data2;

	}
	else if (isNumFirst && !isNumSecond) {

		newFormula += data1;
		newFormula += op;
		newFormula += "0";

	}
	else {
		newFormula += data1;
		newFormula += op;
		newFormula += data2;
	}

	return newFormula; // string of formula which calculator will calculate

}

void Table::calculateFormula(Cell& formula, Coordinates& pos) {
	
	std::string newFormula;
	std::string result;
	std::string data = formula.getData();

	if (isSimpleFormula(data)) {

		newFormula = data;

	}
	else {
		Coordinates firstCell, secondCell;
		std::string op;
		splitReferencesFormula(data, firstCell, secondCell, op);
		
		if (firstCell.row < cells.size() && firstCell.col < cells[firstCell.row].size()
			&& secondCell.row < cells.size() && secondCell.col < cells[secondCell.row].size()) {

			if (!cells[firstCell.row][firstCell.col]->getIsCalculated() && firstCell.row != pos.row && firstCell.col != pos.col) {

				calculateFormula(*cells[firstCell.row][firstCell.col], firstCell);

			}
			else if (!cells[secondCell.row][secondCell.col]->getIsCalculated() && secondCell.row != pos.row && secondCell.col != pos.col) {

				calculateFormula(*cells[secondCell.row][secondCell.col], secondCell);

			}
		}
		newFormula = calculateReferencesFormula(pos,firstCell, secondCell, op);

	}

	result = Calculator::getCalculator().CalculateWithOnlyDigits(newFormula);// calculate
	this->valuesOfCells[pos.row][pos.col] = result; // store
	formula.setIsCalculated(true);

}

void Table::setFormulasCellNotCalculated() {

	for (size_t i = 0; i < cells.size(); i++) {

		for (size_t j = 0; j < cells[i].size(); j++) {

			if (cells[i][j]->getData()[0] == '=') {

				cells[i][j]->setIsCalculated(false);

			}
		}
	}

}


void Table::splitReferencesFormula(const std::string& str, Coordinates& coord1, Coordinates& coord2, std::string& op) {

	std::string r1, c1, r2, c2;
	bool flagOp = false;
	bool flagC = false;

	for (size_t i = 2; i < str.size(); i++) {

		if (Calculator::getCalculator().isOperation(str[i])) {
			op += str[i];
			flagOp = true;
			flagC = false;
		}
		else if (str[i] == 'C') {
			flagC = true;
		}
		else if (str[i] == 'R') {
			continue;
		}
		else if (!flagOp && !flagC) {
			r1 += str[i];
		}
		else if (!flagOp && flagC) {
			c1 += str[i];
		}
		else if (flagOp && !flagC) {
			r2 += str[i];
		}
		else if (flagOp && flagC) {
			c2 += str[i];
		}
	}

	coord1.row = stoi(r1) - 1; // -1 becouse matrix starts with 0 
	coord1.col = stoi(c1) - 1;
	coord2.row = stoi(r2) - 1;
	coord2.col = stoi(c2) - 1;

}

bool Table::isSimpleFormula(const std::string& str) {

	for (size_t i = 0; i < str.size(); i++) {
		if (str[i] == 'R' || str[i] == 'C' && str[i] < '0' && str[i]> '9' && str[i] != '.' && str[i] != '+' && str[i] != '-' && str[i] != '*' &&
			str[i] != '/' && str[i] != '=' && str[i] != '>' && str[i] != '<') {
			return false;
		}
	}

	return true;

}

size_t Table::getRows(std::ifstream& file) const {

	if (!file.good())
		return 0;

	size_t rows = 0;
	std::string buff;

	file.seekg(0, std::ios::beg);

	while (file.good()) {
		std::getline(file, buff, '\n');
		rows++;
	}

	if (!file.eof())
		return 0;

	return rows;
}

size_t Table::getColumns(std::ifstream& file) const {

	if (!file.good())
		return 0;

	size_t cols = 0;
	size_t maxCols = 0;
	std::string buff;

	file.seekg(0, std::ios::beg);

	while (file.good()) {
		std::getline(file, buff, '\n');
		for (size_t i = 0; buff[i] != '\0'; i++) {
			if (buff[i] == ',') {
				cols++;
			}
		}
		if (cols > maxCols) {
			maxCols = cols;
		}
		cols = 0;
	}

	if (!file.eof())
		return 0;

	return ++maxCols;

}

void Table::removeWhiteSpaces(std::string& str) {

	for (int i = 0; i < str.length(); i++) {
		if (str[i] == ' ') {
			str.erase(str.begin() + i);
			i--;
		}
	}

}
