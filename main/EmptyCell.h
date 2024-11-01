#pragma once
#include "Cell.h"

class EmptyCell : public Cell
{
public:
	EmptyCell() = default;
	virtual Cell* clone() const override;
	virtual bool storeCell(std::ofstream&) const override;
	virtual void print()const override;
	virtual std::string getData() const override;
};

