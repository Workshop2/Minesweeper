// Filename: mineSweeper.cpp
// Author: Simon Colmer
// ID:
// Version:
// Date: 05/09
// Description: The main class for mine_sweeper


#include "stdafx.h"
#include "mineSweeper.h"

mine_sweeper::mine_sweeper(int height, int width, int bombs, bool autoRefresh)
{
	mine_sweeper_start(height,width,bombs,autoRefresh);
}

void mine_sweeper::mine_sweeper_start(int height, int width, int bombs, bool autoRefresh)
{
	functional_ = false;
	gameStatus_ = false;
	autoRefresh_ = autoRefresh;

	if(width > 0 && height > 0 && bombs > 0)
		if((width * height) > bombs)
		{
			width_ = width;
			height_ = height;
			bombs_ = bombs;
			posY = 0;
			posX = 0;
			numFlags = 0;

			for(int i = 0; i < height; ++i)
			{
				std::list <std::pair <std::pair <int,short> , std::pair <int,int> > > temp_list;

				for(int j = 0; j < width; ++j)
				{
					std::pair <std::pair <int,short> , std::pair <int,int> > temp_pair;
					temp_pair.first.first = 0;
					temp_pair.first.second = false;

					temp_pair.second.first = i;
					temp_pair.second.second = j;

					temp_list.push_back(temp_pair);
				}
				grid_data.push_back(temp_list);
			}

			//Setting up the bombs
			int temp_bombs = bombs;
			srand((unsigned)time(0)); 
			while(temp_bombs > 0)
			{
				std::pair<int,int> temp_location;
				temp_location.first = rand()%height;
				temp_location.second = rand()%width;
				if(mines[temp_location] == false)
				{
					mines[temp_location] = true;
					--temp_bombs;
				}
			}

			std::list <std::list <std::pair <std::pair <int,short> , std::pair <int,int> > > >::iterator x_proc = grid_data.begin();

			//Process Numbers around bombs
			while(x_proc != grid_data.end())
			{
				std::list <std::pair <std::pair <int,short> , std::pair <int,int> > >::iterator y_proc = x_proc->begin();
				while(y_proc != x_proc->end())
				{
					std::pair<int,int> temp_location;
					temp_location.first = y_proc->second.first;
					temp_location.second = y_proc->second.second;

					if(!mines[temp_location])
					{
						int temp_bomb_total = 0;

						//Top left
						if(((temp_location.first - 1 ) >= 0) && ((temp_location.second - 1 ) >= 0))
						{
							std::pair<int,int> temp_location2 = temp_location;
							temp_location2.first = temp_location.first - 1;
							temp_location2.second = temp_location.second - 1;
							temp_bomb_total += mines[temp_location2];
						}

						//Top middle
						if((temp_location.first - 1 ) >= 0)
						{
							std::pair<int,int> temp_location2 = temp_location;
							temp_location2.first = temp_location.first - 1;
							temp_bomb_total += mines[temp_location2];
						}

						//Top right
						if((temp_location.first - 1 ) >= 0)
						{
							std::pair<int,int> temp_location2 = temp_location;
							temp_location2.first = temp_location.first - 1;
							temp_location2.second = temp_location.second + 1;
							temp_bomb_total += mines[temp_location2];
						}

						//Middle Left
						if((temp_location.second - 1 ) >= 0)
						{
							std::pair<int,int> temp_location2 = temp_location;
							temp_location2.second = temp_location.second - 1;
							temp_bomb_total += mines[temp_location2];
						}

						//Middle Right
						if((temp_location.second + 1 ) <= (width - 1))
						{
							std::pair<int,int> temp_location2 = temp_location;
							temp_location2.second = temp_location.second + 1;
							temp_bomb_total += mines[temp_location2];
						}

						//Bottom Left
						if((temp_location.second - 1 ) >= 0)
						{
							std::pair<int,int> temp_location2 = temp_location;
							temp_location2.first = temp_location.first + 1;
							temp_location2.second = temp_location.second - 1;
							temp_bomb_total += mines[temp_location2];
						}

						//Bottom Middle
						if((temp_location.first + 1 ) <= (height - 1))
						{
							std::pair<int,int> temp_location2 = temp_location;
							temp_location2.first = temp_location.first + 1;
							temp_bomb_total += mines[temp_location2];
						}

						//Bottom Right
						if(((temp_location.first + 1 ) <= (height - 1)) && ((temp_location.second + 1 ) <= (width - 1)))
						{
							std::pair<int,int> temp_location2 = temp_location;
							temp_location2.first = temp_location.first + 1;
							temp_location2.second = temp_location.second + 1;
							temp_bomb_total += mines[temp_location2];
						}

						y_proc->first.first = temp_bomb_total;
					}
					else
						y_proc->first.first = -1;
					++y_proc;
				}
				++x_proc;
			}
			gameStatus_ = true;
			functional_ = true;
			autoRefreshProc_();
		}
		else
			std::cout << "Error on construction, too many bombs" << std::endl;
	else
		std::cout << "Error on construction, initial setup failed" << std::endl;
}

