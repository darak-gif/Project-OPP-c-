#pragma once
#include "Cell.h"

class DoubleCell : public Cell
{
public: 
	DoubleCell() = default;
	DoubleCell(double);
	virtual Cell* clone() const override;
	virtual bool storeCell(std::ofstream&) const override;
	virtual void print()const override;
	virtual std::string getData() const override;
private:
	double data;
};

