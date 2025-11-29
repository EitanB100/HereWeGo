#pragma once

constexpr int MAX_X = 79;
constexpr int MAX_Y = 24;

constexpr char WALL = 'W';
constexpr char OBSTACLE = '*';
constexpr char KEY = 'K';

// Check if character represents a door - suggested by gemini
inline bool isDoor(char c) { 
	return (c == 'D'); 
	return (c >= '1' && c <= '9');
}