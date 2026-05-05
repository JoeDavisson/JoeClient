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

#ifndef STYLEDTEXT_H
#define STYLEDTEXT_H

#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Text_Display.H>

class StyledText : public Fl_Group
{
public:
  StyledText(int, int, int, int, int);
  ~StyledText();

  void append(const char *, const char *, const char *, const char, const char);
  void append(const char *);
  void clear();
  void setFontSize(const int);
  void bgColor(const Fl_Color);

private:
  Fl_Text_Display *text_display;
  Fl_Text_Buffer *text_buf;
  Fl_Text_Buffer *style_buf;
  int scrollback_limit;
};

#endif

