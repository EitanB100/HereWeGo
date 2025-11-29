#pragma once

constexpr char WALL_TILE = 'W';
constexpr char OBSTACLE_TILE = '*';
constexpr char KEY_TILE = 'K';

// Check if character represents a door - suggested by gemini
inline bool isDoor(char c) { 
	
	return (c >= '1' && c <= '9');
}