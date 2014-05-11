// mine_sweeper.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include "conio.h"
#include "mineSweeper.h"

int main()
{
	mine_sweeper mines(13,30,50,true);

	const char CPPKEYUP = 72;
	const char CPPKEYLEFT = 75;
	const char CPPKEYRIGHT = 77;
	const char CPPKEYDOWN = 80;
	const char CPPKEYRETURN = 'c';
	const char CPPKEYFLAG = 'f';
	const char CPPKEYOPEN = 'o';
	const char CPPKEYRESTART = 'r';
	char Arrow = 0;
	Arrow = _kbhit();
	while(mines.getGameStatus()) 
	{
		Arrow = _getch();
		//std::cout << Arrow << std::endl;
		switch(Arrow) 
		{
			case CPPKEYUP:
				mines.moveCursorUp();
			break;

			case CPPKEYDOWN:
				mines.moveCursorDown();
			break;

			case CPPKEYLEFT:
				mines.moveCursorLeft();
			break;

			case CPPKEYRIGHT:
				mines.moveCursorRight();
			break;

			case CPPKEYRETURN:
				mines.openCell();
			break;

			case CPPKEYFLAG:
				mines.flagCell();
			break;

			case CPPKEYOPEN:
				mines.openAll();
			break;

			case CPPKEYRESTART:
				mines.restart();
			break;
		}
	}
	return 0;
}

