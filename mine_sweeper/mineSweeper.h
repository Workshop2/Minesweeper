#include <list>
#include <ctime>
#include <set>
#include <map>
#include <iostream>

class mine_sweeper
{
public:
	mine_sweeper(int width, int height, int bombs, bool autoRefresh = true);
	std::pair<int,bool> openCell();
	std::pair<int,bool> openCell(int y, int x);
	void flagCell();
	void flagCell(int y, int x);
	void display(bool clear = true);
	void moveCursor(int y, int x);
	void moveCursorLeft();
	void moveCursorRight();
	void moveCursorUp();
	void moveCursorDown();
	bool getGameStatus();
	void setAutoRefresh(bool refresh);
	void openAll();
	void restart();

private:
	//Data Storage
	int width_, height_, bombs_, posX, posY, numFlags;
	bool functional_, gameStatus_, autoRefresh_;
	std::list <std::list <std::pair <std::pair <int,short> , std::pair <int,int> > > > grid_data;  
	std::map<std::pair<int,int> ,bool> mines;

	//Private Functions
	void mine_sweeper_start(int width, int height, int bombs, bool autoRefresh = true);
	void autoRefreshProc_();
	void findEmpties(std::list<std::pair <int,int> > *, int y, int x);
	void checkComplete();
	void totalFlags();
};