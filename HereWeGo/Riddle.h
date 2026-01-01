#pragma once
#include <string>
#include <vector>

class Riddle {
	int id = 0;
	std::string question;
	std::vector<std::string> options;
	int correctAnswer = 0;
};