#include "ConsoleRender.h"

ConsoleRender::ConsoleRender(int width, int height) : width(width), height(height)
{
	this->buffers[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	this->buffers[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

	COORD coord{ (SHORT)width, (SHORT)height };
	SMALL_RECT rect{ 0, 0, (SHORT)(width - 1), (SHORT)(height - 1) };

	for(int i{}; i < 2; ++i)
	{
		SetConsoleScreenBufferSize(this->buffers[i], coord);
		SetConsoleWindowInfo(this->buffers[i], TRUE, &rect);

		CONSOLE_CURSOR_INFO cursorInfo;
		GetConsoleCursorInfo(this->buffers[i], &cursorInfo);
		cursorInfo.bVisible = FALSE;
		SetConsoleCursorInfo(this->buffers[i], &cursorInfo);
	}

	SetConsoleActiveScreenBuffer(this->buffers[0]);
	this->currentBufferIndex = 0;
}

ConsoleRender::~ConsoleRender()
{
	CloseHandle(this->buffers[0]);
	CloseHandle(this->buffers[1]);
}

void ConsoleRender::DrawText(const int x, const int y, const std::string& text, const WORD attributes)
{
	if (x < 0 || x >= Width() || y < 0 || y >= Height()) return;

	HANDLE backBuffer = this->buffers[1 - CurrentBufferIndex()];
	COORD coord = { (SHORT)x, (SHORT)y };
	DWORD written;

	WriteConsoleOutputCharacterA(backBuffer, text.c_str(), text.length(), coord, &written);
	FillConsoleOutputAttribute(backBuffer, attributes, text.length(), coord, &written);
}

void ConsoleRender::DrawArt(const int x, const int y, const std::vector<std::string>& art, const WORD attributes, const bool transparent)
{
    for (size_t i = 0; i < art.size(); ++i) {
        int currentY = y + static_cast<int>(i);

        if (currentY >= Height()) break;
        if (currentY < 0) continue;

        const std::string& line = art[i];

        if (!transparent) {
            DrawText(x, currentY, line, attributes);
        }
        else {
            int currentX = x;
            std::string textBuffer = "";
            int startX = currentX;

            for (char ch : line) {
                if (ch != ' ') {
                    if (textBuffer.empty()) {
                        startX = currentX;
                    }
                    textBuffer += ch;
                }
                else {
                    if (!textBuffer.empty()) {
                        DrawText(startX, currentY, textBuffer, attributes);
                        textBuffer.clear();
                    }
                }
                currentX++;
            }
            if (!textBuffer.empty()) {
                DrawText(startX, currentY, textBuffer, attributes);
            }
        }
    }
}

void ConsoleRender::Present() 
{
	CurrentBufferIndex(1 - CurrentBufferIndex());
	SetConsoleActiveScreenBuffer(this->buffers[CurrentBufferIndex()]);
}

void ConsoleRender::Clear()
{
	HANDLE backBuffer = this->buffers[1 - CurrentBufferIndex()];
	COORD coord = { 0, 0 };
	DWORD written;

	FillConsoleOutputCharacter(backBuffer, ' ', Width() * Height(), coord, &written);
	FillConsoleOutputAttribute(backBuffer, 0x07, Width() * Height(), coord, &written);
}
