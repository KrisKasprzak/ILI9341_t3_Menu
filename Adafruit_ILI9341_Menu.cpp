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

  rev   date      author        change
  1.0   1/2022      kasprzak      initial code

*/

#include <Adafruit_ILI9341_Menu.h>  
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

EditMenu::EditMenu(Adafruit_ILI9341 *Display) {
  d = Display;
}

void EditMenu::init(uint16_t TextColor, uint16_t BackgroundColor,
                    uint16_t HighlightTextColor, uint16_t HighlightColor,
                    uint16_t SelectedTextColor, uint16_t SelectedColor,
                    uint16_t MenuColumn, uint16_t ItemRowHeight, uint16_t MaxRow,
                    const char *TitleText, const GFXfont  &ItemFont, const GFXfont  &TitleFont) {

  itc = TextColor;
  ibc = BackgroundColor;
  ihtc = HighlightTextColor;
  ihbc = HighlightColor;
  istc = SelectedTextColor;
  isbc = SelectedColor;
 
  bcolor = TextColor;
  tbt = 0;        // title bar top
  tbl = 0;        // title bar left
  tbh = 40;       // title bar height
  tbw = d->width(); // title bar width, default to screen width
  tox = 20;       // x pixel offset for title text to manage centering
  toy  = 8;       // y pixel offset for title text to manage height centering
  isx = 10;       // where to start the menu item from left, default indent 10
  mm = 5;
  isy = tbt + tbh - ItemRowHeight + mm;  // where to start the menu item from top, default below menu bar by 10, note because array is 1 based we start drawing down a row--so back off a row, but pad 10
  irh = ItemRowHeight;  // select bar height
  irw = tbw - isx;    // select bar width, default to full width
  col = MenuColumn;
  imr = MaxRow;     // user has to indicate this
  iox = 0;  // pixels to offset text in menu bar
  ioy = 0;  // pixels to offset text in menu bar
  itemf = ItemFont;     // item font
  titlef = TitleFont;     // title font
  ttc = TextColor;      // title text color
  tfc = HighlightColor;     // title fill color
  ditc = MENU_C_DKGREY;
  strncpy(ttx, TitleText, MAX_CHAR_LEN);
  strncpy(etx, EXIT_TEXT, MAX_CHAR_LEN);
  currentID = 1;    // id of current highlighted or selected item (1 to ID)
  cr = 1;       // current selected row on the screen (1 to mr-1)
  totalID = 0;    // maximum number of items (where first position is 1)
  sr = 0;       // draw offset for the menu array where first postion is 0
  pr = 1;       // previous selected rown (1 to mr - 1)
  rowselected = false;
  enablestate[0] = true;
  radius = 0;
  thick = 0;
}

int EditMenu::addNI(const char *ItemText, float Data, float LowLimit, float HighLimit, float Increment,
                    byte DecimalPlaces, const char **ItemMenuText) {

  totalID++;
  strcpy(itemlabel[totalID], ItemText);
  data[totalID] = Data;
  low[totalID] = LowLimit;
  high[totalID] = HighLimit;
  inc[totalID] = Increment;
  dec[totalID] = DecimalPlaces;
  haslist[totalID] = false;
  value[totalID] = Data;

  if (ItemMenuText) {
    haslist[totalID] = true;
    itemtext[totalID] = (char **) ItemMenuText;
  }

  IconType[totalID] = ICON_NONE;

  enablestate[totalID] = true;
  return (totalID);

}
int EditMenu::addMono(const char *ItemText, float Data, float LowLimit, float HighLimit, float Increment,
                      byte DecimalPlaces, const char **ItemMenuText,
                      const unsigned char *Bitmap, uint8_t BitmapWidth, uint8_t BitmapHeight) {

  totalID++;
  strcpy(itemlabel[totalID], ItemText);
  data[totalID] = Data;
  low[totalID] = LowLimit;
  high[totalID] = HighLimit;
  inc[totalID] = Increment;
  dec[totalID] = DecimalPlaces;
  haslist[totalID] = false;
  value[totalID] = Data;

  if (ItemMenuText) {
    haslist[totalID] = true;
    itemtext[totalID] = (char **) ItemMenuText;
  }

  itemBitmap[totalID] = Bitmap;
  bmp_w[totalID] = BitmapWidth;
  bmp_h[totalID] = BitmapHeight;
  IconType[totalID] = ICON_MONO;

  enablestate[totalID] = true;
  return (totalID);

}

