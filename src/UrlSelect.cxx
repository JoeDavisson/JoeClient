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

#include <FL/fl_draw.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Select_Browser.H>

#include "Gui.H"
#include "UrlSelect.H"

UrlSelect::UrlSelect(int x, int y, int w, int h, const char *label)
          : Fl_Select_Browser(x, y, w, h, label)
{
  line = 0;
  old_line = 0;
  can_click = false;
  type(0);
}

UrlSelect::~UrlSelect()
{
}

int UrlSelect::handle(int event)
{
  void *p = 0;
  int width = 0;

  switch (event)
  {
    case FL_DRAG:
      Fl_Window::current()->cursor(FL_CURSOR_DEFAULT);
      return 1;
    case FL_RELEASE:
      return Fl_Select_Browser::handle(event);
    case FL_MOVE:
    case FL_ENTER:
      old_line = line;
      p = find_item(Fl::event_y());
      line = 0;

      if (p)
      {
        int index = 1;

        for (void *i = item_first(); i; i = item_next(i))
        {
          if (i == p)
          {
            line = index;
            break;
          }

          index++;
        }
      }

      if (line > 0 && line <= size())
      {
        width = item_width(item_at(line));
        width -= hscrollbar.value();

        if (/*(scrollbar.visible() != 0 &&*/ width >= w() - scrollbar_size())
        {
          width = w() - scrollbar_size();
        }
      
        if (/*scrollbar.visible() && */
            Fl::event_inside(&hscrollbar) == 0 &&
            Fl::event_inside(&scrollbar) == 0 &&
            Fl::event_x() > x() &&
            Fl::event_x() < width)
        {
          Gui::getWindow()->cursor(FL_CURSOR_HAND);
          can_click = true;
          redraw();
        }
          else
        {
          Gui::getWindow()->cursor(FL_CURSOR_DEFAULT);
          can_click = false;
          redraw();
        }
      }
        else
      {
        Gui::getWindow()->cursor(FL_CURSOR_DEFAULT);
        can_click = false;
        redraw();
      }

      if (line != old_line)
        redraw();

      return 1;
    case FL_LEAVE:
      can_click = false;
      Fl_Window::current()->cursor(FL_CURSOR_DEFAULT);
      line = 0;
      redraw();
      return 1;
  }

  return Fl_Select_Browser::handle(event);
}

void UrlSelect::draw()
{
  Fl_Select_Browser::draw();

  if (can_click == false)
    return;

  if (line > 0 && line <= size())
  {
    int width = item_width(item_at(line));
    int height = item_height(item_at(line));

    width -= hscrollbar.value();

    if (scrollbar.visible() != 0 && width >= w() - scrollbar_size())
    {
      width = w() - scrollbar_size();
    }
      
    int ypos = (line - topline() + 1) * height
                     - (vposition() % height) - 2;


    if (ypos < h() - (hscrollbar.visible() != 0 ? scrollbar_size() : 0))
    {
      fl_rectf(x(), y() + ypos, width, 2, fl_rgb_color(128, 128, 128));
    }
  }
}

bool UrlSelect::canClick()
{
  return can_click;
}

