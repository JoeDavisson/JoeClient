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

#include "Language.H"

namespace
{
  const char *english[64] =
  {
    "Server/Connect",
    "Server/Disconnect",
    "Server/Clear Web Links",
    "Server/Clear Private Messages",
    "Server/Quit",
    "Preferences/Theme/Light",
    "Preferences/Theme/Dark",
    "Preferences/Font Size/Small",
    "Preferences/Font Size/Medium",
    "Preferences/Font Size/Large",
    "Help",
    "About",
    "Help/About",
    "Connect To Server",
    "Address",
    "Port",
    "Keep Alive",
    "Quit",
    "Are You Sure?",
    "Ok",
    "Cancel",
    "Yes",
    "No"
  };

  const char *german[64] =
  {
    "Der Server/Verbinden",
    "Der Server/Trennen",
    "Der Server/Links zum Clear Web",
    "Der Server/Private Nachrichten Löschen",
    "Der Server/Aufhören",
    "Einstellungen/Thema/Licht",
    "Einstellungen/Thema/Dunkel",
    "Einstellungen/Schriftgröße/Klein",
    "Eserverinstellungen/Schriftgröße/Medium",
    "Einstellungen/Schriftgröße/Groß",
    "Helfen",
    "Über",
    "Helfen/Über",
    "Verbindung Zum Server Herstellen",
    "Adresse",
    "Port",
    "Keep-Alive",
    "Aufhören",
    "Bist Du Sicher?",
    "Ok",
    "Abbrechen",
    "Ja",
    "Nein"
  };

  const char *swedish[64] =
  {
    "Server/Ansluta",
    "Server/Stänga Av",
    "Server/Tydliga Webblänkar",
    "Server/Rensa Privata Meddelanden",
    "Server/Lämna",
    "Preferenser/Thema/Licht",
    "Preferenser/Thema/Dunkel",
    "Preferenser/Fontstorlek/Liten",
    "Preferenser/Fontstorlek/Medium",
    "Preferenser/Fontstorlek/Stor",
    "Hjälp",
    "Om",
    "Hjälp/Om",
    "Ansluta Till Servern",
    "Adress",
    "Port",
    "Keep-Alive",
    "Lämna",
    "Är du säker?",
    "Ok",
    "Avboka",
    "Ja",
    "Nej"
  };

  char **text = (char **)english;
}

void Language::set(const int x)
{
  switch (x)
  {
    case ENGLISH:
      text = (char **)english;
      break;
    case GERMAN:
      text = (char **)german;
      break;
    case SWEDISH:
      text = (char **)swedish;
      break;
  }
}

const char *Language::get(const int x)
{
  return text[x];
}

