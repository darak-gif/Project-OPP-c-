#pragma once
#include <vector>
#include "Factory.h"
#include "Calculator.h"

struct Coordinates {

	size_t row;
	size_t col;

};

class Table
{
public:
	Table(const std::string);
	Table(const Table&);
	Table& operator=(const Table&);
	~Table();

	void print() const;
	void edit(Coordinates&,std::string&);
	bool save(std::ofstream&) const;

private:
	size_t getRows(std::ifstream&) const;
	size_t getColumns(std::ifstream&) const;
	bool isNumberInString(const std::string&) const;
	void removeWhiteSpaces(std::string&);
	size_t lenOfLongestStringInTheCol(const size_t) const; // it is used for print function
	void destroy();

	void FullValueOfCells(); // The way valuesOfCells is validate
	bool isSimpleFormula(const std::string&); // To decide if to use calculator
	void splitReferencesFormula(const std::string&, Coordinates&, Coordinates&, std::string&);
	void calculateFormula(Cell&, Coordinates&);
	void setFormulasCellNotCalculated(); // All formula Cells will have isCalculated = false for edit function 

	std::string calculateReferencesFormula(const Coordinates&,const Coordinates&, const Coordinates&, const std::string&);

	std::vector<std::vector<Cell*>> cells; // actual data  
	std::vector<std::vector<std::string>> valuesOfCells; // store all data of the table 
														//with calculated formulas

};

