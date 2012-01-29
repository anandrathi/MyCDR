
//
// Little Hex Library V0.9
//
// Copyright (C) 2003, 2004 Ingo Naumann
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

#include <assert.h>
#include "lhexlib.h"

unsigned long int hex_string_to_ulint(string s)

{
	int l = s.length();

	if (l == 1) { 
		char k;
		string::iterator kit;
		kit = s.begin();
		k = *kit;
		return (unsigned long int)(hex1_to_int(k)); }
	 else { if (l == 2) { 
		char k[2];
		s.copy(k,2);
		return (unsigned long int)(hex2_to_int(k)); }
	 else { if (l == 3) { 
		char k[4];
		s = "0" + s;
		s.copy(k,4);
		return (unsigned long int)(hex4_to_ulint(k)); }
	 else { if (l == 4) { 
		char k[4];
		s.copy(k,4);
		return (unsigned long int)(hex4_to_ulint(k)); }
	 else { if (l == 5) { 
		char k[8];
		s = "000" + s;
		s.copy(k,8);
		return hex8_to_ulint(k); }
	 else { if (l == 6) { 
		char k[8];
		s = "00" + s;
		s.copy(k,8);
		return hex8_to_ulint(k); }
	 else { if (l == 7) { 
		char k[8];
		s = "0" + s;
		s.copy(k,8);
		return hex8_to_ulint(k); }
	 else { if (l == 8) { 
		char k[8];
		s.copy(k,8);
		return hex8_to_ulint(k); }
	 else { return 0; }
	}}}}}}};
}

unsigned long int hex8_to_ulint(char s[8])

{
	unsigned long int a,b;
	char k[4];

	k[0] = s[0]; k[1] = s[1];
	k[2] = s[2]; k[3] = s[3];
	a = hex4_to_ulint(k);
	k[0] = s[4]; k[1] = s[5];
	k[2] = s[6]; k[3] = s[7];
	b = hex4_to_ulint(k);

	return (unsigned long int)(((a << 16) & 0xFFFF0000) + b);
}

unsigned long int hex4_to_ulint(char s[4])

{
	unsigned long int a,b;
	char k[2];

	k[0] = s[0]; k[1] = s[1];
	a = (unsigned long int)(hex2_to_int(k));
	k[0] = s[2]; k[1] = s[3];
	b = (unsigned long int)(hex2_to_int(k));

	return (unsigned long int)(((a << 8) & 0xFF00) + b);
}

int hex2_to_int(char s[2])

{
	int a,b;
	
	a = hex1_to_int(s[0]);
	b = hex1_to_int(s[1]);

	if ((a == (-1)) || (b == (-1))) { return (int)(-1); }
	else 
		{
		assert((a >= 0) && (a < 16) && (b >= 0) && (b < 16));
		return (int)(((a << 4) & 0xF0) + b);
		};
}

int hex1_to_int(char s)

{
	if ((s >= 48) && (s <= 57)) { return (int)(s - 48); }
	  else if ((s >= 65) && (s <= 70)) { return (int)(s - 55); }
	    else if ((s >= 97) && (s <= 102)) { return (int)(s - 87); }
	      else return (int)(-1);
}
		
string lhexlib_digit_hex_array[32] = {"0","1","2","3","4","5","6","7","8","9","A","B","C","D","E","F"};

// Should be renamed into "digit_to_hex1"
string digit_to_hex_string(int digit)

//! Converts one decimal or hexadecimal digit (0...15) into a 
//! string ("0", .... "F") (hex string)

{
	string result;
	if ((digit >= 0) && (digit <=15)) { 
		result = lhexlib_digit_hex_array[digit];
		return result;
	}
	else {
		return "?"; 
	};
}

string byte_to_hex2(unsigned char value)

//! Converts byte into hex string

{
	string res1, res2;

	res1 = digit_to_hex_string((value >> 4) & 0x0F);
	res2 = digit_to_hex_string(value & 0x0F);

	return res1 + res2;
}

string uint_to_hex4(unsigned int value)

//! Converts unsigned integer into hex string (4 bytes)

{
	string res1, res2;

	res1 = byte_to_hex2((unsigned char)((value >> 8) & 0x0FF));
	res2 = byte_to_hex2((unsigned char)(value & 0x0FF));

	return res1 + res2;
}

string ulint_to_hex8(unsigned long int value)

//! Converts unsigned long integer into hex string (8 bytes)

{
	string res1, res2;

	res1 = uint_to_hex4((unsigned int)((value >> 16) & 0x0FFFF));
	res2 = uint_to_hex4((unsigned int)(value & 0x0FFFF));

	return res1 + res2;
}

/*
string ulint_to_hex16(unsigned long int value)

//! Converts unsigned long integer into hex string (16 bytes)

{
	string res1, res2;

	res1 = uint_to_hex8((value >> 32) & 0x0FFFFFFFF);
	res2 = uint_to_hex8(value & 0x0FFFFFFFF);

	return res1 + res2;
}
*/

string ulint_to_string(unsigned long int value)

{
	string result;
        int lsd;
	unsigned long int valc; 

	result = "";
	valc   = value;


	do {
		lsd = valc % 10; // Least significant digit
		valc = valc / 10; // Prepare for next digit

		result = digit_to_hex_string(lsd) + result; // Append to the left
	} while (valc > 0);



	return result;
}