int EditMenu::add565(const char *ItemText, float Data, float LowLimit, float HighLimit, float Increment,
                     byte DecimalPlaces, const char **ItemMenuText,
                     const uint16_t *Bitmap, uint8_t BitmapWidth, uint8_t BitmapHeight) {

  totalID++;
  strcpy(itemlabel[totalID], ItemText);
  data[totalID] = Data;
  low[totalID] = LowLimit;
  high[totalID] = HighLimit;
  inc[totalID] = Increment;
  dec[totalID] = DecimalPlaces;
  haslist[totalID] = false;
  value[totalID] = Data;

  if (ItemMenuText) {
    haslist[totalID] = true;
    itemtext[totalID] = (char **) ItemMenuText;
  }

  item565Bitmap[totalID] = Bitmap;
  bmp_w[totalID] = BitmapWidth;
  bmp_h[totalID] = BitmapHeight;
  IconType[totalID] = ICON_565;

  enablestate[totalID] = true;
  return (totalID);

}

int EditMenu::selectRow() {

  if (currentID == 0) {
    // trigger to exit out of the menu
    item = 0;
    return 0;
  }
  // otherwise this is the trigger to enable editing in the row

  rowselected = !rowselected;

  drawRow(currentID);
  item = currentID;
  return currentID;

}


void EditMenu::drawHeader(bool drawtype) {

  d->setFont(&titlef);

  if (drawtype) {
    // draw new menu bar
    d->fillRect(tbl, tbt, tbw, tbh, ihbc);
    d->setCursor(tbl + tox, tbt + toy);
    d->setTextColor(ihtc);
    d->print(etx);
	if (tbh > 25) {
		// anything less will distor the triangles
		d->fillTriangle( tbw - (tbh/3.0) - 5,	tbt + (tbh/2.0) - 3,
						 tbw - (tbh/6.0) - 5,	tbt + (tbh/2.0) -(tbh/3.0),
						 tbw - 5,				tbt + (tbh/2.0) - 3,
						 ihtc);

		d->fillTriangle( tbw - (tbh/3.0) - 5,	tbt + (tbh/2.0) + 3,
						 tbw - (tbh/6.0) - 5,	tbt + (tbh/2.0) +(tbh/3.0),
						 tbw - 5,				tbt + (tbh/2.0) + 3,
						 ihtc);

	  }
  }
  else {
    d->fillRect(tbl, tbt, tbw, tbh, tfc);
    d->setCursor(tbl + tox, tbt + toy);

    d->setTextColor(ttc);
    d->print(ttx);
  }

}

void EditMenu::up() {
  cr--;
  currentID--;
  drawItems();
  if (enablestate[currentID] == false) {
    while (enablestate[currentID] == false) {
      cr--;
      currentID--;
       drawItems();
    }
  }

}

void EditMenu::down() {
  cr++;
  currentID++;
   drawItems();
  if (enablestate[currentID] == false) {
    while (enablestate[currentID] == false) {
      cr++;
      currentID++;
       drawItems();
    }
  }
 

}


void EditMenu::MoveUp() {

  if (rowselected) {
    incrementUp();
  }
  else {
    up();
  }

}

void EditMenu::MoveDown() {

  if (rowselected) {
    incrementDown();
  }
  else {
    down();
  }
}



void EditMenu::draw() {
  drawHeader(false);

  drawItems();
}


