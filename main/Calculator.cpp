#include "Calculator.h"

Calculator& Calculator::getCalculator() {

	static Calculator calculator;
	return calculator;

}

void Calculator::splitFormula(const std::string& str, double& first, double& second, std::string& op) {

	std::string firstNum, secondNum;

	size_t i = 1;
	if (str.length() >=2 && str[1] == '+' || str[1] == '-') {
		firstNum += str[1];
		i = 2;
	}
	bool flag = false;

	for (i; i < str.size(); i++) {
		if (!firstNum.empty() && isOperation(str[i]) && op.length() < 1 || op.length() == 1 && str[i] == '=') {
			op += str[i];
			flag = true;
		}
		else if (!flag) {
			firstNum += str[i];
		}
		else {
			secondNum += str[i];
		}
	}

	first = std::stod(firstNum);
	second = std::stod(secondNum);

}


std::string Calculator::CalculateWithOnlyDigits(const std::string& str) {

	double result = 0;
	double first, second;
	std::string op;

	splitFormula(str, first, second, op);
	if (op == "+") {
		result = first + second;
	}
	else if (op == "-")
		result = first - second;
	else if (op == "*")
		result = first * second;
	else if (op == "/") {
		if (second == 0)
			return "ERROR";
		else
			result = first / second;
	}
	else if (op == "<")
		result = first < second;
	else if (op == ">")
		result = first > second;
	else if (op == "<=")
		result = first <= second;
	else if (op == ">=")
		result = first >= second;
	else if (op == "!=")
		result = first != second;
	else if (op == "==")
		result = first == second;

	std::string res = std::to_string(result);

	return removeZeros(res);

}


std::string  Calculator::removeZeros(std::string& str) {

	for (size_t i = str.length() - 1; str[i] == '0' && i >= 0; i--) {
		str.pop_back();
	}

	if (str.back() == '.')
		str.pop_back();

	return str;
}

bool Calculator::isOperation(const char c) const {

	return  c == '+' || c == '-' || c == '*' ||
		c == '/' || c == '=' || c == '>' || c == '<' || c == '!';

}