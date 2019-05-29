// File: Game.cpp
// Author:Sergio Hernandez Mata
// Project: CS215 Project 3 Fall 2016
// Description of file contents:

#include "Game.h"
#include <time.h>
#include <cstdlib>
#include <mmsystem.h>

//Initializes the object with a device-dependent memory bitmap that has a
//specified width, height, and bit pattern.
static CBitmap images [NUMIMAGES];

using namespace std;

Game::GameSquare::GameSquare ()
{
	// This function will: Initialize GameSquare's square_type what variable with
	// a wall enum type and set imageid to a WALLimg.

	what = WALL;
	imageid = WALLimg;
}

void Game::GameSquare::Display (CDC * deviceContextP)
{
	// This function will: Will Initialize a Class of Device-Context(CDC) object
	// and device context is a Windows data structure containing information about
	// the drawing attributes of a device All drawing calls are made through a
	// device-context object, which encapsulates the Windows APIs for drawing
	// lines, shapes, and text.
	CDC memDC;

	//The CreateCompatibleDC func Creates a memory-device context that is
	//compatible with another device context. You can use it to prepare images
	//in memory.
	int res = memDC.CreateCompatibleDC(deviceContextP);

	//Selects a GDI drawing object such as a pen.
	memDC.SelectObject(&images[imageid]);

	//Transfers a bit-block of color data from the specified source device
	//context into a destination device context, rendering a specified color
	//transparent in the transfer.
	deviceContextP->TransparentBlt (where.left, where.top, where.Width()+1,where.Height()+1,&memDC, 0, 0, 120, 90, RGB(255,255,255));

	//the device is notified and all storage and system resources used by the device are released.
	DeleteDC (memDC);
}

Game::Game ()
{
	// This function will:Initialize the game

	//srand will Initialize a random number generator and we will seed it with
	// the current calendar time
	srand (time (NULL));
	//Initializes the number rows and numCols
	numRows = numCols = 1;
	//This will allocate memory in the heap for grid of type GameSquare which is
	//a pointer to a pointer. This pointer will point to an array of of size
	//numRows
	grid = new GameSquare * [numRows];
	//This loop will fill in the squares columns
	for (int r = 0; r < numRows; r++)
		grid[r] = new GameSquare [numCols];

	//This function loads the specified bitmap resource from the executable
	//file for a module.
	int res = bgImage.LoadBitmap(CString("BACKGROUND_BMP"));
	res = images[PATHimg].LoadBitmap(CString("PATH_BMP"));
	res = images[WALLimg].LoadBitmap(CString("WALL_BMP"));

	res = images[MEimg + 0].LoadBitmap(CString("COPL_BMP"));
	res = images[MEimg + 1].LoadBitmap(CString("COPR_BMP"));
	res = images[MEimg + 2].LoadBitmap(CString("COPU_BMP"));
	res = images[MEimg + 3].LoadBitmap(CString("COPD_BMP"));


	res = images[GOALimg+4].LoadBitmap(CString("GOAL0_BMP"));
	res = images[GOALimg+3].LoadBitmap(CString("GOAL1_BMP"));
	res = images[GOALimg+2].LoadBitmap(CString("GOAL2_BMP"));
	res = images[GOALimg+1].LoadBitmap(CString("GOAL3_BMP"));
	res = images[GOALimg+0].LoadBitmap(CString("GOAL4_BMP"));

}

Game::~Game ()
{
	// This function will: Deallocate the memory used for the game.

	for (int r = 0; r < numRows; r++)
		delete [] grid[r];
	delete [] grid;
}

void Game::Init (int R, int C, CFrameWnd * windowP)
{
	// This function will: Execute the game

	BOOL soundPlayed = PlaySound(TEXT("SOUND_WAV"), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC | SND_LOOP);
	for (int r = 0; r < numRows; r++)
		delete [] grid[r];
	delete [] grid;
	numRows = R;
	numCols = C;
	currentRow = currentCol = 1;
	endRow = numRows - 2;
	endCol = numCols - 2;
	score = 0;
	timeLeft = MIN_TIME;
	grid = new GameSquare * [numRows];
	for (int r = 0; r < numRows; r++)
		grid[r] = new GameSquare [numCols];
	Temporary (1,1);
	grid[currentRow][currentCol].what = ME;
	grid[currentRow][currentCol].imageid = MEimg+1;
	grid[endRow][endCol].what = GOAL;
	grid[endRow][endCol].imageid = GOALimg;

	int count = 4;
	while (count != 0)
	{
		int randCol = rand() % C;
		int randRow = rand() % R;

		if (grid[randRow][randCol].what == PATH)
		{
			grid[randRow][randCol].what = GOAL;
			grid[randRow][randCol].imageid = GOALimg + count;
			count--;
		}
	}
}

void Game::Instructions (CFrameWnd * windowP)
{
	// This function will:

	CString message = "It's New Years in Vegas and the streets are flooded with streetracers!\nIt's up to you to keep our city safe by catching all of them before time runs out!";
	CString title = "Instructions";
	windowP->MessageBox (message, title);
}