void EditMenu::drawItems() {

  int tpr = 0;
  int bs ;
  if (imr > totalID) {
    imr = totalID;
  }
  redraw = false;

  // determine if we need to pan or just increment the list
  if ((currentID >  totalID) && (sr >= 0) ) {
    // up to top
    cr = 0;
    currentID = 0;
    sr = 0;
    redraw = true;
  }

  else if ( (cr < 0) && (sr == 0) ) {
    //  pan whole menu to bottom
    cr = imr;
    currentID = totalID;
    sr = totalID - imr;
    redraw = true;
  }
  else if ( (cr > imr) && ((sr + cr) > totalID) ) {
    //  pan whole menu to top
    cr = 1;
    currentID = 1;
    sr = 0;
    redraw = true;
  }
  else if ((cr > imr) && (sr >= 0))   {
    // scroll whole list up one by one
    sr++;
    cr = imr;
    redraw = true;
  }
  else if ((cr < 1) && (sr > 0))   {
    // scroll whole list down one by one
    sr--;
    cr = 1;
    redraw = true;
  }

  // determine if we need arrows
  if (cr == imr) {
    moredown = true;
  }
  if (cr == 1) {
    moreup = true;
  }

  if (enablestate[currentID] == false) {
    return;
  }

  // determine if we need to redraw the header and draw
  if ((currentID == 0) && (cr == 0)) {
    drawHeader(true);
  }
  else if ((pr == 0) && (cr > 0)) {
    drawHeader(false);
  }
  d->setFont(&itemf);
  // now draw the items in the rows
  for (i = 1; i <= imr; i++) {

    if (redraw) {
      // pr = i;
      // scroll so blank out every row
      d->fillRect(isx, isy + (irh * i) , irw, irh, ibc); // back color

    }

    if (enablestate[i + sr]) {
      temptColor = itc;
    }
    else {
      temptColor = ditc;
    }

    if (i == pr) {
      // maybe just row change so blank previous
      d->fillRect(isx + bmp_w[i + sr], isy + (irh * pr) , irw - bmp_w[i + sr], irh, ibc); // back color

    }

	// icon start 
	// icox;
	// menu bar start 
	bs =	 icox + bmp_w[i + sr] + isx;
	// text start
	itx = bs + iox;

    if (i == cr) {

		if (radius > 0){
			d->fillRoundRect(bs,			isy + (irh * i),			irw - bs				,	irh,				radius,bcolor);
			d->fillRoundRect(bs + thick,	isy + (irh * i) + thick,	irw - bs - (2*thick)	,	irh - (2*thick),	radius, ihbc);
		}
		else {
			d->fillRect(bs,			isy + (irh * i) ,			irw - bs,				irh, bcolor);
			d->fillRect(bs + thick,	isy + (irh * i) + thick,	irw - bs - (2*thick) ,	irh - (2*thick), ihbc);
		}

		
      tpr = i;
	 
		if (moreup) {
		d->fillTriangle(irw - 2 - (2*thick),							isy + (irh * i) + irh - (irh / TRIANGLE_H),
						irw - ((irh / TRIANGLE_W)*.5) - 2- (2*thick),	isy + (irh * i) + (irh / TRIANGLE_H),
						irw - (irh / TRIANGLE_W) - 2- (2*thick),		isy + (irh * i) + irh - (irh / TRIANGLE_H),
						ihtc);
		}
		if (moredown) {
		d->fillTriangle(irw - 2- (2*thick),								isy + (irh * i) + (irh / TRIANGLE_H),
						irw - ((irh / TRIANGLE_W)*.5) - 2- (2*thick),	isy + (irh * i) + irh - (irh / TRIANGLE_H),
						irw - (irh / TRIANGLE_W) - 2- (2*thick),		isy + (irh * i) + (irh / TRIANGLE_H),
						ihtc);
		}

	  temptColor = ihtc;


    }

	// write new text

    d->setTextColor(temptColor);
    d->setCursor(itx, isy + (irh * i) + ioy);
    d->print(itemlabel[i + sr]);

    // write new val
    d->setCursor(col , isy + (irh * i) + ioy);
    if (haslist[i + sr]) {
      d->print(itemtext[i + sr][(int) data[i + sr]]);
    }
    else {
      d->print(data[i + sr], dec[i + sr]);
    }


    // write bitmap
    if (IconType[i + sr] == ICON_MONO) {
      drawMonoBitmap(icox,  icoy + isy + (irh * i), itemBitmap[i + sr], bmp_w[i + sr], bmp_h[i + sr], temptColor );
    }
    else if (IconType[i + sr] == ICON_565) {
      draw565Bitmap(icox,  icoy + isy + (irh * i), item565Bitmap[i + sr], bmp_w[i + sr], bmp_h[i + sr] );
    }


  }

  moreup = false;
  moredown = false;
  pr = tpr;

}



