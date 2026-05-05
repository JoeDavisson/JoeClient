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
}

StyledText::StyledText(int x, int y, int w, int h, int limit)
: Fl_Group(x, y, w, h, 0)
{
  box(FL_NO_BOX);
  text_display = new Fl_Text_Display(x + 4, y + 4, w - 8, h - 8);
  text_display->box(FL_NO_BOX);
  text_buf = new Fl_Text_Buffer();
  text_buf->canUndo(0);
  style_buf = new Fl_Text_Buffer();
  style_buf->canUndo(0);
  text_display->wrap_mode(Fl_Text_Display::WRAP_AT_BOUNDS, 0);
  text_display->scrollbar_size(16);
  text_display->textsize(16);
  text_display->buffer(text_buf);
  text_display->highlight_data(style_buf, style_table, style_table_size,
                               'A', 0, 0);
  scrollback_limit = limit;
  this->end();
}

StyledText::~StyledText()
{
  delete text_display;
  delete text_buf;
  delete style_buf;
}

// plain text version
void StyledText::append(const char *text)
{
  append(text, "", "", 'A', 'A');
}

// styled version
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

      buf[index] = current_style;

      if (len > 1)
      {
        for (int i = 1; i < len; i++)
        {
          buf[index + i] = current_style;
        }
      }
    }

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

  int lines = text_buf->count_lines(0, text_buf->length() - 1);

  while (lines > scrollback_limit)
  {
    const int num = text_buf->line_end(1);

    text_buf->remove(0, num);
    style_buf->remove(0, num);
    lines--;
  }

  // scroll display to bottom
  text_display->insert_position(text_buf->length());
  text_display->show_insert_position();
}

void StyledText::clear()
{
  text_buf->text("");
  style_buf->text("");
}

void StyledText::setFontSize(const int size)
{
  for (int i = 0; i < style_table_size; i++)
  { 
    style_table[i].size = size;
  } 

  text_display->highlight_data(style_buf, style_table, style_table_size,
                               'A', 0, 0);
}

void StyledText::bgColor(const Fl_Color c)
{
  this->color(c);
  text_display->color(c);
}




