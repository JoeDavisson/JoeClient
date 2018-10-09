/*
Copyright (c) 2015 Joe Davisson.

This file is part of JoeClient.

JoeClient is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

JoeClient is distributed in the hope that it will be useful,
state WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with JoeClient; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
*/

#include <cmath>
#include <typeinfo>

#include <FL/Fl_Box.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Help_View.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Tooltip.H>

#include "Chat.H"
#include "Dialog.H"
#include "Gui.H"

class MainWin;

namespace
{
  // window
  MainWin *window;

  // main menu
  Fl_Menu_Bar *menubar;

  Fl_Group *top;
  Fl_Group *top_left;
  Fl_Group *bottom;

  Fl_Text_Buffer *server_text;
  Fl_Text_Buffer *user_text;
  Fl_Text_Buffer *url_text;
  Fl_Text_Buffer *pm_text;

  Fl_Text_Display *server_display;
  Fl_Text_Display *user_display;
  Fl_Help_View *url_display;
  Fl_Text_Display *pm_display;

  Fl_Input *input_field;

  char url_color[16];

  // quit program
  void quit()
  {
    if(Dialog::choice("Quit", "Are You Sure?"))
      exit(0);
  }

  // prevent escape from closing main window
  void closeCallback(Fl_Widget *widget, void *)
  {
    if((Fl::event() == FL_KEYDOWN || Fl::event() == FL_SHORTCUT)
       && Fl::event_key() == FL_Escape)
    {
      return;
    }
    else
    {
      if(Dialog::choice("Quit", "Are You Sure?"))
        widget->hide();
    }
  }
}

// custom class to control window behavior
class MainWin : public Fl_Double_Window
{
public:
  MainWin(int w, int h, const char *label)
  : Fl_Double_Window(w, h, label)
  {
  }

  ~MainWin()
  {
  }
  
  int handle(int event)
  {
    bool shift, ctrl;

    switch(event)
    {
      case FL_FOCUS:
        return 1;
      case FL_UNFOCUS:
        return 1;
      case FL_KEYBOARD:
        // give focus to the main menu
        if(Fl::event_alt() > 0)
        {
          Gui::getMenuBar()->take_focus();
          return 0;
        }

        shift = Fl::event_shift() ? true : false;
        ctrl = Fl::event_ctrl() ? true : false;

        // misc keys
        //switch(Fl::event_key())
        //{
        //}

        return 1;
    }

    return Fl_Double_Window::handle(event);
  }
};

