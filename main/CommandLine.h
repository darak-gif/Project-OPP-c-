#pragma once
#include "Table.h"

enum  class Commands {

	UNKNOWN = -1,
	OPEN = 0,
	SAVE = 1,
	SAVE_AS = 2,
	PRINT = 3,
	EDIT = 4,
	EXIT = 5

};


class CommandLine { //Singleton

public:
	static CommandLine& getCommandLine();
	void start(std::istream&); //the whole app
	

private:
	CommandLine() = default;
	~CommandLine() { delete table; } // Delete the pointer
	CommandLine(const CommandLine&) = delete;
	CommandLine& operator=(const CommandLine&) = delete;

	void open(const std::string&); // All of these commands use the corresponding table function
	void save() const;
	void saveAs(const std::string&)const;
	void print() const;
	void edit(std::string&);
	void exit(std::istream&);
	Commands findCommand(std::string&);
	void execute(std::istream&);
	void printInfo()const;

	void toLowerCases(std::string&);

	Table* table = nullptr; // Pointer to the table
	std::string input; //user input about which command to be executed
	std::string file; // name of the file from that is load the table
	bool hasBeenChanged = false; // For exit if we have to ask for save
	bool mustEnd = false; // To stop 'while' in start
};

