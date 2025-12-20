#pragma once

#include <string>
#include <string_view>

/**
* File based operations
*/
namespace File {
	/**
	* Reads the provided file
	* @param `std::string` filename
	* @returns `std::string`
	* @author x4raynixx
	*/
	std::string readFile(const std::string_view filename);
	
	/**
	* Get last current working directory entry (eg, it will get Projects from /home/sky/Projects)
	* @returns `std::string`
	* @author landingpad75
	*/
	std::string lcwd();
	
	/**
	* Get environment variable
	* @param `const char*` name
	* @returns `std::string`
	* @author landingpad75
	*/
	std::string getenv(const char* name);
	
	/**
	* Get home directory path
	* @returns `std::string`
	* @author landingpad75
	*/
	std::string getHome();
} // namespace File