// Replace this function with the function(s) used to generate your maze
void Game::Temporary(int row, int col)
{
	grid[row][col].what = PATH;
	grid[row][col].imageid = PATHimg;
	int rd = rand() % 4;
	for (int i = 0; i < 4; i++)
	{
		if (rd == 0 && col + 2 < numCols && grid[row][col + 2].what != PATH)// checkinging right bound
		{
			grid[row][col + 1].what = PATH;
			grid[row][col + 1].imageid = PATHimg;
			Temporary(row, col + 2);
		}
		else if (rd == 1 && row - 2 > 0 && grid[row - 2][col].what != PATH)//checking upper bound
		{
			grid[row - 1][col].what = PATH;
			grid[row - 1][col].imageid = PATHimg;
			Temporary(row - 2, col);
		}
		else if (rd == 2 && col - 2 > 0 && grid[row][col - 2].what != PATH)//checking left bound
		{
			grid[row][col - 1].what = PATH;
			grid[row][col - 1].imageid = PATHimg;
			Temporary(row, col - 2);
		}
		else if (rd == 3 && row + 2 < numRows && grid[row + 2][col].what != PATH)//checking lower bound
		{
			grid[row + 1][col].what = PATH;
			grid[row + 1][col].imageid = PATHimg;
			Temporary(row + 2, col);
		}
		rd = (rd + 1) % 4;
	}
}

void Game::Display (CFrameWnd * windowP)
{
	// This function will:

	CPaintDC dc (windowP);
	CRect rect;
	windowP->GetClientRect (&rect);

	CDC memDC;
	int res = memDC.CreateCompatibleDC(&dc);
	memDC.SelectObject(&bgImage);
	dc.TransparentBlt (0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, 2000, 1000, SRCCOPY);
	DeleteDC (memDC);

	SetUp (rect);
	DisplayGameRect (&dc);
	dc.SetBkMode(TRANSPARENT);
	CFont font;
	CString fontName = "Old English Text MT";
	font.CreatePointFont(rect.Height() / 4, fontName, &dc);
	CFont* def_font = dc.SelectObject(&font);
	COLORREF def_color = dc.SetTextColor (RGB (0, 0, 0));
	DisplayScoreRect (&dc);
	DisplaySpecialRect (&dc);
	dc.SelectObject(def_font);
	dc.SetTextColor(def_color);
}

void Game::SetUp (CRect rect)
{
	// This function will:

	int thirdW = rect.Width() / 3;
	int twelfthW = rect.Width() / 12;
	int twelfthH = rect.Height() / 12;

	gameRect = CRect (thirdW+twelfthW/2, twelfthH, rect.Width()-twelfthW, rect.Height()-twelfthH);
	specialRect = CRect (twelfthW, rect.Height()/1, thirdW-twelfthW/2, 3 * rect.Height() / 4);
	scoreRect = CRect (twelfthW, 3 * rect.Height()/4, thirdW-twelfthW/2, rect.Height()-twelfthH);

	int sqHeight = gameRect.Height() / numRows;
	int sqWidth = gameRect.Width() / numCols;
	int top = gameRect.top + (gameRect.Height() - sqHeight * numRows) / 2;
	int left = gameRect.left + (gameRect.Width() - sqWidth * numCols) / 2;
	for (int r = 0; r < numRows; r++)
		for (int c = 0; c < numCols; c++)
			grid[r][c].where = CRect(left + c*sqWidth, top + r*sqHeight, left + (c+1)*sqWidth, top + (r+1)*sqHeight);

	int thirdH = specialRect.Height() / 3;
	int halfW = specialRect.Width() / 2;
	timeRect = CRect (specialRect.left + halfW, specialRect.top, specialRect.right - halfW/2, specialRect.top+thirdH);

	thirdH = scoreRect.Height() / 3;
	gamescoreRect = CRect (scoreRect.left, scoreRect.top, scoreRect.right, scoreRect.top+thirdH);
}

void Game::DisplayGameRect (CDC * deviceContextP)
{
	// This function will:

	deviceContextP->Rectangle(gameRect);//will make game squares transparent
	for (int r = 0; r < numRows; r++)
		for (int c = 0; c < numCols; c++)
			grid [r][c].Display (deviceContextP);
}

