/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is Hunspell, based on MySpell.
 *
 * The Initial Developers of the Original Code are
 * Kevin Hendricks (MySpell) and Németh László (Hunspell).
 * Portions created by the Initial Developers are Copyright (C) 2002-2005
 * the Initial Developers. All Rights Reserved.
 *
 * Contributor(s): David Einstein, Davide Prina, Giuseppe Modugno,
 * Gianluca Turconi, Simon Brouwer, Noll János, Bíró Árpád,
 * Goldman Eleonóra, Sarlós Tamás, Bencsáth Boldizsár, Halácsy Péter,
 * Dvornik László, Gefferth András, Nagy Viktor, Varga Dániel, Chris Halls,
 * Rene Engelhard, Bram Moolenaar, Dafydd Jones, Harri Pitkänen
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <ctype.h>

#include "../hunspell/csutil.hxx"
#include "manparser.hxx"

#ifndef W32
using namespace std;
#endif

ManParser::ManParser() {}

ManParser::ManParser(const char* wordchars) {
  init(wordchars);
}

ManParser::ManParser(const w_char* wordchars, int len) {
  init(wordchars, len);
}

ManParser::~ManParser() {}

char* ManParser::next_token() {
  for (;;) {
    switch (state) {
      case 1:  // command arguments
        if (line[actual][head] == ' ')
          state = 2;
        break;
      case 0:  // dot in begin of line
        if (line[actual][0] == '.') {
          state = 1;
          break;
        } else {
          state = 2;
        }
      // no break
      case 2:  // non word chars
        if (is_wordchar(line[actual].c_str() + head)) {
          state = 3;
          token = head;
        } else if ((line[actual][head] == '\\') &&
                   (line[actual][head + 1] == 'f') &&
                   (line[actual][head + 2] != '\0')) {
          head += 2;
        }
        break;
      case 3:  // wordchar
        if (!is_wordchar(line[actual].c_str() + head)) {
          state = 2;
          char* t = alloc_token(token, &head);
          if (t)
            return t;
        }
        break;
    }
    if (next_char(line[actual].c_str(), &head)) {
      state = 0;
      return NULL;
    }
  }
}
