/*
  The MIT License (MIT)

  library writen by Kris Kasprzak
  
  Permission is hereby granted, free of charge, to any person obtaining a copy of
  this software and associated documentation files (the "Software"), to deal in
  the Software without restriction, including without limitation the rights to
  use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
  the Software, and to permit persons to whom the Software is furnished to do so,
  subject to the following conditions:
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
  FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
  COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
  IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

  On a personal note, if you develop an application or product using this library 
  and make millions of dollars, I'm happy for you!

	rev		date			author				change
	1.0		1/2022			kasprzak			initial code
	2.0   1/2022      kasprzak      added touch support
	3.0   10/2023     kasprzak      fixed minor bugs

	// Website for generating icons
	// https://javl.github.io/image2cpp/


*/

#ifndef ILI9341_MENU_H
#define ILI9341_MENU_H

#define ILI9341_MENU_VER 3.0

#if ARDUINO >= 100
	 #include "Arduino.h"
	 #include "Print.h"
#else
	
#endif

#ifdef __cplusplus
	
#endif

#include <ILI9341_t3.h>  

	

#define MAX_OPT 17		// max elements in a menu, increase as needed
#define MAX_CHAR_LEN 30			// max chars in menus, increase as needed
#define TRIANGLE_H 3.7
#define TRIANGLE_W 2.5
#define MENU_C_DKGREY 0x4A49	// used for disable color, method to change
#define EXIT_TEXT "Exit"		// default text for menu exit text, change here or use a method
#define EDIT_TEXT "Edit"		// default text for menu exit text, change here or use a method

#define ICON_NONE 0
#define ICON_MONO 1
#define ICON_565  2

#define  BUTTON_PRESSED 1
#define  BUTTON_NOTPRESSED 0

class  EditMenu {
		
public:

	EditMenu(ILI9341_t3 *Display,bool EnableTouch = false);

	void init(uint16_t TextColor, uint16_t BackgroundColor, 
		uint16_t HighlightTextColor, uint16_t HighlightColor,
		uint16_t SelectedTextColor, uint16_t SelectedColor,
		uint16_t MenuColumn, uint16_t ItemRowHeight,uint16_t MaxRow,
		const char *TitleText, const ILI9341_t3_font_t &ItemFont, const ILI9341_t3_font_t &TitleFont);

	int addNI(const char *ItemText, float Data, float LowLimit, float HighLimit, 
		float Increment, byte DecimalPlaces = 0, const char **ItemMenuText = NULL);

	int addMono(const char *ItemText, float Data, float LowLimit, float HighLimit, 
		float Increment, byte DecimalPlaces = 0,const char **ItemMenuText = NULL, 
		const unsigned char *Bitmap = nullptr, uint8_t BitmapWidth = 0, uint8_t BitmapHeight = 0);

	int add565(const char *ItemText, float Data, float LowLimit, float HighLimit, 
		float Increment, byte DecimalPlaces = 0, const char **ItemMenuText = NULL, 
		const uint16_t *Bitmap = nullptr, uint8_t BitmapWidth = 0, uint8_t BitmapHeight = 0);
	
	int selectRow();

	void draw();

	void MoveUp();

	void MoveDown();

	void setTitleColors(uint16_t TitleTextColor, uint16_t TitleFillColor);

	void setTitleBarSize(uint16_t TitleTop, uint16_t TitleLeft, uint16_t TitleWith, uint16_t TitleHeight);

	void setTitleText(const char *TitleText,const  char *ExitText);
	
	void setTitleText(const char *TitleText, const char *ExitText, const char *EditText);

	void setTitleTextMargins(uint16_t LeftMargin, uint16_t TopMargin);

	void setIncrementDelay(uint16_t Delay);
	
	void setMenuBarMargins(uint16_t LeftMargin, uint16_t Width, uint16_t BorderRadius, uint16_t BorderThickness);

	void setItemColors( uint16_t DisableTextColor, uint16_t BorderColor, uint16_t EditModeBorderColor = 0);

	void setItemTextMargins(uint16_t LeftMargin, uint16_t TopMargin, uint16_t MenuMargin);

	void setItemText(int ItemID, const char *ItemText);

	void setIconMargins(uint16_t LeftMargin, uint16_t TopMargin);

	void SetItemValue(int ItemID, float ItemValue);
	
	void setLimits(int ItemID, float LowLimit, float HighLimit, float Increment, byte DecimalPlaces = 0);

	void setAllColors(
	uint16_t TextColor, uint16_t BackgroundColor, 
	uint16_t HighlightTextColor, uint16_t HighlightColor, uint16_t HighlightBorderColor,
	uint16_t SelectedTextColor, uint16_t SelectedColor, uint16_t SelectBorderColor,
	uint16_t DisableTextColor ,	uint16_t TitleTextColor, uint16_t TitleFillColor);

	void disable(int ID);

	void enable(int ID);

	bool getEnableState(int ID);
	
	int press(int16_t ScreenX, int16_t ScreenY);

	void drawRow(int ID);
		
	float value[MAX_OPT];
	
	bool isEditing();
	
	int item;

private:

	void drawHeader(bool ShowEdit);
	
	void up();

	void down();

	void incrementUp();
	
	void incrementDown();

	void drawItems();

	void drawMonoBitmap(int16_t x, int16_t y, const unsigned char *bitmap, uint8_t w, uint8_t h, uint16_t color);

	void draw565Bitmap(int16_t x, int16_t y, const uint16_t *bitmap, uint8_t w, uint8_t h);
	
