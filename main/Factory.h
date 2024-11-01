#pragma once
#include <string>

#include "Cell.h"
#include "IntCell.h"
#include "DoubleCell.h"
#include "StringCell.h"
#include "Formula.h"
#include "EmptyCell.h"

enum CellType {

	UNKNOWN = -1,

	INT = 0,
	DOUBLE = 1,
	STRING = 2,
	FORMULA = 3,
	EMPTY = 4

};

class Factory // Singleton
{
public:
	static Factory& getFactory();
	Cell* creatCell(const std::string) const;
private:
	Factory() = default; 
	~Factory() = default;
	Factory(const Factory&) = delete;
	Factory& operator=(const Factory&) = delete;

	bool isIntCell(const std::string&) const;
	bool isDoubleCell(const std::string&) const;
	bool isStringCell(const std::string&) const;
	bool isFormula(const std::string&) const;
	bool isEmptyCell(const std::string&) const;

	CellType getTypeOfCell(const std::string&) const;//CellType getTypeOfCell(const std::string&) const;

	Cell* createIntCell(const std::string&) const;
	Cell* createDoubleCell(const std::string&) const;
	Cell* createStringCell(const std::string&) const;
	Cell* createFormulaCell(const std::string&) const;
	Cell* createEmptyCell() const;
	
};

