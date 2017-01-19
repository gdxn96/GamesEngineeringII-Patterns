// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <iostream>

#include "Command.h"
#include "CommandGroup.h"
#include "History.h"
#include <assert.h>
#include <chrono>
#include <thread>
#include <Windows.h>
#include <Tchar.h>

class TestDocument 
{
public:
	std::string text;

	void print() const 
	{
		system("cls");
		std::cout << text;
	}
};


class AppendCommand : public Command
{
private:
	std::string s;
	TestDocument & document;

public:
	AppendCommand(TestDocument & doc, std::string const & s) : s(s), document(doc)
	{
	}

	void execute() 
	{
		document.text += s;
	}

	void undo() 
	{
		auto len = document.text.length() - s.length();
		document.text.erase(len);
	}
};



int main() 
{
	TestDocument document;
	document.text = "";

	History history;
	document.print();
	history.add(new AppendCommand(document, "Type Something! Escape key is tied to a macro, try it out!"), true);
	
	document.print();

	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	DWORD NumInputs = 0;
	DWORD InputsRead = 0;
	INPUT_RECORD irInput;
	char prev = NULL;
	bool skip = true;
	std::string output;
	while (ReadConsoleInput(hInput, &irInput, 1, &InputsRead))
	{
		if (skip)
		{
			skip = false;
			continue;
		}
		char ASCII = char(irInput.Event.KeyEvent.wVirtualKeyCode);
		if (ASCII && prev != ASCII)
		{
			if ((ASCII >= ' ' && ASCII <= '~'))
			{
					std::cout << ASCII << std::endl;
					output = "";
					output += ASCII;
					history.add(new AppendCommand(document, output), true);
					document.print();
			}
			else if (ASCII == char(8))
			{
				history.undo();
				document.print();
			}
			else if (ASCII == char(27))
			{
				CommandGroup* ESC = new CommandGroup();
				ESC->add(new AppendCommand(document, "The quick brown fox jumps over the lazy dog!"));
				ESC->add(new AppendCommand(document, "The quick brown fox jumps over the lazy dog!"));
				ESC->add(new AppendCommand(document, "The quick brown fox jumps over the lazy dog!"));
				history.add(ESC, true);
				document.print();
			}
		}
		else
		{
			ASCII = char();
		}
		FlushConsoleInputBuffer(hInput);
		prev = ASCII;
	}
	system("PAUSE");
}

