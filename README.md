<b><h2><center>Version 1.0 updated 1/11/2021</center></h1></b>
1. Initial library posting

<b><h2><center>Menu System for ILI9341 displays and mechanical input buttons</center></h1></b>

This library will simplify creating and implementing a menu system for FTF displays that are use the ILI9341_t3 library and Teensy(R) microcontrollers. 
This library includes two menu objects

  1) A simple selection menu (ItemMenu) where the user can scroll up/down and select
  an item and some programmer defined action can be performed such as drawing a screen or activating a sub menu
  2) An menu with in-line menu (EditMenu) where the user can scroll up/down the list and select an item
  however the difference is that move/up down on a selected item will scroll the values stored with that
  menu item and NOT scroll the list, selecing a selected item will restore control to moving up/down the menu list

  highlights
  <ul> 1. menu items exceeds screen size but library handles wrapping</ul> 
  <ul>2. each item selection has a range, increment and decimal readout</ul> 
  <ul>3. items can also be read from a char array</ul> 
  <br>
  <br>
While this library was created for the ILI9341_t3 and intended for Teensy-based MCU's, the library may be adaptable to other display drivers. Note the driver must use the Adafruit_GFX libraries. To allow other display drivers, modify the lines in the .h and .cpp files to allow a different driver to be passed in. For example change call such as SliderH(ILI9341_t3 *disp,... to SliderH(Adafruit_RA8875 *disp,...
<br>
<br>

![header image](https://raw.github.com/KrisKasprzak/ILI9341_t3_Menu/master/Images/0003.jpg)
![header image](https://raw.github.com/KrisKasprzak/ILI9341_t3_Menu/master/0006.jpg)
![header image](https://raw.github.com/KrisKasprzak/ILI9341_t3_Menu/master/0007.jpg)
![header image](https://raw.github.com/KrisKasprzak/ILI9341_t3_Menu/master/0021.jpg)
![header image](https://raw.github.com/KrisKasprzak/ILI9341_t3_Menu/master/0023.jpg)
![header image](https://raw.github.com/KrisKasprzak/ILI9341_t3_Menu/master/0044.jpg)
![header image](https://raw.github.com/KrisKasprzak/ILI9341_t3_Menu/master/0073.jpg)
![header image](https://raw.github.com/KrisKasprzak/ILI9341_t3_Menu/master/0074.jpg)

<br>
<br>

I have included an example that shows how to create multi-menu system with both a simple select meny that call editabel menus
<br>
![header image](https://raw.github.com/KrisKasprzak/ILI9341_t3_Menu/master/0003.jpg)
![header image](https://raw.github.com/KrisKasprzak/ILI9341_t3_Menu/master/0073.jpg)

<br>
<br>
<b><h2><center>Revision history</center></h1></b>
<br>
1.0		initial code<br>
2.0		added shapes and sizes for handles<br>
3.0		fixed some repaint issues in CGraph, added marker support, added Button class with tons of methods<br>
4.0		fixed bugs added Button, Checkbox, OptionButton classes<br>
5.0		modified sliders, option and check to return true/false if pressed, and actual value stored in value property<br>
5.1		added automatic "blank out" old handle support insided draw() method in sliderH and SliderV (really needed when a slide is redrawn based on input other than a finger slide (encoder)<br>
5.4		Added ring sliders <br>
