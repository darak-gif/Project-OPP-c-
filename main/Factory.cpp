#include "Factory.h"

Factory& Factory::getFactory() {

	static Factory factory;
	return factory;

}


Cell* Factory::creatCell(const std::string data) const {

	switch (getTypeOfCell(data))
	{
	case CellType::INT: 
		return createIntCell(data);
	case CellType ::DOUBLE :
		return createDoubleCell(data);
	case CellType::STRING :
		return createStringCell(data);
	case CellType ::FORMULA :
		return createFormulaCell(data);
	case CellType::EMPTY:
		return createEmptyCell();
	case CellType::UNKNOWN :
		throw std::invalid_argument("Invalid data input!");
	}

}

bool  Factory::isIntCell(const std::string& str) const {

	if (str == "" || str[0]=='=' || str[0]=='\"')
		return false;

    for (std::size_t i = 0; str[i] != '\0'; i++) {
		if (str[i] != '+' && str[i] != '-' && str[i]!='.' && !(str[i] >= '0' && str[i] <= '9')) {
			throw std::invalid_argument("In integer number you can NOT put letters or symbols!");
		}
        if (str[i] == '.') {
			return false;
        }
		
    }

    return true;

}

bool Factory::isDoubleCell(const std::string& str) const {

	if (str == "" || str[0] == '=' || str[0] == '\"')
		return false;

	bool hasDot = false;

	for (std::size_t i = 0; str[i] != '\0'; i++) {

		if (str[i] != '+' && str[i] != '-' && str[i] != '.' && !(str[i] >= '0' && str[i] <= '9')) {

			throw std::invalid_argument("In double number you can NOT put letters or symbols!");

		}else if (str[i] == '.' && hasDot) {

			throw std::invalid_argument("That double digit has more than one point in itself!");

		}
		else if (str[i] == '.') {
			hasDot = true;
		}

	}

	return hasDot;

}

bool Factory::isStringCell(const std::string& str) const {

	if (str != "\0" && str[0] == '"' && str[str.length() - 1] != '"')
		throw std::invalid_argument("That string must have \" in the end!");

	if (str != "\0" && str[0] != '"' && str[str.length() - 1] == '"')
		throw std::invalid_argument("That string must have \" at the beginnig!");

	bool has = false;
	bool hasMatched = true;
	for (size_t i = 0; i < str.length(); i++) {
		if (str[i] == '"' && has) {
			hasMatched = true;
			has = false;
		}
		else if (str[i] == '"') {
			has = true;
			hasMatched = false;
		}

	}

	if (!hasMatched)
		throw std::invalid_argument("That string must have  matching \\\"");

	if (str == "\0" || str[0] != '"' || str[str.length() - 1] != '"') {
		return false;
	}

	return true;

}

bool isOperation(const char c) {

	return  c == '+' || c == '-' || c == '*' ||
		c == '/' || c == '=' || c == '>' || c == '<' || c == '!';

}

bool Factory::isFormula(const std::string& str) const {

	if (str == "\0" || str[0] != '=') {

		return false;

	}

	std::string coordinates;
	std::string operation;

	for (size_t i = 0; i < str.length(); i++) {
		
		if (isOperation(str[i]) && i>0) {
			operation += str[i];
			
		}

		if (str[i] == 'R' || str[i] == 'C') {
			coordinates += str[i];
		}
		
	}
	
	if(coordinates!="RCRC" && coordinates!="")
		throw std::invalid_argument("Invalid coordinates in formula! Coordinates must be R for row and C for column");

	if (operation == "\0" || operation.length() > 5)
		throw std::invalid_argument("Invalid operation in formula!");

	for (std::size_t i = 1; str[i] != '\0'; i++) {

		if (str[i] != '=' && str[i] != '<' && str[i] != '>' && str[i] != '+'
			&& str[i] != '-' && str[i] != '*' && str[i] != '/'
			&& str[i] != 'R' && str[i] != 'C' && str[i]!='.' && (str[i] < '0' || str[i]>'9')) {
			return false;
		}
	}

	return true;
}

bool Factory::isEmptyCell(const std::string& str) const {

	return str == "";

}

CellType Factory::getTypeOfCell(const std::string& data) const {

	if (isIntCell(data))
		return CellType::INT;

	else if (isDoubleCell(data))
		return CellType::DOUBLE;

	else if (isStringCell(data))
		return CellType::STRING;

	else if (isFormula(data))
		return CellType::FORMULA;

	else if (isEmptyCell(data))
		return CellType::EMPTY;

	return CellType::UNKNOWN;

}

Cell* Factory::createIntCell(const std::string& data) const {

	int store = std::stoi(data);
	return new IntCell(store);

}

Cell* Factory::createDoubleCell(const std::string& data) const {

	double store = std::stod(data);
	return new DoubleCell(store);

}

Cell* Factory::createStringCell(const std::string& data) const {

	return new StringCell(data);

}

Cell* Factory::createFormulaCell(const std::string& data) const {

	return new Formula(data);

}

Cell* Factory::createEmptyCell() const {

	return new EmptyCell();

}
