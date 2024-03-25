/*
Copyright (c) 2024 Joe Davisson.

This file is part of JoeClient.

JoeClient is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

JoeClient is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with JoeClient; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
*/

#ifndef GUI_H
#define GUI_H

class Fl_Double_Window;
class Fl_Menu_Bar;

class Gui
{
public:
  static void init();
  static void show();
  static void setMenuItem(const char *);
  static void clearMenuItem(const char *);
  static Fl_Double_Window *getWindow();
  static Fl_Menu_Bar *getMenuBar();
  static void append(const char *);
  static void appendUser(int, const char *);
  static void appendURL(const char *);
  static void appendPM(const char *);
  static void clearUsers();
  static void clearURLs();
  static void clearPMs();
  static void linkColor();
  static void sendMessage();
  static void setPlainTheme();
  static void setGtkLightTheme();
  static void setGtkDarkTheme();
  static void setAquaTheme();
  static void setFontSmall();
  static void setFontMedium();
  static void setFontLarge();

private:
  Gui() { }
  ~Gui() { }
};

#endif

