// File: Game.h
// Author: Sergio Hernandez Mata
// Project: CS215 Project 3 Fall 2016
// Description of file contents:

#ifndef GAME_H
#define GAME_H

//includes the Microsoft Foundation Classes
#include <afxwin.h>


#define NUMIMAGES 10
#define MIN_TIME 30

enum square_types {PATH, WALL, ME, GOAL};
enum image_type {MEimg = 0, PATHimg= 4, WALLimg, GOALimg};

class Game
{
	public:
		Game ();
		Game (const Game & other);
		~Game ();
		Game & operator = (const Game & other);
		void Init (int R, int C, CFrameWnd * windowP);
		void Instructions (CFrameWnd * windowP);
		void Display (CFrameWnd * windowP);
		void Click (int x, int y, CFrameWnd * windowP);
		void Move (char dir, CFrameWnd * windowP);
		void Message (CFrameWnd * windowP);
		void UpdateTime (CFrameWnd * windowP);
		bool Done (CFrameWnd * windowP);
		void Temporary(int row, int col);
	private:

		void SetUp (CRect window);
		void DisplayGameRect (CDC * deviceContextP);
		void DisplayScoreRect (CDC * deviceContextP);
		void DisplaySpecialRect (CDC * deviceContextP);

		struct GameSquare
		{
			GameSquare ();
			void Display (CDC * deviceContextP);
			square_types what;
			int points;	// for goal
			int imageid;
			CRect where;
		};

		GameSquare ** grid;
		int numRows;
		int numCols;
		int score;
		int timeLeft;
		int currentRow, currentCol;
		int endRow, endCol;
		CRect gameRect;
		CRect specialRect;
		CRect scoreRect;
		CRect timeRect;
		CRect gamescoreRect;
		CBitmap bgImage;
};

#endif
