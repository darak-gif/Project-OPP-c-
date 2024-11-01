#pragma once
#include<fstream>

class Cell
{
public:
	Cell() { }
	virtual ~Cell() { }
	virtual bool storeCell(std::ofstream&) const = 0; // Pure virtual functions
	virtual Cell* clone() const = 0;
	virtual std::string getData() const = 0;
	virtual void print()const = 0;

	bool getIsCalculated() const;
	void setIsCalculated(bool);

protected:
	bool isCalculated = true; // Flag indicating whether the cell is calculated
};

