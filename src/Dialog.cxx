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

#include <cstdlib>

#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Flex.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Progress.H>
#include <FL/Fl_Widget.H>

#include "Chat.H"
#include "CheckBox.H"
#include "Dialog.H"
#include "DialogWindow.H"
#include "Language.H"

namespace
{
  Fl_Color button_color;
}

namespace About
{
  namespace Items
  {
    DialogWindow *dialog;
    Fl_Flex *flex;
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
    Items::dialog = new DialogWindow(384, 128,
                                     Language::get(Language::ABOUT));
    Items::flex = new Fl_Flex(8, 8,
                              Items::dialog->w() - 16, Items::dialog->h() - 16,
                              Fl_Flex::VERTICAL);
    Items::title = new Fl_Box(FL_NO_BOX, 0, 0, 0, 0, "JoeClient");
    Items::title->align(FL_ALIGN_INSIDE | FL_ALIGN_TOP);
    Items::title->labelsize(24);
    Items::title->labelfont(FL_HELVETICA_BOLD);

    Items::flex->gap(8);
    Items::copyright = new Fl_Box(FL_FLAT_BOX, 0, 0, 0, 0,
                                  "Copyright (c) 2026 Joe Davisson");
    Items::copyright->align(FL_ALIGN_INSIDE | FL_ALIGN_TOP);
    Items::copyright->labelsize(16);
    Items::version = new Fl_Box(FL_FLAT_BOX, 0, 0, 0, 0, PACKAGE_STRING);
    Items::version->align(FL_ALIGN_INSIDE | FL_ALIGN_TOP);
    Items::version->labelsize(16);
    Items::flex->end();

    int y1 = Items::flex->h();
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
    Fl_Flex *flex1;
    Fl_Flex *flex2;
    Fl_Input *address;
    Fl_Int_Input *port;
    CheckBox *enable_ssl;
    CheckBox *keep_alive;
    Fl_Button *ok;
    Fl_Button *cancel;
  }

  void begin()
  {
    if (Chat::isConnected() == true)
      return;

    Items::ok->color(button_color);
    Items::ok->take_focus();
    Items::cancel->color(button_color);
    Items::dialog->show();
  }

  void close()
  {
    Items::dialog->hide();
    Chat::connectToServer(Items::address->value(),
                          atoi(Items::port->value()),
                          Items::enable_ssl->value(),
                          Items::keep_alive->value());
  }

  void quit()
  {
    Items::dialog->hide();
  }

  void init()
  {
    Items::dialog = new DialogWindow(480, 300,
                                   Language::get(Language::CONNECT_TO_SERVER));

    Items::flex1 = new Fl_Flex(8, 32,
                              Items::dialog->w() - 16, 32,
                              Fl_Flex::HORIZONTAL);
    Items::flex1->gap(8);
    Items::address = new Fl_Input(0, 0, 0, 0,
                                  Language::get(Language::CONNECT_ADDRESS));
    Items::address->align(FL_ALIGN_TOP_LEFT);
    Items::address->value("localhost");
    Items::address->textsize(20);
    Items::address->labelsize(18);
    Items::port = new Fl_Int_Input(0, 0, 0, 0,
                                   Language::get(Language::CONNECT_PORT));
    Items::port->align(FL_ALIGN_TOP_LEFT);
    Items::port->value("6666");
    Items::port->textsize(20);
    Items::port->labelsize(18);
    Items::flex1->end();

    Items::flex2 = new Fl_Flex(8, Items::flex1->y() + Items::flex1->h() + 16,
                              Items::dialog->w() - 16, 32,
                              Fl_Flex::HORIZONTAL);
    Items::enable_ssl = new CheckBox(Items::dialog, 0, 0, 0, 0, "SSL", 0);
    Items::enable_ssl->value(0);
    Items::enable_ssl->labelsize(18);
    Items::keep_alive = new CheckBox(Items::dialog,
                                 0, 0, 0, 0,
                                 Language::get(Language::CONNECT_KEEP_ALIVE),
                                 0);
    Items::keep_alive->value(0);
    Items::keep_alive->labelsize(18);
    Items::flex2->end();

    int y1 = Items::flex2->y() + Items::flex2->h() + 16;
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
    Items::box->labelsize(16); 
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
    Items::box->labelsize(16); 
    y1 += 64;
    Items::dialog->addOkCancelButtons(&Items::ok, &Items::cancel, &y1);
    Items::ok->copy_label(Language::get(Language::YES));
    Items::cancel->copy_label(Language::get(Language::NO));
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
  {
    Fl::check();
  }

  return Choice::yes;
}

void Dialog::setButtonColor(Fl_Color c)
{
  button_color = c;
}

