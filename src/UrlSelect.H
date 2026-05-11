/*
Copyright (c) 2026 Joe Davisson.

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

#ifndef URL_SELECT_H
#define URL_SELECT_H

#include <FL/Fl_Select_Browser.H>

class UrlSelect : public Fl_Select_Browser
{
public:
  UrlSelect(int, int, int, int, const char *);
  ~UrlSelect();

  int handle(int);
  void draw();
  bool canClick();

private:
  int line = 0;
  int old_line = 0;
  bool can_click = false;
};

#endif

