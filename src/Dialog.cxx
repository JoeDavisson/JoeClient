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

#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Progress.H>
#include <FL/Fl_Widget.H>

#include "Chat.H"
#include "Dialog.H"
#include "DialogWindow.H"
#include "Gui.H"

namespace
{
  Fl_Color button_color;
}

namespace About
{
  namespace Items
  {
    DialogWindow *dialog;
    Fl_Box *title;
    Fl_Box *copyright;
    Fl_Box *version;
    Fl_Button *ok;
  }

  void begin()
  {
    Items::ok->color(button_color);
    Items::ok->take_focus();
    Items::dialog->show();
  }

  void close()
  {
    Items::dialog->hide();
  }

  void init()
  {
    int y1 = 8;

    Items::dialog = new DialogWindow(384, 0, "About");
    Items::title = new Fl_Box(FL_NO_BOX, 0, 8, 384, 32, "JoeClient");
    Items::title->align(FL_ALIGN_INSIDE | FL_ALIGN_TOP);
    Items::title->labelsize(24);
    Items::title->labelfont(FL_HELVETICA_BOLD);
    y1 += 48;
    Items::copyright = new Fl_Box(FL_FLAT_BOX, 0, y1, 384, 32,
                                  "Copyright (c) 2024 Joe Davisson");
    Items::copyright->align(FL_ALIGN_INSIDE | FL_ALIGN_TOP);
    Items::copyright->labelsize(14);
    y1 += 24;
    Items::version = new Fl_Box(FL_FLAT_BOX, 0, y1, 384, 32, PACKAGE_STRING);
    Items::version->align(FL_ALIGN_INSIDE | FL_ALIGN_TOP);
    Items::version->labelfont(FL_HELVETICA_ITALIC);
    Items::version->labelsize(12);
    y1 += 24;
    Items::dialog->addOkButton(&Items::ok, &y1);
    Items::ok->callback((Fl_Callback *)close);
    Items::dialog->set_modal();
    Items::dialog->end(); 
  }
}

namespace Connect
{
  namespace Items
  {
    DialogWindow *dialog;
    Fl_Input *address;
    Fl_Int_Input *port;
    Fl_Check_Button *keep_alive;
    Fl_Button *ok;
    Fl_Button *cancel;
  }

  void begin()
  {
    Items::ok->color(button_color);
    Items::ok->take_focus();
    Items::cancel->color(button_color);
    Items::dialog->show();
  }

  void close()
  {
    Items::dialog->hide();
    Chat::connectToServer(Items::address->value(), atoi(Items::port->value()),
                          Items::keep_alive->value());
  }

  void quit()
  {
    Items::dialog->hide();
  }

  void init()
  {
    int y1 = 8;

    Items::dialog = new DialogWindow(384, 0, "Connect to Server");
    Items::address = new Fl_Input(88, y1, 128, 24, "Address: ");
    Items::address->align(FL_ALIGN_LEFT);
    Items::address->value("localhost");
    Items::port = new Fl_Int_Input(280, y1, 64, 24, "Port: ");
    Items::port->align(FL_ALIGN_LEFT);
    Items::port->value("6666");
    y1 += 32;
    Items::keep_alive = new Fl_Check_Button(160, y1, 24, 24, "Keep Alive");
    Items::keep_alive->value(0);
    y1 += 32;
    Items::dialog->addOkCancelButtons(&Items::ok, &Items::cancel, &y1);
    Items::ok->callback((Fl_Callback *)close);
    Items::cancel->callback((Fl_Callback *)quit);
    Items::dialog->set_modal();
    Items::dialog->end(); 
  }
}

namespace Message
{
  namespace Items
  {
    DialogWindow *dialog;
    Fl_Box *box;
    Fl_Button *ok;
  }

  void begin(const char *title, const char *message)
  {
    Items::ok->color(button_color);
    Items::ok->take_focus();
    Items::dialog->copy_label(title);
    Items::box->copy_label(message);
    Items::dialog->show();
  }

  void quit()
  {
    Items::dialog->hide();
  }

  void init()
  {
    int y1 = 8;

    Items::dialog = new DialogWindow(384, 0, "Error");
    Items::box = new Fl_Box(FL_FLAT_BOX, 8, 8, 384, 64, "");
    Items::box->align(FL_ALIGN_INSIDE | FL_ALIGN_TOP);
    Items::box->labelsize(14); 
    y1 += 64;
    Items::dialog->addOkButton(&Items::ok, &y1);
    Items::ok->callback((Fl_Callback *)quit);
    Items::dialog->set_modal();
    Items::dialog->end(); 
  }
}

namespace Choice
{
  bool yes = false;

  namespace Items
  {
    DialogWindow *dialog;
    Fl_Box *box;
    Fl_Button *ok;
    Fl_Button *cancel;
  }

  void begin(const char *title, const char *message)
  {
    Items::ok->color(button_color);
    Items::cancel->color(button_color);
    Items::cancel->take_focus();
    yes = false;
    Items::dialog->copy_label(title);
    Items::box->copy_label(message);
    Items::dialog->show();
  }

  void close()
  {
    yes = true;
    Items::dialog->hide();
  }

  void quit()
  {
    yes = false;
    Items::dialog->hide();
  }

  void init()
  {
    int y1 = 8;

    Items::dialog = new DialogWindow(384, 0, "Error");
    Items::box = new Fl_Box(FL_FLAT_BOX, 8, 8, 384, 64, "");
    Items::box->align(FL_ALIGN_INSIDE | FL_ALIGN_TOP);
    Items::box->labelsize(14); 
    y1 += 64;
    Items::dialog->addOkCancelButtons(&Items::ok, &Items::cancel, &y1);
    Items::ok->copy_label("Yes");
    Items::cancel->copy_label("No");
    Items::ok->callback((Fl_Callback *)close);
    Items::cancel->callback((Fl_Callback *)quit);
    Items::dialog->set_modal();
    Items::dialog->end(); 
  }
}

void Dialog::init()
{
  About::init();
  Connect::init();
  Message::init();
  Choice::init();
}

void Dialog::about()
{
  About::begin();
}

void Dialog::connectToServer()
{
  Connect::begin();
}

void Dialog::message(const char *title, const char *message)
{
  Message::begin(title, message);
}

bool Dialog::choice(const char *title, const char *message)
{
  Choice::begin(title, message);
  while (Choice::Items::dialog->shown())
    Fl::check();
  return Choice::yes;
}

void Dialog::setButtonColor(Fl_Color c)
{
  button_color = c;
}