void EditMenu::drawRow(int itemID) {

  bool drawarrow = false;
  uint16_t textcolor, backcolor, bordercolor;
  bordercolor = bcolor;

  moredown = false;
  moreup = false;

  	// compute starting place for text
	itx = isx + icox +  bmp_w[itemID];
	 bordercolor = bcolor;
	 textcolor = itc;

  if (itemID <= sr) {
    // item is off screen
    return;
  }

  if (itemID > (sr + imr)) {
    // item is off screen
    return;
  }

  if ((cr + sr) == itemID) {
    // current row is draw row AND selected
    textcolor = istc;
    backcolor = isbc;
    drawarrow = false;

    if (!rowselected) {
      // case 2 = current row is draw row AND highlighted
      textcolor = ihtc;
      backcolor = ihbc;
      drawarrow = true;
    }
    else if (rowselected) {
      textcolor = istc;
      backcolor = isbc;
      drawarrow = false;
    }
  }
  else  {
    // current row is not selected
    if (enablestate[itemID]) {
      textcolor = itc;
	  bordercolor = ibc;
    }
    else {
      textcolor = ditc;
	  bordercolor = ibc;
    }
    backcolor = ibc;
    drawarrow = true;
  }

  if ((cr == 5) && (!rowselected)) {
    moredown = true;
  }
  else if ((cr == 1) && (!rowselected)) {
    moreup = true;
  }
  else if (cr == 0) {
    moredown = true;
    moreup = true;
  }

	if (radius > 0){
		d->fillRoundRect(itx,			isy + (irh * (itemID - sr)) ,			irw - itx,				irh,		radius,bordercolor);
		d->fillRoundRect(itx + thick,	isy + (irh * (itemID - sr)) + thick,	irw - itx - (2*thick) - isx , irh - (2*thick), radius, backcolor);
	}
	else {
		d->fillRect(itx,			isy + (irh * (itemID - sr)) ,			irw -itx  ,				irh, bcolor);
		d->fillRect(itx + thick,	isy + (irh * (itemID - sr)) + thick,	irw - itx - (2*thick) ,	irh - (2*thick), backcolor);

	}

  d->setFont(&itemf);
  d->setTextColor(textcolor);
  d->setCursor(itx + iox, isy + (irh * (itemID - sr)) + ioy);
  d->print(itemlabel[itemID]);

  d->setCursor(col , isy + (irh * (itemID - sr)) + ioy);
  if (haslist[itemID]) {
    d->print(itemtext[itemID][(int) data[itemID]]);
  }
  else {
    d->print(data[itemID], dec[itemID]);

  }

  // write bitmap
  if (IconType[itemID] == ICON_MONO) {
    drawMonoBitmap(icox,  icoy + isy + (irh * (itemID - sr)), itemBitmap[itemID], bmp_w[itemID], bmp_h[itemID], temptColor );
  }
  else if (IconType[itemID] == ICON_565) {
    draw565Bitmap(icox,  icoy + isy + (irh * (itemID - sr)), item565Bitmap[itemID], bmp_w[itemID], bmp_h[itemID] );
  }


  if (drawarrow) {
    if (moreup) {
      d->fillTriangle(  irw - 2-(2*thick),             isy + (irh * cr) + irh - (irh / TRIANGLE_H),
                        irw - ((irh / TRIANGLE_W)*.5) - 2-(2*thick),  isy + (irh * cr) + (irh / TRIANGLE_H),
                        irw - (irh / TRIANGLE_W) - 2-(2*thick),   isy + (irh * cr) + irh - (irh / TRIANGLE_H),
                        ihtc);
    }
    else if (moredown) {
      d->fillTriangle(  irw - 2-(2*thick),              isy + (irh * cr) + (irh / TRIANGLE_H),
                        irw - ((irh / TRIANGLE_W)*.5) - 2-(2*thick),  isy + (irh * cr) + irh - (irh / TRIANGLE_H),
                        irw - (irh / TRIANGLE_W) - 2-(2*thick),   isy + (irh * cr) + (irh / TRIANGLE_H),
                        ihtc);
    }
  }

}

void EditMenu::incrementUp() {

  d->setFont(&itemf);
  d->setTextColor(istc);

  if (haslist[currentID]) {

    if ((data[currentID] + inc[currentID]) < high[currentID]) {
      data[currentID] += inc[currentID];
	  d->fillRect(col, isy + (irh * cr)+thick, irw - col - (2*thick), irh- (2*thick), isbc);
      d->setCursor(col + iox , isy + (irh * cr) + ioy);
      d->print(itemtext[currentID][(int) data[currentID]]);
    }
    else {
      data[currentID] = low[currentID];
      d->fillRect(col, isy + (irh * cr)+thick, irw - col - (2*thick), irh- (2*thick), isbc);
      d->setCursor(col, isy + (irh * cr) + ioy);
      d->print(itemtext[currentID][(int) data[currentID]]);
    }

  }
  else {
    data[currentID] += inc[currentID];
    if (data[currentID] > high[currentID]) {
      data[currentID] = low[currentID];
    }
    d->fillRect(col, isy + (irh * cr)+thick, irw - col - (2*thick), irh- (2*thick), isbc);
    d->setCursor(col +  iox, isy + (irh * cr) + ioy);
    d->print(data[currentID], dec[currentID]);
  }
  if (IconType[currentID] == ICON_MONO) {
    drawMonoBitmap(icox,  icoy + isy + (irh * cr), itemBitmap[currentID], bmp_w[currentID], bmp_h[currentID], temptColor );
  }
  else if (IconType[currentID] == ICON_565) {
    draw565Bitmap(icox,  icoy + isy + (irh * cr), item565Bitmap[currentID], bmp_w[currentID], bmp_h[currentID] );
  }
  value[currentID] = data[currentID];
  item = currentID;
}

