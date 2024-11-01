#pragma once
#include "Cell.h"

class IntCell : public Cell
{
public:
	IntCell() = default;
	IntCell(int);
	virtual Cell* clone() const override;
	virtual bool storeCell(std::ofstream&) const override;
	virtual void print()const override;
	virtual std::string getData() const override;

private:
	int data;
};

