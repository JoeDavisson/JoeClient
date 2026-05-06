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
#include "UrlBrowse.H"

class Url_Select_Browser : public Fl_Select_Browser
{
public:
  Url_Select_Browser(int x, int y, int w, int h, const char *label)
         : Fl_Select_Browser(x, y, w, h, label)
  {
  }

  ~Url_Select_Browser() { }

  int line = 0;
  int old_line = 0;
  int dy = 0;
  void *p = 0;

  int handle(int event)
  {
    switch (event)
    {
      case FL_DRAG:
        Fl_Window::current()->cursor(FL_CURSOR_DEFAULT);
        return 1;
      case FL_PUSH:
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
          Fl_Window::current()->cursor(FL_CURSOR_HAND);
        }
          else
        {
          Fl_Window::current()->cursor(FL_CURSOR_DEFAULT);
        }

        if (line != old_line)
          redraw();

        return 1;
      case FL_LEAVE:
        Fl_Window::current()->cursor(FL_CURSOR_DEFAULT);
        line = 0;
        redraw();
        return 1;
    }

    return Fl_Select_Browser::handle(event);
  }

  void draw()
  {
    Fl_Select_Browser::draw();

    if (line > 0 && line <= size())
    {
      const int width = item_width(item_at(line));
      const int height = item_height(item_at(line));
      const int ypos = (line - topline() + 1) * height - vposition() % height - 2;

      fl_rectf(x(), y() + ypos, width, 2, fl_rgb_color(128, 128, 128));
    }
  }
};

UrlBrowse::UrlBrowse(int x, int y, int w, int h)
: Fl_Group(x, y, w, h, 0)
{
  box(FL_FLAT_BOX);
  url_browse = new Url_Select_Browser(x + 4, y + 4, w - 8, h - 8, 0);
  url_browse->box(FL_FLAT_BOX);
  url_browse->textsize(16);
  this->end();
}

UrlBrowse::~UrlBrowse()
{
  delete url_browse;
}

void UrlBrowse::add(const char * text)
{
  url_browse->add(text);
}

void UrlBrowse::bgColor(const Fl_Color c)
{
  this->color(c);
  url_browse->color(c);
}

void UrlBrowse::bottomline(const int line)
{
  url_browse->bottomline(line);
}

void UrlBrowse::callback(Fl_Callback *cb)
{
  url_browse->callback(cb);
}

void UrlBrowse::clear()
{
  url_browse->clear();
}

int UrlBrowse::size()
{
  return url_browse->size();
}

const char *UrlBrowse::text(const int line)
{
  return url_browse->text(line);
}

void UrlBrowse::textsize(const int size)
{
  url_browse->textsize(size);
}

int UrlBrowse::value()
{
  return url_browse->value();
}

void UrlBrowse::resize(int x, int y, int w, int h)
{
  Fl_Group::resize(x, y, w, h);
  url_browse->resize(x + 4, y + 4, w - 8, h - 8);
}

