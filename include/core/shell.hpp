#pragma once

#include "command.hpp"
#include "history.hpp"
#include <string>
#include <mutex>
#include <thread>

class Shell {
public:
		/**
			* Main Shell class - contains all abstracted functions to run and use Blush
		 */
		void Init();

		/**	
			* Run the shell
			* @returns `void`
		 */
		int run();
		
		/**
			* Shell cleanup and pre-exit state
		 */
		~Shell();

		static History* getHistory();
private:
	std::string fullWorkingPath;
	std::mutex commandExecution;
	std::jthread thread;
	History history;
	static History* currentHistory;
};