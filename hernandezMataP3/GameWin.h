// File: GameWin.h
// Author: 
// Project: CS215 Project 3 Fall 2016
// Description of file contents:

#include <afxwin.h>
#include "Game.h"

class GameWin : public CFrameWnd
{
	public:
		GameWin ();
		afx_msg void OnPaint ();
		afx_msg void OnLButtonDown( UINT nFlags, CPoint point );
		afx_msg void OnKeyDown (UINT achar, UINT repeat, UINT flags);
		afx_msg void OnTimer (UINT nIDEvent);
	private:
		Game myGame;
		bool firstTime;
		DECLARE_MESSAGE_MAP ()
		int numRows;
		int numCols;
		int level;
};
