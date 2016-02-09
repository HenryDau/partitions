/**
 * Contains the implementation of functions declared in command.h.
 */

#include "command.h"
#include <algorithm>
#include <iostream>
#include <iterator>

using namespace std;

/**
* I appologize if the formatting isn't great, this is my first time trying vim *
**/

bool partition_tokens(vector<string> tokens, vector<command_t>& commands) {
	size_t pos;
	command_t* currentCommand = new command_t(); 		//defauls to STDIN and STDOUT
	for (pos = 0; pos < tokens.size(); pos++){
		if (tokens[pos] == "<"){																		/********* INPUT *********/
			//Look for infile
			if (pos == 0 || pos == (tokens.size() - 1)){
				cerr << "Invalid infile format" << endl;
				return false;
			}
		 if (tokens[pos + 1] == "|" || tokens[pos+1] == ">>" || tokens[pos+1] == ">" || tokens[pos+1] == "<"){
        cerr << "Invalid file name" << endl;
        return false;
      }
			if (currentCommand->input_type != READ_FROM_STDIN){
        cerr << "Reading from multiple destinations" << endl;
        return false;
      }
			currentCommand->infile = tokens[pos+1];
			currentCommand->input_type = READ_FROM_FILE;
			pos++;
		} else if (tokens[pos] == ">" || tokens[pos] == ">>"){			/********* OUTPUT *********/
			//Deal with writing to a file
			if (pos == (tokens.size() - 1)){
				cerr << "Invalid outfile format" << endl;
				return false;
			}
			if (currentCommand->output_type != WRITE_TO_STDOUT){
        cerr << "Writing to multiple destinations" << endl;
        return false;
      }
			if (tokens[pos + 1] == "|" || tokens[pos+1] == ">>" || tokens[pos+1] == ">" || tokens[pos+1] == "<"){
				cerr << "Invalid file name" << endl;
				return false;
			}
      currentCommand->outfile = tokens[pos+1];
			if (tokens[pos] == ">")
      	currentCommand->output_type = WRITE_TO_FILE;
			else
				currentCommand->output_type = APPEND_TO_FILE;	
			pos++;	
		} else if (tokens[pos] == "|"){															/********* PIPE *********/
			// Deal with a pipe
			if (currentCommand->argv.size() == 0){
				cerr << "Can not pipe without commands" << endl;
				return false;
			}
			if (currentCommand->output_type != WRITE_TO_STDOUT){
				cerr << "Writing to multiple destinations" << endl;
				return false;
			}
			currentCommand->output_type = WRITE_TO_PIPE;
			commands.push_back(*currentCommand);
			currentCommand = new command_t();
			currentCommand->input_type = READ_FROM_PIPE;
		} else {																										 /********* DEFAULT *********/
			currentCommand->argv.push_back(tokens[pos]);	// default add current token to commands
		}
	}
	if (currentCommand->argv.empty()){
    cerr << "Can not have a pipe without commands" << endl;
    return false;
  }
 	commands.push_back(*currentCommand);
	delete currentCommand;

	return true;
}

// Feel free to ignore everything below here. It's just code to allow you to
// cout a command in a nice, pretty format. =)


const char* input_types[] = {
  "READ_FROM_STDIN",
  "READ_FROM_FILE",
  "READ_FROM_PIPE"
};


const char* output_types[] = {
  "WRITE_TO_STDOUT",
  "WRITE_TO_PIPE",
  "WRITE_TO_FILE",
  "APPEND_TO_FILE"
};


ostream& operator <<(ostream& out, const command_t& cmd) {
  copy(cmd.argv.begin(), cmd.argv.end(), ostream_iterator<string>(out, " "));

  out << "\n    input:   " << input_types[cmd.input_type]
      << "\n    output:  " << output_types[cmd.output_type]
      << "\n    infile:  " << cmd.infile
      << "\n    outfile: " << cmd.outfile;

  return out;
}
