#pragma once
#include<string>
#include"Cell.h"

class StringCell : public Cell
{
public: 
	StringCell() = default;
	StringCell(const std::string&);
	virtual Cell* clone() const override;
	virtual bool storeCell(std::ofstream&) const override;
	virtual void print()const override;
	virtual std::string getData() const override; //return clear not data

private:
	std::string data;
	std::string clear; // store the pure string without \ and " 
};

