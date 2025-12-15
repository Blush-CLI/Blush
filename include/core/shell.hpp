#pragma once

#include "command.hpp"
#include <string>
#include <mutex>
#include <thread>

class Shell {
public:
		/**
			* Main Shell class - contains all abstracted functions to run and use Blush
		 */
		Shell();

		/**	
			* Run the shell
			* @returns `void`
		 */
		void run();
		
		/**
			* Shell cleanup and pre-exit state
		 */
		~Shell();
private:
	void createRunnerThread();
	std::string fullWorkingPath;
	std::mutex commandExecution;
	std::jthread thread;
	// we'll put some state here later
};