bool mine_sweeper::getGameStatus()
{
	return gameStatus_;
}

std::pair<int,bool> mine_sweeper::openCell()
{
	return openCell(posY,posX);
}

std::pair<int,bool> mine_sweeper::openCell(int y, int x)
{
	std::pair<int,bool> returnVal;

	if(functional_ && gameStatus_)
	{	
		std::list <std::list <std::pair <std::pair <int,short> , std::pair <int,int> > > >::iterator x_proc = grid_data.begin();
		while(x_proc != grid_data.end())
		{
			std::list <std::pair <std::pair <int,short> , std::pair <int,int> > >::iterator y_proc = x_proc->begin();
			while(y_proc != x_proc->end())
			{
				if((y_proc->second.first == y) && (y_proc->second.second == x))
				{
					if(y_proc->first.second != 1)
					{
						y_proc->first.second = 1;
						if(y_proc->first.first == -1)
						{
							returnVal.second = false;
							gameStatus_ = false;
							std::cout << ">>>>>>>>>>>>>>>>>>>>GAME FAILED<<<<<<<<<<<<<<<<<<<<" << std::endl;
						}
						else
						{
							//Open all 0's around it
							if(y_proc->first.first == 0)
							{
								std::list<std::pair<int,int> > openTemp;
								std::list<std::pair<int,int> > * listPointer = &openTemp;

								findEmpties(listPointer,y,x);

								while(openTemp.size() > 0)
								{
									std::list <std::list <std::pair <std::pair <int,short> , std::pair <int,int> > > >::iterator x_proc2 = grid_data.begin();
									while(x_proc2 != grid_data.end())
									{
										std::list <std::pair <std::pair <int,short> , std::pair <int,int> > >::iterator y_proc2 = x_proc2->begin();
										while(y_proc2 != x_proc2->end())
										{
											if((y_proc2->second.first == openTemp.begin()->first) && (y_proc2->second.second == openTemp.begin()->second))
											{
												if(y_proc2->first.first == 0 && y_proc2->first.second == 0)
												{
													y_proc2->first.second = 1;
													findEmpties(listPointer,openTemp.begin()->first,openTemp.begin()->second);
												}
												else if(y_proc2->first.first != -1 && y_proc2->first.second == 0)
													y_proc2->first.second = 1;

											}
											++y_proc2;
										}
										++x_proc2;
									}
									openTemp.erase(openTemp.begin());
								}
							}
							returnVal.second = true;
						}

					}
					returnVal.first = y_proc->first.first;
				}
				++y_proc;
			}
			++x_proc;
		}
	}
	autoRefreshProc_();
	return returnVal;
}

void mine_sweeper::autoRefreshProc_()
{
	if(autoRefresh_)
		display(true);
}

