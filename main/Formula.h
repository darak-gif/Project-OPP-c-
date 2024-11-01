#pragma once
#include <string>
#include "Cell.h"

class Formula : public Cell
{
public:
	Formula() = default;
	Formula(const std::string&);
	virtual Cell* clone() const override;
	virtual bool storeCell(std::ofstream&) const override;
	virtual void print()const override;
	virtual std::string getData() const override;

private:
	std::string data;
};

