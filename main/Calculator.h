#pragma once
#include "Table.h"
#include <string>

class Calculator { //Singleton
public:
	friend class Table; // to use function isOperation

	static Calculator& getCalculator();
	std::string CalculateWithOnlyDigits(const std::string&); //Calculated only basic formulas without coordinates
private:
	Calculator() = default;
	~Calculator() = default;
	Calculator(const Calculator&) = delete;
	Calculator& operator= (const Calculator&) = delete;

	void splitFormula(const std::string&, double&, double&, std::string&);
	std::string removeZeros(std::string&);
	bool isOperation(const char c) const;
};