void mine_sweeper::display(bool clear /*= true*/)
{
	totalFlags();
	if(clear)
		system("cls");

	if(functional_)
	{
		std::cout << "     Bombs: " << (bombs_ - numFlags)<< std::endl;


		std::list <std::list <std::pair <std::pair <int,short> , std::pair <int,int> > > >::iterator x_proc = grid_data.begin();
		std::cout << "     |";
		for(int i = 0; i < width_; ++i)
			std::cout << "--";
		std::cout << "|" << std::endl;

		while(x_proc != grid_data.end())
		{
			std::cout << "     |";
			//std::cout << "_";
			std::list <std::pair <std::pair <int,short> , std::pair <int,int> > >::iterator y_proc = x_proc->begin();
			while(y_proc != x_proc->end())
			{
				if((posY == y_proc->second.first) && (posX == y_proc->second.second))
					std::cout << ">";
				else
					std::cout << " ";

				if((y_proc->first.second == 1) && (y_proc->first.first > 0))
					std::cout << y_proc->first.first;
				else if((y_proc->first.second == 1) && (y_proc->first.first == 0))
					std::cout << " ";
				else if((y_proc->first.second == 1) && (y_proc->first.first == -1))
					std::cout << "B";
				else if((y_proc->first.second == -2))
					std::cout << "F";
				else
					std::cout << ".";


				++y_proc;
			}
			std::cout << "|" << std::endl;
			++x_proc;
		}

		std::cout << "     |";
		for(int i = 0; i < width_; ++i)
			std::cout << "--";
		std::cout << "|";
		std::cout << std::endl<<std::endl<<"Controls:" << std::endl;
		std::cout << "   Up = Up Arrow"<<std::endl;
		std::cout << "   Down = Down Arrow"<<std::endl;
		std::cout << "   Left = Left Arrow"<<std::endl;
		std::cout << "   Right = Right Arrow"<<std::endl;
		std::cout << "   Open Cell = c"<<std::endl;
		std::cout << "   Flag Cell = f"<<std::endl;
		if(!gameStatus_)
			std::cout << std::endl << "Please press 'r' to restart the game" << std::endl;
	}
	else
		std::cout << "Error, no game in progress" << std::endl;
}

void mine_sweeper::moveCursorUp()
{
	if(posY > 0 && gameStatus_)
		--posY;
	autoRefreshProc_();
}

void mine_sweeper::moveCursorDown()
{
	if(posY < (height_ -1) && gameStatus_)
		++posY;
	autoRefreshProc_();
}

void mine_sweeper::moveCursorLeft()
{
	if(posX > 0 && gameStatus_)
		--posX;
	autoRefreshProc_();
}

void mine_sweeper::moveCursorRight()
{
	if(posX < (width_ -1) && gameStatus_)
		++posX;
	autoRefreshProc_();
}

void mine_sweeper::openAll()
{
	std::list <std::list <std::pair <std::pair <int,short> , std::pair <int,int> > > >::iterator x_proc = grid_data.begin();

	//Process Numbers around bombs
	while(x_proc != grid_data.end())
	{
		std::list <std::pair <std::pair <int,short> , std::pair <int,int> > >::iterator y_proc = x_proc->begin();
		while(y_proc != x_proc->end())
		{
			y_proc->first.second = 1;
			++y_proc;
		}
		++x_proc;
	}
	
	autoRefreshProc_();

}

void mine_sweeper::flagCell()
{
	flagCell(posY, posX);
}

void mine_sweeper::flagCell(int y, int x)
{
	if(functional_ /*&& ((bombs_ - numFlags)>0)*/)
	{	
		std::list <std::list <std::pair <std::pair <int,short> , std::pair <int,int> > > >::iterator x_proc = grid_data.begin();
		while(x_proc != grid_data.end())
		{
			std::list <std::pair <std::pair <int,short> , std::pair <int,int> > >::iterator y_proc = x_proc->begin();
			while(y_proc != x_proc->end())
			{
				if((y_proc->second.first == y) && (y_proc->second.second == x))
				{
					if(y_proc->first.second == 0)
					{
						y_proc->first.second = -2;
					}
					else if(y_proc->first.second == -2)
					{
						y_proc->first.second = 0;
					}
				}
				++y_proc;
			}
			++x_proc;
		}
		autoRefreshProc_();
		checkComplete();
	}
}

