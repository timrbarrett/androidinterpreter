#ifndef TRB_INTERPRETER_H
#define TRB_INTERPRETER_H

/*
	Include any needed files
*/
#include <Arduino.h>
#include <String.h>
//#include <TRB_Fragment.h>

/*
	Define the class
*/
class TRB_Interpreter {
	
	public:
		// constructors
		TRB_Interpreter();
		
		// public methods
		void addCommand(String commands);
		bool processNextCommand();
		bool done();
		int getVariable(int index);
		String output();
		int getNumberOfVariables();
		
	private:

		// private variables
		String _commands ="";
		int _nextCommandToExecute;
		int _variables[5];
		String _errorMessage = "";
};

#endif

/*
	a			[a-z]
	0			[0-9]
	#			[00-FF]
	.			element can be repeated
	( 			start a new command
	?			['v' or ]
	
	va0			set variable [a-z] to ['t'|'f']
	)			stop a command / 
				execute the command / 
				delete from command list
				
	Future?
	d#(a.)(?.)	define a method, with parameters
	(i(c)(v)).	if condition then
	a<b			returns true if a<b
	a>b
*/