// initialize main gui
void Gui::init()
{
  int pos;

  // main window
  window = new MainWin(800, 600, "JoeClient");
  window->callback(closeCallback);

  // generate menu
  menubar = new Fl_Menu_Bar(0, 0, window->w(), 24);
  menubar->box(FL_THIN_UP_BOX);

  menubar->add("&Server/&Connect...", 0,
    (Fl_Callback *)Dialog::connect, 0, 0);
  menubar->add("&Server/&Disconnect", 0,
    (Fl_Callback *)Chat::disconnect, 0, FL_MENU_DIVIDER);
  menubar->add("&Server/Clear &Web Links", 0,
    (Fl_Callback *)clearURLs, 0, 0);
  menubar->add("&Server/&Clear &Private Messages", 0,
    (Fl_Callback *)clearPMs, 0, FL_MENU_DIVIDER);
  menubar->add("&Server/&Quit", 0,
    (Fl_Callback *)quit, 0, 0);


  menubar->add("&Preferences/&Theme/&Plain", 0,
    (Fl_Callback *)setPlainTheme, 0, FL_MENU_RADIO);
  menubar->add("&Preferences/&Theme/Gtk (&Light)", 0,
    (Fl_Callback *)setGtkLightTheme, 0, FL_MENU_RADIO);
  menubar->add("&Preferences/&Theme/Gtk (&Dark)", 0,
    (Fl_Callback *)setGtkDarkTheme, 0, FL_MENU_RADIO);
  menubar->add("&Preferences/&Theme/&Aqua", 0,
    (Fl_Callback *)setAquaTheme, 0, FL_MENU_RADIO);
  menubar->add("&Preferences/&Font Size/&Small", 0,
    (Fl_Callback *)setFontSmall, 0, FL_MENU_RADIO);
  menubar->add("&Preferences/&Font Size/&Medium", 0,
    (Fl_Callback *)setFontMedium, 0, FL_MENU_RADIO);
  menubar->add("&Preferences/&Font Size/&Large", 0,
    (Fl_Callback *)setFontLarge, 0, FL_MENU_RADIO);

  setMenuItem("&Preferences/&Theme/&Plain");
  setMenuItem("&Preferences/&Font Size/&Medium");

  menubar->add("&Help/&About", 0,
    (Fl_Callback *)Dialog::about, 0, 0);

  server_text = new Fl_Text_Buffer();
  user_text = new Fl_Text_Buffer();
  url_text = new Fl_Text_Buffer();
  pm_text = new Fl_Text_Buffer();

  // bottom group
  bottom = new Fl_Group(0, window->h() - 144, window->w(), 144);

  url_display = new Fl_Help_View(bottom->x(), bottom->y(),
                                    bottom->w() / 2, bottom->h());
  url_display->box(FL_UP_BOX);
  url_display->textsize(14);
//  url_display->wrap_mode(Fl_Text_Display::WRAP_AT_BOUNDS, 0);
//  url_display->buffer(url_text);

  pm_display = new Fl_Text_Display(bottom->x() + bottom->w() / 2, bottom->y(),
                                    bottom->w() / 2, bottom->h());
  pm_display->box(FL_UP_BOX);
  pm_display->textsize(14);
  pm_display->wrap_mode(Fl_Text_Display::WRAP_AT_BOUNDS, 0);
  pm_display->buffer(pm_text);

  bottom->end();

  // top group
  top = new Fl_Group(0, menubar->h(),
                     window->w(), bottom->y() - menubar->h());

  user_display = new Fl_Text_Display(menubar->w() - 128, menubar->h(),
                               128, top->h());
  user_display->box(FL_UP_BOX);
  user_display->textsize(14);
  user_display->wrap_mode(Fl_Text_Display::WRAP_AT_BOUNDS, 0);
  user_display->buffer(user_text);

  top_left = new Fl_Group(0, menubar->h(),
                     window->w() - user_display->w(), top->h());

  input_field = new Fl_Input(0, bottom->y() - 32, top_left->w(), 32);
  input_field->textsize(16);
  input_field->when(FL_WHEN_ENTER_KEY);
  input_field->callback((Fl_Callback *)sendMessage);

  server_display = new Fl_Text_Display(top_left->x(),
                                       top_left->y(),
                                       top_left->w(),
                                       top_left->h() - input_field->h());

  //server_display->wrap_mode(Fl_Text_Display::WRAP_AT_BOUNDS, 0);
  server_display->box(FL_UP_BOX);
  server_display->textsize(14);
  server_display->wrap_mode(Fl_Text_Display::WRAP_AT_BOUNDS, 0);
  server_display->buffer(server_text);

  top_left->resizable(server_display);
  top_left->end();

  top->resizable(top_left);
  top->end();

  window->size_range(512, 384, 0, 0, 0, 0, 0);
  window->resizable(top);
  window->end();

  setPlainTheme();
  clearURLs();

  // fix certain icons if using a light theme
  //if(Project::theme == Project::THEME_LIGHT)
  //{
  //}
}

// show the main program window (called after gui is constructed)
void Gui::show()
{
  window->show();
}

// draw checkmark next to a menu item
void Gui::setMenuItem(const char *s)
{
  Fl_Menu_Item *m;
  m = (Fl_Menu_Item *)menubar->find_item(s);

  if(m)
    m->set();
}

// remove checkmark from menu item
void Gui::clearMenuItem(const char *s)
{
  Fl_Menu_Item *m;
  m = (Fl_Menu_Item *)menubar->find_item(s);

  if(m)
    m->clear();
}

Fl_Double_Window *Gui::getWindow()
{
  return window;
}

Fl_Menu_Bar *Gui::getMenuBar()
{
  return menubar;
}

void Gui::append(const char *text)
{
  server_text->append(text); 

  int lines = server_text->count_lines(0, server_text->length());

  // limit scrollback buffer to 1000 lines
  while(lines > 1000)
  {
    server_text->remove(server_text->line_start(1),
                        server_text->line_end(1) + 1);
    lines--;
  }

  // scroll display to bottom
  server_display->insert_position(server_text->length());
  server_display->show_insert_position();
}

void Gui::appendUser(int line, const char *name)
{
  char buf[32];

  sprintf(buf, "[%d] %s", line, name);
  user_text->append(buf); 

  if(name[strlen(buf) - 1] != '\n')
    user_text->append("\n"); 

  int lines = user_text->count_lines(0, user_text->length());

  // limit scrollback buffer to 100 lines
  while(lines > 50)
  {
    user_text->remove(user_text->line_start(1),
                      user_text->line_end(1) + 1);
    lines--;
  }

  // scroll display to bottom
  user_display->insert_position(user_text->length());
  user_display->show_insert_position();
}

void Gui::appendURL(const char *text)
{
  url_text->append("<a href=\""); 
  url_text->append(text); 
  url_text->append("\">"); 
  url_text->append(text); 
  url_text->append("</a><br>\n"); 

//  if(text[strlen(text) - 1] != '\n')
//    url_text->append("\n"); 

  int lines = url_text->count_lines(0, url_text->length());

  // limit scrollback buffer to 50 lines
  while(lines > 50 - 1)
  {
    url_text->remove(url_text->line_start(1),
                     url_text->line_end(1) + 1);
    lines--;
  }

  // scroll display to bottom
  url_display->value(url_text->text());
  linkColor();
//  url_display->insert_position(url_text->length());
//  url_display->show_insert_position();
}

