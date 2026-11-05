#pragma once
#include <iostream>
#include <windows.h>
#include <string>
#include <vector>

class ConsoleRender
{
	HANDLE buffers[2];
	int currentBufferIndex;
	size_t width;
	size_t height;

public:
	ConsoleRender(int width, int height);
	~ConsoleRender();

	int CurrentBufferIndex() const { return currentBufferIndex; }
	void CurrentBufferIndex(const int index) { this->currentBufferIndex=index; }

	size_t Width() const { return this->width; }
	void Width(const size_t newWidth) { this->width = newWidth; }
	
	size_t Height() const { return this->height; }
	void Height(const size_t newHeight) { this->height = newHeight; }

	void DrawText(const int x, const int y, const std::string& text, const WORD attributes = 0x07);
	void DrawArt(const int x, const int y, const std::vector<std::string>& art, 
		const WORD attributes = 0x07, const bool transparent = false);
	void Present();
	void Clear();
};
