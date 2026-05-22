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

#ifndef LANGUAGE_H
#define LANGUAGE_H

class Language
{
public:
  enum
  {
    ENGLISH,
    GERMAN,
    SWEDISH
  };

  enum
  {
    SERVER_CONNECT,
    SERVER_DISCONNECT,
    SERVER_CLEAR_WEB_LINKS,
    SERVER_CLEAR_PRIVATE_MESSAGES,
    SERVER_QUIT,
    PREFERENCES_THEME_LIGHT,
    PREFERENCES_THEME_DARK,
    PREFERENCES_FONT_SIZE_SMALL,
    PREFERENCES_FONT_SIZE_MEDIUM,
    PREFERENCES_FONT_SIZE_LARGE,
    HELP,
    ABOUT,
    HELP_ABOUT,
    CONNECT_TO_SERVER,
    CONNECT_ADDRESS,
    CONNECT_PORT,
    CONNECT_KEEP_ALIVE,
    QUIT,
    ARE_YOU_SURE,
    OK,
    CANCEL,
    YES,
    NO
  };

  static void set(const int);
  static const char *get(const int);

private:
  Language() { }
  ~Language() { }
};

#endif