void Gui::appendPM(const char *text)
{
  pm_text->append(text); 

  if(text[strlen(text) - 1] != '\n')
    pm_text->append("\n"); 

  int lines = pm_text->count_lines(0, pm_text->length());

  // limit scrollback buffer to 100 lines
  while(lines > 100)
  {
    pm_text->remove(pm_text->line_start(1),
                    pm_text->line_end(1) + 1);
    lines--;
  }

  // scroll display to bottom
  pm_display->insert_position(pm_text->length());
  pm_display->show_insert_position();
}

void Gui::clearUsers()
{
  user_text->text("");
}

void Gui::clearURLs()
{
  url_text->text("");
  linkColor();
}

void Gui::clearPMs()
{
  pm_text->text("");
}

void Gui::linkColor()
{
  // remove first line
  url_text->remove(url_text->line_start(1),
                   url_text->line_end(1) + 1);

  // make copy of buffer text
  char *temp = url_text->text();

  url_text->text("");
  url_text->append("<body link=\"");
  url_text->append(url_color);
  url_text->append("\">\n");
  url_text->append(temp);

  free(temp);
  url_display->value(url_text->text());
}

void Gui::sendMessage()
{
  Chat::write(input_field->value());
  input_field->value("");
}

void Gui::setPlainTheme()
{
  Fl::set_color(FL_BACKGROUND_COLOR, 224, 224, 224);
  Fl::set_color(FL_BACKGROUND2_COLOR, 192, 192, 192);
  Fl::set_color(FL_FOREGROUND_COLOR, 0, 0, 0);
  Fl::set_color(FL_INACTIVE_COLOR, 128, 128, 128);
  Fl::set_color(FL_SELECTION_COLOR, 64, 64, 64);
  input_field->color(fl_rgb_color(208, 208, 208));
  Dialog::setButtonColor(fl_rgb_color(192, 192, 192));
  strcpy(url_color, "#000000");
  linkColor();
  Fl::scheme("none");
  getWindow()->redraw();
}

void Gui::setGtkLightTheme()
{
  Fl::set_color(FL_BACKGROUND_COLOR, 224, 224, 224);
  Fl::set_color(FL_BACKGROUND2_COLOR, 192, 192, 192);
  Fl::set_color(FL_FOREGROUND_COLOR, 0, 0, 0);
  Fl::set_color(FL_INACTIVE_COLOR, 128, 128, 128);
  Fl::set_color(FL_SELECTION_COLOR, 80, 80, 80);
  input_field->color(fl_rgb_color(176, 176, 176));
  Dialog::setButtonColor(fl_rgb_color(192, 192, 192));
  strcpy(url_color, "#000000");
  linkColor();
  Fl::scheme("gtk+");
  getWindow()->redraw();
}

void Gui::setGtkDarkTheme()
{
  Fl::set_color(FL_BACKGROUND_COLOR, 80, 80, 80);
  Fl::set_color(FL_BACKGROUND2_COLOR, 64, 64, 64);
  Fl::set_color(FL_FOREGROUND_COLOR, 248, 248, 248);
  Fl::set_color(FL_INACTIVE_COLOR, 128, 128, 128);
  Fl::set_color(FL_SELECTION_COLOR, 224, 224, 224);
  input_field->color(fl_rgb_color(56, 56, 56));
  Dialog::setButtonColor(fl_rgb_color(96, 96, 96));
  strcpy(url_color, "#ffffff");
  linkColor();
  Fl::scheme("gtk+");
  getWindow()->redraw();
}

void Gui::setAquaTheme()
{
  Fl::set_color(FL_BACKGROUND_COLOR, 224, 224, 224);
  Fl::set_color(FL_BACKGROUND2_COLOR, 208, 208, 208);
  Fl::set_color(FL_FOREGROUND_COLOR, 0, 0, 0);
  Fl::set_color(FL_INACTIVE_COLOR, 192, 192, 192);
  Fl::set_color(FL_SELECTION_COLOR, 255, 255, 255);
  input_field->color(fl_rgb_color(208, 208, 208));
  Dialog::setButtonColor(fl_rgb_color(255, 255, 255));
  strcpy(url_color, "#000000");
  linkColor();
  Fl::scheme("plastic");
  getWindow()->redraw();
}

void Gui::setFontSmall()
{
  server_display->textsize(12);
  server_display->buffer(0);
  server_display->buffer(server_text);
  server_display->redraw();
}

void Gui::setFontMedium()
{
  server_display->textsize(14);
  server_display->buffer(0);
  server_display->buffer(server_text);
  server_display->redraw();
}

void Gui::setFontLarge()
{
  server_display->textsize(16);
  server_display->buffer(0);
  server_display->buffer(server_text);
  server_display->redraw();
}