void EditMenu::incrementDown() {
     d->setFont(&itemf);
      d->setTextColor(istc);
  if (haslist[currentID]) {
    if ((data[currentID] - inc[currentID]) >= low[currentID]) {
      data[currentID] -= inc[currentID];
      d->fillRect(col , isy + (irh * cr)+thick, irw - col - (2*thick), irh- (2*thick), isbc);
      d->setCursor(col , isy + (irh * cr) + ioy);
      d->print(itemtext[currentID][(int) data[currentID]]);
    }
    else {
      data[currentID] = high[currentID] - 1;
      d->fillRect(col , isy + (irh * cr)+thick, irw - col - (2*thick), irh- (2*thick), isbc);
      d->setCursor(col, isy + (irh * cr) + ioy);
      d->print(itemtext[currentID][(int) data[currentID]]);
    }
  }
  else {
    data[currentID] -= inc[currentID];
    if (data[currentID] < low[currentID]) {
      data[currentID] = high[currentID];
    }
    d->fillRect(col, isy + (irh * cr)+thick, irw - col - (2*thick), irh- (2*thick), isbc);
    d->setCursor(col , isy + (irh * cr) + ioy);
    d->print(data[currentID], dec[currentID]);
  }

  if (IconType[currentID] == ICON_MONO) {
    drawMonoBitmap(icox,  icoy + isy + (irh * cr), itemBitmap[currentID], bmp_w[currentID], bmp_h[currentID], temptColor );
  }
  else if (IconType[currentID] == ICON_565) {
    draw565Bitmap(icox,  icoy + isy + (irh * cr), item565Bitmap[currentID], bmp_w[currentID], bmp_h[currentID] );
  }

  value[currentID] = data[currentID];
  item = currentID;

}

void EditMenu::setTitleColors( uint16_t TitleTextColor, uint16_t TitleFillColor) {
  ttc = TitleTextColor;
  tfc = TitleFillColor;
}

void EditMenu::setTitleBarSize(uint16_t TitleTop, uint16_t TitleLeft, uint16_t TitleWith, uint16_t TitleHeight){
  tbt = TitleTop;
  tbl = TitleLeft;
  tbw = TitleWith;
  tbh = TitleHeight;
}

void EditMenu::setTitleText( char *TitleText,  char *ExitText) {
  strncpy(ttx, TitleText, MAX_CHAR_LEN);
  strncpy(etx, ExitText, MAX_CHAR_LEN);
}

void EditMenu::setTitleTextMargins(uint16_t LeftMargin, uint16_t TopMargin){
  tox = LeftMargin; // pixels to offset text in menu bar
  toy = TopMargin;  // pixels to offset text in menu bar
}


void EditMenu::setMenuBarMargins(uint16_t LeftMargin, uint16_t Width, uint16_t BorderRadius, uint16_t BorderThickness) {
  isx = LeftMargin;  // pixels to offset text in menu bar
  irw = Width - isx;  // pixels to offset text in menu bar
  radius = BorderRadius;
  thick = BorderThickness;   
}

void EditMenu::setItemColors( uint16_t DisableTextColor, uint16_t BorderColor) {
  ditc = DisableTextColor;
  bcolor = BorderColor;
}


void EditMenu::setItemTextMargins(uint16_t LeftMargin, uint16_t TopMargin, uint16_t MenuMargin) {
  iox = LeftMargin;  // pixels to offset text in menu bar
  ioy = TopMargin;  // pixels to offset text in menu bar
  mm = MenuMargin; ////////////////////////////////////////////////
  isy = tbt + tbh - irh + mm;
}

void EditMenu::setItemText(int ItemID, const char *ItemText) {
	strcpy(itemlabel[ItemID], ItemText);
	drawRow(ItemID);
}

void EditMenu::setIconMargins(uint16_t LeftMargin, uint16_t TopMargin) {
  icox = LeftMargin; // pixels to offset text in menu bar
  icoy = TopMargin;  // pixels to offset text in menu bar
}


void EditMenu::disable(int ID) {
  enablestate[ID] = false;
}

void EditMenu::enable(int ID) {
  enablestate[ID] = true;
}

bool EditMenu::getEnableState(int ID) {
  return enablestate[ID];
}

void EditMenu::SetItemValue(int ItemID, float ItemValue){
	data[ItemID] = ItemValue;
}

void EditMenu::drawMonoBitmap(int16_t x, int16_t y, const unsigned char *bitmap, uint8_t w, uint8_t h, uint16_t color) {

  uint8_t sbyte = 0;
  uint8_t byteWidth = 0;
  int jj, ii;

  byteWidth = (w + 7) / 8;

  for (jj = 0; jj < h; jj++) {
    for (ii = 0; ii < w; ii++) {
      if (ii & 7)  sbyte <<= 1;
      else sbyte   = pgm_read_byte(bitmap + jj * byteWidth + ii / 8);
      if (sbyte & 0x80) d->drawPixel(x + ii, y + jj, color);
    }
  }

}

