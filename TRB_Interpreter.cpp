/*
	Author: Tim R Barrett
	Date: 26th Feb 2017
	
	An unoptimised minimal interpreter 
		that can read from BLE UART
		and take appropriate action
		
*/

#ifndef TRB_INTERPRETER_CPP
#define TRB_INTERPRETER_CPP

/*
	Include any needed files
*/
#include <String.h>
#include <Arduino.h>
#include <TRB_Interpreter.h>
//#include <Fragment.h>

/*
	Define the class
*/
	TRB_Interpreter::TRB_Interpreter() {
		_nextCommandToExecute = 0;
		
	}
	
	void TRB_Interpreter::addCommand(String commands) {
		_commands.concat(commands);
	}
	
	
	
	bool TRB_Interpreter::done() {
		
		bool done = ( _nextCommandToExecute > (int)_commands.length());
		return done;
		
	}
	
	bool TRB_Interpreter::processNextCommand() {
		if (done()) {
			return false;
		} else {
			
			// big switch statement
			switch (_commands[_nextCommandToExecute]) {
				// set variable to value immediately following
				case '(': 
				{
					++_nextCommandToExecute; // consume the '('
					break;
				}
				
				case ')': 
				{					++_nextCommandToExecute; // consume the ')'
					break;
				}
					
				case 'v':
				{
					_nextCommandToExecute++; // consume the 'v'
					
					int asciiVariableIndex = _commands[_nextCommandToExecute++]; //consume the 'v'
					int variableIndex= asciiVariableIndex - 97; //'a' = 0
					int asciiValue = _commands[_nextCommandToExecute++];
					int value = asciiValue - 48; // '0' = 0
					
					/* 
						check for variableIndex being out of bounds
					*/
					if (variableIndex <0 || variableIndex > getNumberOfVariables()) {
						_errorMessage.concat(F("E:variable bounds  "));
						Serial.println(_errorMessage);
					}
					_variables[variableIndex]=value;
					Serial.print(F("**** set variable: "));
					Serial.print(variableIndex);
					Serial.print(F(" to: "));
					Serial.println(value);
					
					/*  this is be horrible for memory, 
						but works if memory isn't at a premium.
					*/
					_commands=_commands.substring(_nextCommandToExecute);
					break;
				}
				break;
			}
			return true;	
		}	
	}

	/* will return a string with exactly the characters sent back over BLE */

	String TRB_Interpreter::output() {
		
		String interpreterOutput ="Commands:           ";
		
		int repeatLines;
		if (_commands.length()==0) {
			repeatLines = 0;
		} else {
			repeatLines = (_commands.length()) / 5;
		}
		
		int CHARACTERS_PER_LINE = 5;
		
		for ( int currentLine = 0; 
			currentLine < repeatLines+1 ; 
			++currentLine ) {
	
			String characters = "";
			
			for ( int index = (currentLine) * CHARACTERS_PER_LINE; 
				index < ((currentLine+1) * CHARACTERS_PER_LINE);
				++index)  {
					
					if (index <= _commands.length() ) {
						
						// put a star by the nextCommandToExecute
						if (_nextCommandToExecute==index) {
							interpreterOutput.concat("*"); 
							//Serial.print("*");
						} else {
							interpreterOutput.concat(" ");
							//Serial.print(" ");
						}
			 
						char buf[3];
						sprintf(buf, "%02x", _commands[index]);
						interpreterOutput.concat(buf);
	
						//Serial.println(buf);
			
						char temp = _commands.charAt(index);
						characters.concat(temp);
				
					} else { // end keep within defined bytes 
						interpreterOutput.concat("    ");// space out the blanks
					}
					
			} // end index loop
			interpreterOutput.concat(characters);
		} // end currentLine loop
		
		/*
		-------------- variables ---------------
		*/

		interpreterOutput.concat("Variables:          ");
		
		for ( int index2 = 0; index2 < getNumberOfVariables() ; ++index2 ) {
			
			if (true) {
				Serial.print(F("numberOfVariables"));
				Serial.print(getNumberOfVariables());
			}
			
			interpreterOutput.concat(" ");
			char buf2[3];
			sprintf(buf2, "%02x", _variables[index2]);
			interpreterOutput.concat(buf2);
			
		}

		return interpreterOutput;
		 
	}
	
	int TRB_Interpreter::getVariable(int index) {
		
		return _variables[index];
		
	}
	int TRB_Interpreter::getNumberOfVariables() {
		
		return 5;
		
	}


	
	
	


#endif