void Game::DisplayScoreRect (CDC * deviceContextP)
{
	// This function will:

	//deviceContextP->Rectangle(scoreRect);
	//deviceContextP->Rectangle(gamescoreRect);
	char buffer [50];
	sprintf_s (buffer, "Score: %d", score);
	deviceContextP->DrawText(CString(buffer), gamescoreRect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
}

void Game::DisplaySpecialRect (CDC * deviceContextP)
{
	// This function will:

	//deviceContextP->Rectangle(specialRect);
	//deviceContextP->Rectangle(timeRect);
	char buffer [20];
	sprintf_s (buffer, "%d:%02d", timeLeft/60, (timeLeft < 0 ? 0 : timeLeft%60));
	deviceContextP->DrawText("Time:", CRect(specialRect.left, specialRect.top, timeRect.left, timeRect.bottom), DT_SINGLELINE | DT_VCENTER | DT_RIGHT);
	deviceContextP->DrawText(CString (buffer), timeRect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
}

void Game::Click (int y, int x, CFrameWnd * windowP)
{
	// This function will:

}

void Game::Move (char direction, CFrameWnd * windowP)
{
	// This function will:

	CRect oldWhere = grid[currentRow][currentCol].where | grid[currentRow-1][currentCol].where;
	switch (direction)
	{
		case 'a': // moving left
			if (currentCol - 1 > 0 && grid[currentRow][currentCol - 1].what == PATH)
			{
				grid[currentRow][currentCol].what = PATH;
				grid[currentRow][currentCol].imageid = PATHimg;
				currentCol = currentCol - 1;
				grid[currentRow][currentCol].what = ME;
				grid[currentRow][currentCol].imageid = MEimg+0;
			}
			else if (grid[currentRow][currentCol - 1].what == GOAL)
			{
				grid[currentRow][currentCol].what = PATH;
				grid[currentRow][currentCol].imageid = PATHimg;
				currentCol = currentCol - 1;
				grid[currentRow][currentCol].what = ME;
				grid[currentRow][currentCol].imageid = MEimg+0;

				windowP->InvalidateRect(scoreRect);
				score += 200;

			}
			break;

		case 'w': // moving up
			if (currentRow - 1 > 0 && grid[currentRow - 1][currentCol].what == PATH)
			{
				grid[currentRow][currentCol].what = PATH;
				grid[currentRow][currentCol].imageid = PATHimg;
				currentRow = currentRow - 1;
				grid[currentRow][currentCol].what = ME;
				grid[currentRow][currentCol].imageid = MEimg+2;
			}
			else if (grid[currentRow - 1][currentCol].what == GOAL)
			{
				grid[currentRow][currentCol].what = PATH;
				grid[currentRow][currentCol].imageid = PATHimg;
				currentRow = currentRow - 1;
				grid[currentRow][currentCol].what = ME;
				grid[currentRow][currentCol].imageid = MEimg+2;

				windowP->InvalidateRect(scoreRect);
				score += 200;
			}
			break;

		case 'd': // moving right
			if (currentCol + 1  < numCols && grid[currentRow][currentCol + 1].what == PATH)
			{
				grid[currentRow][currentCol].what = PATH;
				grid[currentRow][currentCol].imageid = PATHimg;
				currentCol = currentCol + 1;
				grid[currentRow][currentCol].what = ME;
				grid[currentRow][currentCol].imageid = MEimg+1;

			}
			else if (grid[currentRow][currentCol + 1].what == GOAL)
			{
				grid[currentRow][currentCol].what = PATH;
				grid[currentRow][currentCol].imageid = PATHimg;
				currentCol = currentCol + 1;
				grid[currentRow][currentCol].what = ME;
				grid[currentRow][currentCol].imageid = MEimg+1;

				windowP->InvalidateRect(scoreRect);
				score += 200;
			}
			break;

		case 's': // moving down
			if (currentRow + 1  < numRows && grid[currentRow + 1][currentCol].what == PATH)
			{
				grid[currentRow][currentCol].what = PATH;
				grid[currentRow][currentCol].imageid = PATHimg;
				currentRow = currentRow + 1;
				grid[currentRow][currentCol].what = ME;
				grid[currentRow][currentCol].imageid = MEimg+3;
			}
			else if (grid[currentRow + 1][currentCol].what == GOAL)
			{
				grid[currentRow][currentCol].what = PATH;
				grid[currentRow][currentCol].imageid = PATHimg;
				currentRow = currentRow + 1;
				grid[currentRow][currentCol].what = ME;
				grid[currentRow][currentCol].imageid = MEimg+3;

				windowP->InvalidateRect(scoreRect);
				score += 200;
			}

			break;
	}
	CRect newWhere = grid[currentRow][currentCol].where | grid[currentRow-1][currentCol].where;
	windowP->InvalidateRect (oldWhere | newWhere);
}

void Game::Message (CFrameWnd * windowP)
{
	// This function will:

	CString message, title;
	if (timeLeft > 0)
	{

		message = "You save the city!";
		title = "Congratulations!";
	}
	else
	{
		message = "Sorry you have to be faster than that!";
		title = "Sorry...";
	}
	windowP->KillTimer(1);
	windowP->MessageBox (message, title);
}

bool Game::Done (CFrameWnd * windowP)
{
	// This function will:

	if (timeLeft < 1)
	{
		return true;
	}
	else if (score >= 1000)
	{
		return true;
	}
	return false;
}

void Game::UpdateTime (CFrameWnd * windowP)
{
	// This function will: Update the elapsed time each time it is called by OnTimer.

	timeLeft--;
	if (timeLeft <= 0)
		windowP->KillTimer(1);
	windowP->InvalidateRect (timeRect);
}
