/*
Copyright (c) 2015 Joe Davisson.

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

#include "FL/Fl.H"

#include "Chat.H"
#include "Dialog.H"
#include "Gui.H"

int main(int argc, char *argv[])
{
  // fltk related inits
  Fl::visual(FL_DOUBLE | FL_RGB);

  // program inits
  Dialog::init();
  Gui::init();

  // delay showing main gui until after all arguments are checked
  Gui::show();

  return Fl::run();
}

