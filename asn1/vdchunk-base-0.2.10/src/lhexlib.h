  
//
// Little Hex and Conversion Library V0.9.1
//
// Copyright (C) 2003, 2004, 2010 Ingo Naumann
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 2 of
// the License, or (at your option) any later version.
//
// This program distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public
// License along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA

#ifndef LHEXLIB
#define LHEXLIB

#include <string>

using namespace std;

unsigned long int hex_string_to_ulint(string s);
unsigned long int hex8_to_ulint(char s[8]);
unsigned long int hex4_to_ulint(char s[4]);
int hex2_to_int(char s[2]);
int hex1_to_int(char s);

string digit_to_hex_string(int digit);
string byte_to_hex2(unsigned char value);
string uint_to_hex4(unsigned int value);
string ulint_to_hex8(unsigned long int value);
// string ulint_to_hex16(unsigned long int value);

string ulint_to_string(unsigned long int value);

#endif
