#include "CommandLine.h"
#include <fstream>
#include <iostream>

CommandLine& CommandLine::getCommandLine() {

	static CommandLine line;
	return line;

}

void CommandLine::start(std::istream& stream) { //the whole program

	printInfo();

	while (!this->mustEnd) {
		
		this->input.clear();
		std::getline(stream, this->input, '\n');

		try { // here catch all throws 
			execute(stream);
		}
		catch (const std::exception& ex) {
			std::cout << ex.what() << std::endl;
			exit(stream); //exit set mustEnd = true
		}
		catch (const char* ex) {
			std::cout << ex;
			if(ex!= "You must open a table first!\n" && ex!= "Attempt to execute an invalid Edit command!\n")
				exit(stream);
			else if(ex == "You must open a table first!\n") {
				std::cout << "Enter new command: " << std::endl;
			}
			else {
				std::cout << "Try again to edit: " << std::endl;
			}
		}
		catch (...) {
			std::cout << "Somenthing was catched" << std::endl;
			exit(stream);
		}
		std::cout << std::endl;
	}

}

void CommandLine::open(const std::string& file) { 

	if (this->table) { // can not work with two opened table

		throw "You already have opened table! First must close this one!\n";
	}

	this->table = new Table(file); 
	this->file = file;
	std::cout << "Successfully opened " << this->file << std::endl;

}

void CommandLine::save() const {

	if(!this->table)
		throw "You must open a table first!\n";

	if (hasBeenChanged) {
		
		std::ofstream stream(this->file);
		bool flag = this->table->save(stream);
		stream.clear();
		stream.close();
		
		if (!flag)
			throw "Unsuccessfully saved file";

	}
	std::cout << "Successfully saved " << this->file << std::endl;
	
}

void  CommandLine::saveAs(const std::string& file) const{

	if (!this->table)
		throw "You must open a table first!\n";

	std::ofstream stream(file);
	bool flag = this->table->save(stream);
	stream.clear();
	stream.close();
	if(!flag)
		throw "Unsuccessfully saved another file";
	
	std::cout << "Successfully saved another " << file << std::endl;
}

void CommandLine::print() const{

	if (!this->table)
		throw "You must open a table first!\n";

	this->table->print();

}

void CommandLine::edit(std::string& line) {

	if (!this->table)
		throw "You must open a table first!\n";

	std::string firstPos, secondPos;
	size_t pos = line.find(' ');
	if (pos <= 0) {
		std::cout << "Incorrect coordinates!" << std::endl;
		throw "Attempt to execute an invalid Edit command!\n";
	}
		 
	firstPos = line.substr(0, pos);

	line.erase(0, pos + 1);
	pos = line.find(' ');
	if (pos <= 0) {

		std::cout << "Incorrect coordinates!" << std::endl;
		throw "Attempt to execute an invalid Edit command!\n";
	}
	secondPos = line.substr(0, pos);

	Coordinates coor;
	try {
		coor.row = std::stoul(firstPos);
		coor.col = std::stoul(secondPos);
	}
	catch (std::exception& ex) {
		std::cout << "Incorrect coordinates!" << std::endl;
		throw "Attempt to execute an invalid Edit command!\n";
	}

	if (line == secondPos) {
		line.clear();
	}
	else {
		line.erase(0, pos + 1);
	}

	this->table->edit(coor, line); // can throw
	this->hasBeenChanged = true;

}

void CommandLine::exit(std::istream& stream) {

	if (!this->table) { //If there is no opened table just close the app
		this->mustEnd = true;
		return;
	}


	if (this->hasBeenChanged) {
		char answer;
		std::cout << "Do you want to save changes?(y/n)" << std::endl;;
		do {
			stream >> answer;
			if (answer == 'y') {
				save();
			}
			if (answer != 'n' && answer != 'y') {
				std::cout << "Please answer with y or n! Where y is for YES and n is for NO" << std::endl;;
			}
		} while (answer != 'n' && answer != 'y');
		stream.ignore(100, '\n');
	}
	this->mustEnd = true;

}

Commands CommandLine::findCommand(std::string& command) {

	toLowerCases(command); // so user can write OpEN or open or Open 

	if (command == "open")
		return Commands::OPEN;
	else if (command == "save")
		return Commands::SAVE;
	else if (command == "saveas")
		return Commands::SAVE_AS;
	else if (command == "print")
		return Commands::PRINT;
	else if (command == "edit")
		return Commands::EDIT;
	else if (command == "exit")
		return Commands::EXIT;

	return Commands::UNKNOWN;

}

void CommandLine::execute(std::istream& stream) { // this can throw

	std::cout << "- - - - - - - - - - - - - - - - - - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - " << std::endl; // tova ako go mahna problem s std::cin.ignor()
	size_t pos = input.find(' ');
	std::string command;

	if (pos > 0)
		command = input.substr(0, pos);
	else
		command = input;

	Commands comm = findCommand(command);

	if (comm == Commands::UNKNOWN) {

		std::cout<< "Attempt to execute an invalid command!" << std::endl;
		std::cout<< "Enter again: " << std::endl;

	}else if (comm == Commands::OPEN) {
		input.erase(0, pos + 1);// remove the command and the space 
		open(input);
	}
	else if (comm == Commands::SAVE) {
		save();
	}
	else if (comm == Commands::SAVE_AS) {
		input.erase(0, pos + 1);//remove the command and the space 
		saveAs(input);
	}
	else if (comm == Commands::PRINT) {
		print();
	}
	else if (comm == Commands::EDIT) {
		input.erase(0, pos + 1);//// remove the command and the space 
		edit(input);
	}
	else {
		exit(stream);
	}

}

void CommandLine::printInfo()const {
	
	std::cout << "    ---Application for working with spreadsheets---\n";
	std::cout << "To open and load table, please enter \"open <path to the file> \"" << std::endl;
	std::cout << "To print the table, please enter \"print\"" << std::endl;
	std::cout << "To save the table, please enter \"save\"" << std::endl;
	std::cout << "To save the table to a new file, please enter \" saveas <path to the new file> \"" << std::endl;
	std::cout << "To edit cell from the table, please enter \" edit <row col data> \"" << std::endl;
	std::cout << "If you want to close the app, please enter \" exit \"\n" << std::endl;

}

void CommandLine::toLowerCases(std::string& str) {

	for (size_t i = 0; i < str.length(); i++) {

		str[i] = std::tolower(str[i]);
	}

}