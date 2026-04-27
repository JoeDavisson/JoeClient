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

#include <cstdio>
#include <cstring>
#include <vector>

#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Text_Display.H>

#include "StyledText.H"

namespace
{
  Fl_Text_Buffer *text_buf;
  Fl_Text_Buffer *style_buf;

  Fl_Text_Display::Style_Table_Entry style_table[] =
  {
    { 0x00000000, FL_HELVETICA, 16 },
    { 0x77777700, FL_HELVETICA_ITALIC, 16 },
    { 0x00000000, FL_HELVETICA_BOLD, 16 },
    { 0x77777700, FL_HELVETICA_BOLD_ITALIC, 16 },
    { 0x77777700, FL_HELVETICA_ITALIC, 16 },
    { 0x00000000, FL_HELVETICA, 16 },
    { 0x00000000, FL_HELVETICA, 16 }
  };

  const int style_table_size = sizeof(style_table) / sizeof(style_table[0]);
  int scrollback_limit = 1000;
}

StyledText::StyledText(int x, int y, int w, int h, int limit)
: Fl_Text_Display(x, y, w, h, 0)
{
  text_buf = new Fl_Text_Buffer();
  text_buf->canUndo(0);
  style_buf = new Fl_Text_Buffer();
  style_buf->canUndo(0);
  wrap_mode(Fl_Text_Display::WRAP_AT_BOUNDS, 0);
  scrollbar_size(16);
  buffer(text_buf);
  highlight_data(style_buf, style_table, style_table_size, 'A', 0, 0);
  scrollback_limit = limit;
}

StyledText::~StyledText()
{
}

void StyledText::append(const char *text,
                        const char *match1,
                        const char *match2,
                        const char style1,
                        const char style2)
{
  const int utf_len = strlen(text);
  char buf[utf_len + 1];
  int index = 0;
  char current_style = style1;

  const char *p1 = std::strstr(text, match1);
  const char *p2 = std::strstr(text, match2);
  int index1 = -1;
  int index2 = -1;

  if (p1)
  {
    index1 = p1 - text;
    index1 += strlen(match1);
  }
  
  if (p2)
  {
    index2 = p2 - text;
    index2 += strlen(match2);
  }

  while (true)
  {
    int len = fl_utf8len1(text[index]);

    if (text[index] == '\n' || text[index] == '\r')
    {
      buf[index] = '\n';
      index++;
      break;
    }
      else
    {
      if (index == index1)
      {
        current_style = style1;
      }
      else if (index == index2)
      {
        current_style = style2;
      }
    }

    buf[index] = current_style;

    index += len;

    if (index >= utf_len)
    {
      index = utf_len;
      break;
    }
  }

  buf[index] = '\0';
  text_buf->append(text);
  style_buf->append(buf);

  int lines = text_buf->count_lines(0, text_buf->length());

  while (lines > scrollback_limit)
  {
    const int num = text_buf->line_end(1);

    text_buf->remove(0, num);
    style_buf->remove(0, num);
    lines--;
  }

  // scroll display to bottom
  insert_position(text_buf->length());
  show_insert_position();
}

void StyledText::remove(const int start, const int end)
{
  text_buf->remove(start, end);
  style_buf->remove(start, end);
}

void StyledText::setFontSize(const int size)
{
  for (int i = 0; i < style_table_size; i++)
  { 
    style_table[i].size = size;
  } 

  highlight_data(style_buf, style_table, style_table_size, 'A', 0, 0);
}