	ILI9341_t3 *d;
	char itemlabel[MAX_OPT][MAX_CHAR_LEN];
	char ttx[MAX_CHAR_LEN];
	char etx[MAX_CHAR_LEN]; 
	char dtx[MAX_CHAR_LEN];
	ILI9341_t3_font_t itemf;
	ILI9341_t3_font_t titlef;
	uint16_t itc, ibc, ihtc, ihbc, istc, isbc;	// item variables
	uint16_t tbl, tbt, tbw, tbh, ttc, tfc, tox, toy;	// title variables
	// margins
	uint16_t imr, isx, itx, isy, irh, irw, ioy, iox, mm,icox, icoy;
	int i;
	int totalID;
	int MaxRow;
	int currentID;
	int cr;
	byte debounce;
	bool EditMode = false;
	int sr, pr;
	float low[MAX_OPT];
	float high[MAX_OPT];
	float inc[MAX_OPT];
	byte dec[MAX_OPT];
	
	char **itemtext[MAX_OPT];
	
	bool rowselected = false;
	bool haslist[MAX_OPT];
	bool enablestate[MAX_OPT];
	bool drawTitleFlag = true;
	bool redraw = false;
	uint16_t ditc = 0;
	uint16_t col = 0;
	uint16_t temptColor = 0, bcolor = 0, sbcolor = 0;
	const unsigned char	*itemBitmap[MAX_OPT];
	const uint16_t *item565Bitmap[MAX_OPT];
	uint8_t bmp_w[MAX_OPT];
	uint8_t bmp_h[MAX_OPT];
	byte IconType[MAX_OPT];
	uint16_t  radius = 0;
	uint16_t thick = 0;
	uint16_t incdelay = 50;
	bool InputFromTouch = false;
	bool dh = true; 
};

class  ItemMenu {

	
public:
	ItemMenu(ILI9341_t3 *Display, bool EnableTouch = false);
	
	void init(uint16_t TextColor, uint16_t BackgroundColor,
		uint16_t HighlightTextColor, uint16_t HighlightColor, 
		uint16_t ItemRowHeight,uint16_t MaxRow,
		const char *TitleText, const ILI9341_t3_font_t &ItemFont, const ILI9341_t3_font_t &TitleFont);
		
	int addNI(const char *ItemLabel);

	int addMono(const char *ItemLabel, const unsigned char *Bitmap, uint8_t BitmapWidth, uint8_t BitmapHeight );

	int add565(const char *ItemLabel, const uint16_t *Bitmap, uint8_t BitmapWidth, uint8_t BitmapHeight);

	void draw();
	
	void MoveUp();

	void MoveDown();

	int selectRow();

	void setTitleColors(uint16_t TitleTextColor, uint16_t TitleFillColor);

	void setTitleBarSize(uint16_t TitleTop, uint16_t TitleLeft, uint16_t TitleWith, uint16_t TitleHeight);

	void setTitleText( char *TitleText,  char *ExitText);
	
	void setTitleText(char *TitleText, char *ExitText, char *EditText);
	
	void setTitleTextMargins(uint16_t LeftMargin, uint16_t TopMargin);

	void setMenuBarMargins(uint16_t LeftMargin, uint16_t Width, byte BorderRadius, byte BorderThickness);

	void setItemColors( uint16_t DisableTextColor, uint16_t BorderColor);

	void setItemTextMargins(uint16_t LeftMargin, uint16_t TopMargin, uint16_t MenuMargin);

	void setItemText(int ItemID, const char *ItemText);

	void setIconMargins(uint16_t LeftMargin, uint16_t TopMargin);

	void setAllColors(uint16_t TextColor, uint16_t BackgroundColor, 
	uint16_t HighlightTextColor, uint16_t HighlightColor, 
	uint16_t HighLightBorderColor, uint16_t DisableTextColor, 
	uint16_t TitleTextColor, uint16_t TitleFillColor);

	void disable(int ID);

	void enable(int ID);

	bool getEnableState(int ID);

	int press(int16_t ScreenX, int16_t ScreenY);

	void drawRow(int ID, uint8_t style);

	float value[MAX_OPT];

	int item;

private:

	void drawHeader(bool ShowEdit);

	void drawItems();
	
	void drawMonoBitmap(int16_t x, int16_t y, const unsigned char *bitmap, uint8_t w, uint8_t h, uint16_t color);

	void draw565Bitmap(int16_t x, int16_t y, const uint16_t *Bitmap , uint8_t w, uint8_t h);

	ILI9341_t3 *d;
	//bool enabletouch;
	char itemlabel[MAX_OPT][MAX_CHAR_LEN];
	char ttx[MAX_CHAR_LEN];
	char etx[MAX_CHAR_LEN];
	char dtx[MAX_CHAR_LEN];
	ILI9341_t3_font_t itemf;
	ILI9341_t3_font_t titlef;
	uint16_t bkgr, isx, itx, isy, irh, itc, ibc, ihbc, ihtc, isc, imr, irw, ioy, iox;	// item variables
	uint16_t tbl, tbt, tbw, tbh, ttc, tfc, tox, toy, icox, icoy, di, mm;	// title variables
	uint16_t ditc, difc, temptColor, bcolor;
	bool hasIcon = false, moreup = false, moredown = false;
	int i;
	int totalID;
	int MaxRow;
	int currentID;
	int cr;
	byte debounce;
	int sr, pr;
	const unsigned char *itemBitmap[MAX_OPT];
	const uint16_t *item565Bitmap[MAX_OPT];
	bool rowselected = false;
	bool drawTitleFlag = true;
	bool redraw = false;
	bool enablestate[MAX_OPT];
	uint8_t bmp_w[MAX_OPT];
	uint8_t bmp_h[MAX_OPT];
	byte IconType[MAX_OPT];
	byte radius, thick;
	bool InputFromTouch = false;
	bool dh = true; // draw header
	
};


#endif