void EditMenu::draw565Bitmap(int16_t x, int16_t y, const uint16_t *bitmap, uint8_t w, uint8_t h) {

  uint16_t offset = 0;

  int j, i;

  for (i = 0; i < h; i++) {
    for (j = 0; j < w; j++) {
      d->drawPixel(j + x, i + y, bitmap[offset]);
      offset++;
    }
  }

}

/*





*/



ItemMenu::ItemMenu(Adafruit_ILI9341 *Display) {
  d = Display;
}

void ItemMenu::init(uint16_t TextColor, uint16_t BackgroundColor,
                    uint16_t HighlightTextColor, uint16_t HighlightColor,
                    uint16_t ItemRowHeight, uint16_t MaxRow,
                    const char *TitleText, const GFXfont  &ItemFont, const GFXfont  &TitleFont) {

  itc = TextColor;
  ibc = BackgroundColor;
  ihtc = HighlightTextColor;
  ihbc = HighlightColor;
  bcolor = TextColor;
  tbt = 0;        // title bar top
  tbl = 0;        // title bar left
  tbh = 40;       // title bar height
  tbw = d->width(); // title bar width, default to screen width
  tox = 20;       // x pixel offset for title text to offset from left a bit
  toy  = 10;        // y pixel offset for title text to manage height offset from top
  isx = 0;        // where to start the menu item from left, default indent 10
  icox = 0;
  icoy = 0;
  mm = 5;
  isy = tbt + tbh - ItemRowHeight + mm;  // where to start the menu item from top, note because array is 1 based we start drawing down a row--so back off a row, but pad 10
  irh = ItemRowHeight;  // select bar height
  irw = tbw - isx;    // select bar width, default to full width
  imr = MaxRow;     // user has to indicate this
  iox = 0;        // pixels to offset text in menu bar
  ioy = 0;        // pixels to offset text in menu bar
  itemf = ItemFont;     // item font
  titlef = TitleFont;      // menu bar font, default to item font
  ttc = TextColor;    // title text color
  tfc = HighlightColor; // title fill color
  ditc = MENU_C_DKGREY;
  strncpy(ttx, TitleText, MAX_CHAR_LEN);
  strncpy(etx, EXIT_TEXT, 6);
  item = 1;
  drawTitleFlag = true; // flag if we draw menu bar again
  currentID = 1;      // id of current highlighted or selected item (1 to ID)
  cr = 1;         // current selected row on the screen (1 to mr-1)
  totalID = 0;      // maximum number of items (where first position is 1)
  sr = 0;         // draw offset for the menu array where first postion is 0
  pr = 1;         // previous selected rown (1 to mr - 1)
  rowselected = false;
  enablestate[0] = true;
  redraw = true;
  radius = 0;
  thick = 0;

}

int ItemMenu::addNI(const char *ItemLabel) {

  totalID++;

  IconType[totalID] = ICON_NONE;

  enablestate[totalID] = true;
  strncpy(itemlabel[totalID], ItemLabel, MAX_CHAR_LEN);
  return (totalID);

}

int ItemMenu::addMono(const char *ItemLabel, const unsigned char *Bitmap, uint8_t BitmapWidth, uint8_t BitmapHeight) {

  totalID++;

  itemBitmap[totalID] = Bitmap;
  bmp_w[totalID] = BitmapWidth;
  bmp_h[totalID] = BitmapHeight;
  IconType[totalID] = ICON_MONO;

  enablestate[totalID] = true;
  strncpy(itemlabel[totalID], ItemLabel, MAX_CHAR_LEN);
  return (totalID);
}

int ItemMenu::add565(const char *ItemLabel, const uint16_t *Bitmap, uint8_t BitmapWidth, uint8_t BitmapHeight) {

  totalID++;

  item565Bitmap[totalID] = Bitmap;
  bmp_w[totalID] = BitmapWidth;
  bmp_h[totalID] = BitmapHeight;
  IconType[totalID] = ICON_565;

  enablestate[totalID] = true;
  strncpy(itemlabel[totalID], ItemLabel, MAX_CHAR_LEN);
  return (totalID);
}