void mine_sweeper::findEmpties(std::list<std::pair<int,int> > * openTemp,int y,int x)
{
	//Above
	if(y > 0)
	{
		std::pair<int,int> tempPair;
		tempPair.first = y -1;
		tempPair.second = x;
		openTemp->push_back(tempPair);
	}
	//Below
	if(y < (height_ -1))
	{
		std::pair<int,int> tempPair;
		tempPair.first = y +1;
		tempPair.second = x;
		openTemp->push_back(tempPair);
	}
	//Left
	if(x > 0)
	{
		std::pair<int,int> tempPair;
		tempPair.first = y;
		tempPair.second = x -1;
		openTemp->push_back(tempPair);
	}
	//Right
	if(x < (width_ -1))
	{
		std::pair<int,int> tempPair;
		tempPair.first = y;
		tempPair.second = x +1;
		openTemp->push_back(tempPair);
	}

	//Top Left
	if(y > 0 && x > 0)
	{
		std::pair<int,int> tempPair;
		tempPair.first = y -1;
		tempPair.second = x - 1;
		openTemp->push_back(tempPair);
	}
	//Top Right
	if((y > 0) && x < (width_ -1))
	{
		std::pair<int,int> tempPair;
		tempPair.first = y -1;
		tempPair.second = x +1;
		openTemp->push_back(tempPair);
	}
	//Bottom left
	if(y < (height_ -1) && x >0)
	{
		std::pair<int,int> tempPair;
		tempPair.first = y +1;
		tempPair.second = x -1;
		openTemp->push_back(tempPair);
	}
	//Bottom Right
	if(y < (height_ -1) && x < (width_ -1))
	{
		std::pair<int,int> tempPair;
		tempPair.first = y+1;
		tempPair.second = x +1;
		openTemp->push_back(tempPair);
	}
	
}


void mine_sweeper::checkComplete()
{
	if((bombs_ - numFlags) == 0)
	{
		int countFlags = 0;
		std::list <std::list <std::pair <std::pair <int,short> , std::pair <int,int> > > >::iterator x_proc = grid_data.begin();
		while(x_proc != grid_data.end())
		{
			std::list <std::pair <std::pair <int,short> , std::pair <int,int> > >::iterator y_proc = x_proc->begin();
			while(y_proc != x_proc->end())
			{
				if((y_proc->first.first == -1) && (y_proc->first.second == -2))
					++countFlags;
				++y_proc;
			}
			++x_proc;
		}
		if(countFlags == bombs_)
		{
			gameStatus_ = false;
			std::cout << ">>>>>>>>>>>>>>>>>>>> You ' finnnnnished it :) <<<<<<<<<<<<<<<<<<<<"<<std::endl<<"Please press 'r' to restart the game"<<std::endl;
		}
	}
}

void mine_sweeper::restart()
{
	while(grid_data.size() > 0)
		grid_data.erase(grid_data.begin());

	while(mines.size() > 0)
		mines.erase(mines.begin());

	mine_sweeper_start(height_, width_, bombs_);
}

void mine_sweeper::totalFlags()
{
	int countFlags = 0;
	std::list <std::list <std::pair <std::pair <int,short> , std::pair <int,int> > > >::iterator x_proc = grid_data.begin();
	while(x_proc != grid_data.end())
	{
		std::list <std::pair <std::pair <int,short> , std::pair <int,int> > >::iterator y_proc = x_proc->begin();
		while(y_proc != x_proc->end())
		{
			if(y_proc->first.second == -2)
				++countFlags;
			++y_proc;
		}
		++x_proc;
	}
	numFlags = countFlags;
}