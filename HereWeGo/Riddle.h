#pragma once
#include <string>
#include <vector>

struct Riddle {
	int id = 0;
	std::string question;
	std::vector<std::string> options;
	int correctAnswer = 0;
};