void ItemMenu::drawHeader(bool hl) {

  d->setFont(&titlef);
  
  if (hl) {
    // draw new menu bar
    d->setCursor(tbl + tox, tbt + toy);
    d->fillRect(tbl, tbt, tbw, tbh, ihbc);
    d->setTextColor(ihtc);
    d->print(etx);

  	d->fillTriangle( tbw - (tbh/3.0) - 5,	tbt + (tbh/2.0) - 3,
                     tbw - (tbh/6.0) - 5,	tbt + (tbh/2.0) -(tbh/3.0),
                     tbw - 5,				tbt + (tbh/2.0) - 3,
                     ihtc);

	d->fillTriangle( tbw - (tbh/3.0) - 5,	tbt + (tbh/2.0) + 3,
                     tbw - (tbh/6.0) - 5,	tbt + (tbh/2.0) +(tbh/3.0),
                     tbw - 5,				tbt + (tbh/2.0) + 3,
                     ihtc);
  }

  else {
	d->setCursor(tbl + tox, tbt + toy);
    d->fillRect(tbl, tbt, tbw, tbh, tfc);
    d->setTextColor(ttc);
    d->print(ttx);
  }

}

void ItemMenu::draw() {
  drawHeader(false);

  drawItems();
}


void ItemMenu::drawItems() {
int  bs;

  if (imr > totalID) {
    imr = totalID;
  }
  redraw = false;

  // determine if we need to pan or just increment the list
  if ((currentID >  totalID) && (sr >= 0) ) {
    // up to top
    cr = 0;
    currentID = 0;
    sr = 0;
    redraw = true;
  }

  else if ( (cr < 0) && (sr == 0) ) {
    //  pan whole menu to bottom
    cr = imr;
    currentID = totalID;
    sr = totalID - imr;
    redraw = true;
  }
  else if ( (cr > imr) && ((sr + cr) > totalID) ) {
    //  pan whole menu to top
    cr = 1;
    currentID = 1;
    sr = 0;
    redraw = true;
  }
  else if ((cr > imr) && (sr >= 0))   {
    // scroll whole list up one by one
    sr++;
    cr = imr;
    redraw = true;
  }
  else if ((cr < 1) && (sr > 0))   {
    // scroll whole list down one by one
    sr--;
    cr = 1;
    redraw = true;
  }

  // determine if we need arrows
  if (cr == imr) {
    moredown = true;
  }
  if (cr == 1) {
    moreup = true;
  }

  if (enablestate[currentID] == false) {
    return;
  }

  // determine if we need to redraw the header and draw
  if ((currentID == 0) && (cr == 0)) {
    drawHeader(true);
  }
  else if ((pr == 0) && (cr > 0)) {
    drawHeader(false);
  }

  d->setFont(&itemf);

  // now draw the items in the rows
  for (i = 1; i <= imr; i++) {


	// icon start 
	// icox;
	// menu bar start 
	bs = icox + bmp_w[i + sr] + isx;

	// text start
	itx = bs + iox;

    if (enablestate[i + sr]) {
      temptColor = itc;
    }
    else {
      temptColor = ditc;
    }

    if (redraw) {
      // scroll so blank out every row including icon since row will get scrolled
      d->fillRect(bs, isy + (irh * i), irw - bs, irh, ibc); // back color
    }

    if (i == pr) {
      // maybe just row change so blank previous
      d->fillRect(bs, isy + (irh * pr) , irw - bs, irh, ibc); // back color
    }

    if (i == cr) {

		if (radius > 0){
			d->fillRoundRect(bs,			isy + (irh * i) ,			irw - bs,				irh,				radius,		bcolor);
			d->fillRoundRect(bs + thick,	isy + (irh * i) + thick,	irw -bs - (2*thick),	irh - (2*thick),	radius,		ihbc);
		}
		else {
			d->fillRect(bs,				isy + (irh * i) ,			irw -bs  ,				irh, bcolor);
			d->fillRect(bs + thick ,	isy + (irh * i) + thick,	irw -bs - (2*thick) ,	irh - (2*thick), ihbc);
		}


      if (moreup) {
        d->fillTriangle(irw - 8 - thick,							isy + (irh * i) + irh - (irh / TRIANGLE_H),
                        irw - ((irh / TRIANGLE_W)*.5) - 8- thick,	isy + (irh * i) + (irh / TRIANGLE_H),
                        irw - (irh / TRIANGLE_W) - 8- thick,		isy + (irh * i) + irh - (irh / TRIANGLE_H), ihtc);
      }
      if (moredown) {
        d->fillTriangle(irw - 8- thick,    							isy + (irh * i) + (irh / TRIANGLE_H),
                        irw - ((irh / TRIANGLE_W)*.5) - 8- thick,	isy + (irh * i) + irh - (irh / TRIANGLE_H),
                        irw - (irh / TRIANGLE_W) - 8- thick,		isy + (irh * i) + (irh / TRIANGLE_H),     ihtc);
      }
	  temptColor = ihtc;

    }

    // write bitmap

    if (IconType[i + sr] == ICON_MONO) {
      drawMonoBitmap(icox,  icoy + isy + (irh * i), itemBitmap[i + sr], bmp_w[i + sr], bmp_h[i + sr], itc );
    }
    else if (IconType[i + sr] == ICON_565) {
      draw565Bitmap(icox,  icoy + isy + (irh * i), item565Bitmap[i + sr], bmp_w[i + sr], bmp_h[i + sr] );
    }

    // write text
    d->setTextColor(temptColor);
    d->setCursor(itx , isy + (irh * i) + ioy);
    d->print(itemlabel[i + sr]);
  }

  moreup = false;
  moredown = false;
  pr = cr;

}

void ItemMenu::MoveUp() {
  cr--;
  currentID--;
  drawItems();
  if (enablestate[currentID] == false) {
    while (enablestate[currentID] == false) {
      cr--;
      currentID--;
      drawItems();
    }
  }
}

void ItemMenu::MoveDown() {

  cr++;
  currentID++;
  drawItems();
  if (enablestate[currentID] == false) {
    while (enablestate[currentID] == false) {
      cr++;
      currentID++;
      drawItems();
    }
  }

}

int ItemMenu::selectRow() {
  if (currentID == 0) {
    cr = 1;
    sr = 0;
  }
  item = currentID;
  return currentID;
}

void ItemMenu::setTitleColors( uint16_t TitleTextColor, uint16_t TitleFillColor) {
  ttc = TitleTextColor;
  tfc = TitleFillColor;

}

void ItemMenu::setTitleBarSize( uint16_t TitleTop, uint16_t TitleLeft, uint16_t TitleWith, uint16_t TitleHeight) {

  tbt = TitleTop;
  tbl = TitleLeft;
  tbw = TitleWith;
  tbh = TitleHeight;

}

void ItemMenu::setTitleText(char *TitleText, char *ExitText) {
  strncpy(ttx, TitleText, MAX_CHAR_LEN);
  strncpy(etx, ExitText, MAX_CHAR_LEN);
}

void ItemMenu::setTitleTextMargins(uint16_t LeftMargin, uint16_t TopMargin) {
  tox = LeftMargin;   // pixels to offset text in menu bar
  toy = TopMargin;   // pixels to offset text in menu bar
}

void ItemMenu::setMenuBarMargins(uint16_t LeftMargin, uint16_t Width, byte BorderRadius, byte BorderThickness) {
  isx = LeftMargin;  // pixels to offset text in menu bar
  irw = Width - isx;  // pixels to offset text in menu bar
  radius = BorderRadius;
  thick = BorderThickness; 

}

void ItemMenu::setItemColors(uint16_t DisableTextColor, uint16_t BorderColor){
   ditc = DisableTextColor;
   bcolor = BorderColor;
}

void ItemMenu::setItemTextMargins(uint16_t LeftMargin, uint16_t TopMargin, uint16_t MenuMargin) {
  iox = LeftMargin;  // pixels to offset text in menu bar
  ioy = TopMargin;  // pixels to offset text in menu bar
  mm = MenuMargin; 
  isy = tbt + tbh - irh + mm;
}

void ItemMenu::setIconMargins(uint16_t IconOffsetX, uint16_t IconOffsetY) {
  icox = IconOffsetX; // pixels to offset text in menu bar
  icoy = IconOffsetY;  // pixels to offset text in menu bar
}

void ItemMenu::disable(int ID) {
  enablestate[ID] = false;
}

void ItemMenu::enable(int ID) {
  enablestate[ID] = true;
}

bool ItemMenu::getEnableState(int ID) {
  return enablestate[ID];
}


void ItemMenu::drawMonoBitmap(int16_t x, int16_t y, const unsigned char *bitmap, uint8_t w, uint8_t h, uint16_t color) {

  uint8_t sbyte = 0;
  uint8_t byteWidth = 0;
  int jj, ii;

  byteWidth = (w + 7) / 8;

  for (jj = 0; jj < h; jj++) {
    for (ii = 0; ii < w; ii++) {
      if (ii & 7)  sbyte <<= 1;
      else sbyte   = pgm_read_byte(bitmap + jj * byteWidth + ii / 8);
      if (sbyte & 0x80) d->drawPixel(x + ii, y + jj, color);
    }
  }

}

void ItemMenu::draw565Bitmap(int16_t x, int16_t y, const uint16_t *bitmap, uint8_t w, uint8_t h) {

  uint16_t offset = 0;

  int j, i;

  for (i = 0; i < h; i++) {
    for (j = 0; j < w; j++) {
      d->drawPixel(j + x, i + y, bitmap[offset]);
      offset++;
    }
  }

}
