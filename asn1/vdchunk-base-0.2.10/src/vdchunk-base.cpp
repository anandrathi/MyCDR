
// VDChunk -- The Versatile Data Chunk
//
// Copyright (C) 2005-2010 Ingo Naumann
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
//
// References:
//
//  [1] Burton S. Kaliski; A Layman's Guide to a Subset of ASN1, BER
//      and DER; AN RSA Laboratories Technical Note
//

#include <stdlib.h>
#include "vdchunk-base.h"
// #include "lhexlib.h" // Already included in header file

using namespace std;

namespace vdchunk {

// *****************************************
// ***           Methods                 ***
// *****************************************

// ***********************************************
// *** Methods of the Class "vdchunk_sub_base" ***
// ***********************************************

int vdchunk_sub_base::add_fill1(vdcbyte c, unsigned long int num)

//! Adds a particular fill byte several times to the buffer

//! \param c fill byte
//! \param num quantity

{
 int               errcode = 0;
 unsigned long int loop;
 for (loop = 0; loop < num; loop++) {
  errcode = add_byte(c);
  if (errcode != 0) return errcode;
  };
 return errcode;
}

int vdchunk_sub_base::add_string(string s)

//
//! Adds a string (e.g. "Hello").

//! \param s string
//

{
#ifdef FFACOMMENT3
    cout << name << ".vdchunk::add_string(" << s << ")" << endl;
#endif
  
 int errcode = 0;
 int result;
 vdcbyte h;

 string::iterator myiterator;
 myiterator = s.begin();
 while ((myiterator != s.end()) && (errcode == 0)) {

  h = (vdcbyte)*myiterator;
  errcode = add_byte(h);
  myiterator++;
  }; // while

 return errcode;
}

int vdchunk_sub_base::add_hexstring(string s)

//
//! Adds some bytes given as a string in hex format (e.g. "AB00340f").

//! \param s string in hex format
//

{
#ifdef FFACOMMENT4
    cout << "vdchunk::add_hexstring(" << s << ")" << endl;
#endif
  
 int errcode = 0;
 int result;
 char l,h,bs[2];
 vdcbyte val;
 bool finished;

 string::iterator myiterator;
 myiterator = s.begin();
 while ((myiterator != s.end()) && (errcode == 0)) {

	finished = false;
	while (!finished) {
		h = *myiterator;
		myiterator++;
		// Ignore spaces before highbyte
		if ((h >= 0x030) || (myiterator == s.end())) 
			{ finished = true; };
	};
  
	if (myiterator == s.end()) {
		// Unexpected end of string
		// errcode = 4;

		//  Interpret "x" at the end as "0x"..... (see below)
		l = h; h = '0';
	}
	else {
		l = *myiterator;
		myiterator++;

		// Interpret "x " as "0x"
		if (l < 0x030) 
			{ l = h; h = '0'; };
	};

		bs[0] = h;
		bs[1] = l;
		result = hex2_to_int(bs);
		if (result < 0) {
			// Error in hex number
			errcode = 5;
		}
		else {
			assert((result >= 0) && (result < 256));
			val = (vdcbyte)result;

			// Add byte to buffer
			errcode = add_byte(val);
		}

 }; // while

 return errcode;
}

// ***********************************************
// ***   Methods of the Class "vdchunk_base"   ***
// ***********************************************

// *** Parsing ***

void vdchunk_base::parse_reset()

//! Resets parse counter

{
#ifdef FFACOMMENT4
    cout << name << ".vdchunk::parse_reset()" << endl;
#endif
  
	parsec = 0;
}

unsigned long int vdchunk_base::remaining_parse_length()

//! Returns the remaining parse length 

{
	assert(entries-parsec>=0);
	return (entries-parsec);
}

unsigned long int vdchunk_base::get_parse_counter()

//! Returns the parse counter

{
	return this->parsec;
}

void vdchunk_base::set_parse_counter(unsigned long int value)

//! Sets the parse counter

{
	this->parsec = value;
	return;
}

unsigned long int vdchunk_base::parse_counter()

//! Returns the parse counter. DEPRECATED -- Use "get_parse_counter".

{
	return parsec;
}

int vdchunk_base::parse_read()

//!
//!  Parser: Read Next Value
//!  Result: Value or -1 for End of File (EOF)
//!

{
#ifdef FFACOMMENT5
    cout << name << ".vdchunk_base::parse_read() : " << endl;
#endif
  
	int i;
	if (parsec >= entries) {

#ifdef FFACOMMENT6
    cout << "-1" << endl;
#endif
  
		return -1;
	}
	else {
		assert(parsec >= 0);
		assert(parsec < entries);

		i = (int)(get_byte(parsec)); 
		i = i & (int)(0x0FF); // IN 2009-00-21
		parsec++;

#ifdef FFACOMMENT6
    cout << i << endl;
#endif
  
		assert(parsec > 0);
		assert(parsec <= entries);
		return i;
	};
}

int vdchunk_base::parse_look_ahead()

//! Parser: "Look Ahead" one value (do not increase counter)

//! Return value: -1 for End of File (EOF)

{
	int i;
	if (parsec == entries) {
		return -1;
	}
	else {
		i = (int)(get_byte(parsec)); 
		i = i & (int)(0x0FF); 
		return i;
	};
}

int vdchunk_base::parse_look_ahead(unsigned long int pos)

	/*
	 * Parser: "Look Ahead" one value (do not increase counter)
         *
         * Parameter: 
	 *           POS    - If not zero, skip POS vdcbytes
	 *
	 * Result: Value or -1 for End of File (EOF)
	 *
         * (PARSE_LOOK_AHEAD() equals PARSE_LOOK_AHEAD(0))
	 *
	 */

{
	int i;
	if ((parsec+pos) >= entries) {
		return -1;
	}
	else {
		i = (int)(get_byte(parsec+pos)); 
		i = i & (int)(0x0FF); 
		return i;
	};
}

int vdchunk_base::parse_skip_bytes(unsigned long int num)

	/*
	 * Parser: Skip NUM vdcbytes from reading
	 *
	 * Result: -1, if there weren't enough vdcbytes to skip
	 *         0,  otherwise
	 *
	 */

{
	parsec = parsec + num;
	if (parsec > entries) {
		parsec = entries;
		return -1;
	}
	else {
		return 0;
	};
}

int vdchunk_base::parse_copy_bytes(unsigned long int n, vdchunk_base *neu)

//! Copy n bytes from current parser postion into new object; object must already exist
	
//!  Result: -1, if there weren't enough bytes to copy (the new object contains the existing ones) 
//!          -2, if new object does not exist
//!	     -3, if a write error occurs
//!           0,  otherwise

{
	unsigned long int final, loop;
	int res, errcode;
	vdcbyte c;

	if (neu != NIL) {
		neu->clear();

		res = 0;

		final = parsec + n;
		if (final > entries) {
			final = entries;
			res = -1; // not enough vdcbytes in object
		}
		
		errcode = 0;
		loop = parsec;

		while ((loop < final) && (errcode == 0)) {
  			c = this->get_byte(loop);
  			errcode = neu->add_byte(c);
  			loop++;
		}; // while

		if (errcode != 0) { res = -3; }; // write error
	}
	else {
		res = -2; // new object does not exist
	};
	
	return res;
}

unsigned int vdchunk_base::get_word16_BE(unsigned long int pos)

//!  Reads two bytes and interprets them as a 16-bit word. 
//!  If the position is outside the data block, or the data 
//!  block is empty the return value is zero. If only the position
//!  of the second byte is outside the data block, the return
//!  value contains the first byte as the lower byte, and the higher
//!  byte is zero.
//!  Big Endian, i.e. the most significant byte (MSB) comes first.
	
//! \param pos Position in data block

{
	unsigned int res;
	vdcbyte      c;

	if (pos >= entries) {
		res = 0;
	}
	else {
		c = this->get_byte(pos);
		res = (unsigned int)c;
		if ((pos+1) < entries) {

			res = (res << 8) & (0x0FF00);
			c = this->get_byte(pos+1);
			assert((res+c) == (res|c));
			res = res + (unsigned int)c;
		};
	};

	return res;
}

unsigned int vdchunk_base::get_word16_LE(unsigned long int pos)

//!  Reads two bytes and interprets them as a 16-bit word. 
//!  If the position is outside the data block, or the data 
//!  block is empty the return value is zero. If only the position
//!  of the second byte is outside the data block, the return
//!  value contains the first byte as the lower byte, and the higher
//!  byte is zero.
//!  Little Endian, i.e. the least significant byte (LSB) comes first.
	
//! \param pos Position in data block

{
	unsigned int res;
	vdcbyte      c, d;

	if (pos >= entries) {
		res = 0;
	}
	else {
		c = this->get_byte(pos);
		res = (unsigned int)c; // only for exceptional case
		if ((pos+1) < entries) {

			// Little Endian: c = low byte, d = high byte
			d = this->get_byte(pos+1);
			res = (unsigned int)d;
			res = (res << 8) & (0x0FF00);

			assert((res+c) == (res|c));
			res = res + (unsigned int)c;
		};
	};

	return res;
}

unsigned int vdchunk_base::get_word32_BE(unsigned long int pos)

//!  Reads two bytes and interprets them as a 32-bit word. 
//!  Big Endian, i.e. the most significant byte (MSB) comes first.
	
//! \param pos Position in data block

{
	unsigned long int res;
	unsigned int      cw;

	if (pos >= entries) {
		res = 0;
	}
	else {
		cw = this->get_word16_BE(pos);
		res = (unsigned long int)cw;
		if ((pos+3) < entries) {

			res = (res << 16) & (0x0FFFF0000);
			cw = this->get_word16_BE(pos+2);
			assert((res+cw) == (res|cw));
			res = res + (unsigned long int)cw;
		};
	};

	return res;
}

unsigned int vdchunk_base::get_word32_LE(unsigned long int pos)

//!  Reads two bytes and interprets them as a 32-bit word. 
//!  Little Endian, i.e. the least significant byte (LSB) comes first.
	
//! \param pos Position in data block

{
	unsigned long int res;
	unsigned int      cw, dw;

	res = 0;

	if (pos >= entries) {
		res = 0;
	}
	else {
		cw = this->get_word16_LE(pos);
		if ((pos+3) < entries) {

			// Little Endian: cw = low word, dw = high word
			dw = this->get_word16_LE(pos+2);
			res = (unsigned long int)dw;
			res = (res << 16) & (0x0FFFF0000);

			assert((res+cw) == (res|cw));
			res = res + (unsigned long int)cw;
		};
	};

	return res;
}

// *** ASN1 Parsing ***

#ifdef VDCHUNKABSTRACTASN1

long int vdchunk_base::parse_asn1_length()

//! Parses length of ASN1 tag

{
	return this->parse_ber_encoded_length();

	// In the case of indefinite length, this should parse the tag....
}

long int vdchunk_base::parse_ber_encoded_length()

//!  Parse BER-encoded length

//!
//!  Result: -1               --  if DER length is encoded incorrectly
//!         -2                --  if DER encoding is corrupt due to EOF
//!         -8                --  Indefinite length
//!
//!         resulting length  --   otherwise
//!
//! Documentation: 
//!          a) ISO 7816-4, 5.2.2.2, BER-TLV length fields
//!          b) ITU-T X.690
//!

// ToDo: 
//  - Slight differences between BER and DER (see below, 81)
//  - Indefinite length

{
	return this->parse_der_encoded_length();

	// In the case of indefinite length, this should parse the tag....
}

long int vdchunk_base::parse_der_encoded_length()

//!  Parse DER-encoded length

//!
//!  Result: -1               --  if DER length is encoded incorrectly
//!         -2                --  if DER encoding is corrupt due to EOF
//!         -8                --  Indefinite length
//!
//!         resulting length  --   otherwise
//!
//! Documentation: 
//!          a) ISO 7816-4, 5.2.2.2, BER-TLV length fields
//!          b) ITU-T X.690
//!

{
#ifdef FFACOMMENT4
    cout << name << ".vdchunk::parse_der_encoded_length()" << endl;
#endif

	int c, d, e;
	long int temp;

	c = parse_read(); assert(c < 256);
	if (c < 0) {
		// EOF
		return -2;
	};

	if (c < 0x080) {
		// Indefinite length

		return (long int)c;
	} // c < 80
	else {
	if (c == 0x080) {
		// Not in DER: 80h encodes indefinite length 
//		c = this->remaining_parse_length(); (old work-around)
		return -8;
	} // c = 80
	else {
	if (c == 0x081) {
		c = parse_read(); assert(c < 256);
		if (c < 0) return -2;

		if (c > 127) {
			return (long int)c;
		} 
		else {
		// DER:
		// return -1;

		// BER:
		return (int)c;
		}
	} // c = 81
	else {
	if (c == 0x082) {
		c = parse_read(); assert(c < 256);
		if (c < 0) return -2;
		d = parse_read(); assert(d < 256);
		if (d < 0) return -2;

		temp = (long int)c;
		temp = temp<<8;
		temp = temp + (long int)d;
		assert(temp > 0);
		
		return temp;

	} // if c = 82
	else {
	if (c == 0x083) {
		c = parse_read(); assert(c < 256);
		if (c < 0) return -2;
		d = parse_read(); assert(d < 256);
		if (d < 0) return -2;
		e = parse_read(); assert(e < 256);
		if (e < 0) return -2;

		temp = (long int)c;
		temp = temp<<8;
		temp = temp + (long int)d;
		temp = temp<<8;
		temp = temp + (long int)e;
		assert(temp > 0);
		
		return temp;
	} // if c = 83
	else {
		return -1;
	}; // else 83
	}; // else 82
	}; // else 81
	}; // else 80
	}; // else <80
}

string vdchunk_base::parse_object_identifier()

	// 
	// Parses an object identifier (DER encoded length) and returns the result as a string
	//
	// Return value:
	//              object identifier as a string with decimal values separated by
	//              commas, e.g. "1.2.840.10045", or "err" in case of an error
	//
	// Precondition:
	//             - The parse counter must point to the object identifier (06 len ... ...)

{
#ifdef FFACOMMENT3
    cout << name << ".vdchunk::parse_object_identifier()" << endl;
#endif

	string result;
	unsigned long int len, loop, val;
	int c;


	if ((this->parse_read() & 0x01f) == 0x006) {
		len = this->parse_der_encoded_length();
		if (len > 0) {
			result = "";

			// The first byte has value 40*value1+value2

			c = this->parse_read();
			len--;
			if (c >= 0) {
				assert(c < 256);
				val = (unsigned long int)(c / 40);
				result = ulint_to_string(val);
				// assert((c % 40) == (c - val*40)); // check
				val = c % 40;
				result = result + "." + ulint_to_string(val);
			}
			else {
				assert(c == -1);
				// Error: Unexpected EOF
				result = "err";
				return result;
			};
			
			// The following vdcbytes, if any, encode value3, ...., value-n. Each
			// value is encoded base 128, most significant digit first, with as
			// few digits as possible, and the most significant bit of each
			// vdcbyte except the last in the value's encoding set to '1'. [1]

			val = 0;
			while (len > 0) {
				c = this->parse_read();
				len--;
				if (c >= 0) {
					assert(c < 256);
					if (c < 128) {
						// last vdcbyte of number
						val = val*128;
						val = val + c;
						// add to string
						result = result + "." + ulint_to_string(val);
						val = 0;
					}	
					else {
						// accumulate values
						val = val*128;
						val = val + (c & 0x7f);
					};
				}
				else {
					assert(c == -1);
					// Error: Unexpected EOF
					result = "err";
					return result;
				};
			}; // while
		}
		else {
			// Error: Zero length
			result = "err";
			return result;
		};
	}
	else {
		// Error: Tag indicating object identifier missing
		result = "err";
	};

	return result;
}

#endif // VDCHUNKABSTRACTASN1

// *** Input ***

int vdchunk_base::read_binary_file(string filename)

//! Reads binary file and copies the content into the buffer. The buffer will be emptied
//! before reading the file.

//! \param filename File name

{

#ifdef FFACOMMENT3
    cout << "vdchunk_base::read_binary_file(" << filename << ")" << endl;
#endif
  
    int errcode = 0;
  
    this->clear();
    
    FILE *fp;
    if ((fp = fopen(filename.c_str(), "rb")) == NULL) // "r" = read, "w" = write
        {
            // File error
    
#ifdef FFACOMMENTERROR
            fprintf(stderr, "VDCHUNK: File error. Can't open %s\n", filename.c_str());
#endif

            return 2; // Exit code 2: File error
        };

    errcode = this->read_binary_stream(fp);
 
    fclose(fp);

    return errcode;
}

int vdchunk_base::read_binary_stream(FILE *fp)

//! Reads block from binary stream and appends it to the buffer.

//! \param fp Stream

{

#ifdef FFACOMMENT5
  cout << "vdchunk_base::read_binary_stream()" << endl;
#endif

    char c;
    int  i       = 0;
    int  errcode = 0;
    size_t dummy;
    
    clearerr(fp);
    
//
// ***
// 

//
// a) Doesn't work properly; stops at 0
//
//   while ((c = fgetc(fp)) && (feof(fp) == 0)) {
//
// b) Doesn't work properly; stops at 0FFh (-1)
//
//   while ((c = fgetc(fp)) != EOF)  {
//
// c) This seems to work
//

    dummy = fread(&c, 1, 1, fp);
    while (!feof(fp))  {

    i++; // Count no. of bytes
    // fprintf(stdout,"%d ", (int)c); 

    // Add byte to buffer and 
    errcode = add_byte((vdcbyte)c);
    // return error if this operation fails
    if (errcode != 0) return errcode;
    
            dummy = fread(&c, 1, 1, fp);
//
// ***
//
    
      }; // while

    return 0; // Exit: No errors
}

int vdchunk_base::read_binary_from_port(int fd)

//! Reads one byte from an open port and appends it to the buffer.
//! Untested.

//! \param fd File descriptor of port

{

#ifdef FFACOMMENT5
 cout << "vdchunk_base::read_binary_from_port()" << endl;
#endif

 int res = 0;
 unsigned long int loop;
 vdcbyte c;
 vdcbyte *sp;

 sp = &c;
 res = read(fd, sp, 1);

 // res = 0 : No byte available
 if (res == 0) this->add_byte(c);
 
 return res; // Exit: res < 0 : No errors
}

int vdchunk_base::read_hex_file(string filename)

//! Reads file and copies the content into the buffer.
//! File format: Bytes as hex values ("01020304"); spaces will be ignored
//! ("01 02 03 04 05") as well as empty lines and lines beginning
//! with "%", "$" or "#".

//! \param filename File name
//! \return 0, if ok; 2, if file error

{

#ifdef FFACOMMENT3
	cout << "vdchunk_base::read_hex_file(" << filename << ")" << endl;
#endif
  
	unsigned long int loop;
	int               errcode;
	bool              finished;
	char              sbuffer1[255];
	char              *sp, c;
	string            s1, s2;
  
	this->clear();
    
	errcode = 0;
	FILE *fp;
	if ((fp = fopen(filename.c_str(), "rb")) == NULL) // "r" = read, "w" = write
        {
            // File error
    
#ifdef FFACOMMENTERROR
            fprintf(stderr, "VDCHUNK: File error. Can't open %s.\n", filename.c_str());
#endif

            errcode = 2; // Exit code 2: File error
        }
	else {
		finished = false;

		while (!finished) {

		sp = &sbuffer1[0]; // Speicherplatz fuer String
		sp = fgets(sp, 255, fp);  // String lesen

		if (feof(fp)) finished = true;

		if (sp!=0) {
			s1 = sbuffer1; // "old-style"-String in String umwandeln
			// s1 = s1.substr(0, s1.length()-1); // "newline" am Ende loeschen
			// if (s1.substr(0,3) == "EOF") { finished = true; skip_line = true; };

		        if ((s1[0] == '#') || (s1[0] == '%') || (s1[0] == '$')) {
                		// Comments
				// Don't do anything
		        }
		        else {
				s2 = "";
				for (loop = 0; loop<s1.length(); loop++) {
					c = s1[loop];
					if (((c >= '0') && (c <= '9')) ||
					    ((c >= 'A') && (c <= 'F')) ||
					    ((c >= 'a') && (c <= 'f'))) {
						s2.append(1,c);
					}; // if
				}; // for
				this->add_hexstring(s2);
			}; // else
		}; // end of if
		}; // end of while

	fclose(fp); // Should not be closed when it wasn't open

	}; // else

	return errcode;
}

// *** Output ***

int vdchunk_base::write_binary_file(string filename)
   
//! Writes the content of the buffer into a binary file.

//! \param filename File name

{

#ifdef FFACOMMENT3
    cout << name << ".vdchunk_base::write_binary_file(" << filename << ")" << endl;
#endif
  
    int errcode = 0;
  
    FILE *fp;
    if ((fp = fopen(filename.c_str(), "w")) == NULL) // "r" = read, "w" = write
        {
            // File error
    
#ifdef FFACOMMENTERROR
            fprintf(stderr, "VDCHUNK_ABSTRACT: File error. Can't open %s\n", filename.c_str());
#endif

            return 2; // Exit code 2: File error
        };

    errcode = this->write_binary_stream(fp);
 
    fclose(fp);

    return errcode;
}

int vdchunk_base::write_binary_stream(FILE *fp)

//! Writes the content of the buffer into a binary stream.

//! \param fp Stream

{

#ifdef FFACOMMENT5
  cout << "vdchunk_base::write_binary_stream()" << endl;
#endif

    unsigned long int loop;
    vdcbyte c;

    // Loop over all entries
    for (loop = 0; loop < entries; loop++)
 {

 // Get vdcbyte from buffer
 c = get_byte(loop);
 // Write vdcbyte into stream
 fprintf(fp, "%c", c);
 
 };
    
    return 0; // Exit: No errors
}

int vdchunk_base::write_binary_to_port(int fd)

//! Writes the content of the buffer to an open port.

//! \param fd File descriptor of port

{

#ifdef FFACOMMENT5
 cout << "vdchunk_base::write_binary_to_port()" << endl;
#endif

 int res = 0;
 unsigned long int loop;
 vdcbyte c;
 vdcbyte *sp;

 // Loop over all entries
 for (loop = 0; (loop < entries) && (res >= 0); loop++)
 {

	// Get byte from buffer
	c = get_byte(loop);
	// Write byte to port
	sp = &c;
	res = write(fd, sp, 1);
 };
 
 return res; // Exit: No errors
}

int vdchunk_base::write_hex_file(string filename)

//! Write buffer to hex file (similar to Intel hex file but without additional information)
   
//! \param filename Filename

{

#ifdef FFACOMMENT3
    cout << "vdchunk_base::write_hex_file(" << filename << ")" << endl;
#endif
  
    int errcode = 0;
    unsigned long int loop;
    unsigned int      loop2,dloop;
    unsigned char     d;
  
    ihex_default_reclen = 16;

    // Check value for default record length
    if ((ihex_default_reclen < 1) || (ihex_default_reclen > 255))
       ihex_default_reclen = 16;

    FILE *fp;
    if ((fp = fopen(filename.c_str(), "w")) == NULL) // "r" = read, "w" = write
        {
            // File error
    
#ifdef FFACOMMENTERROR
            fprintf(stderr, "vdchunk_base::write_hex_file:: File error. Can't open %s\n", filename.c_str());
#endif

            return 2; // Exit code 2: File error
        };

/*
    // This works .....

    loop = 0;
    while (loop < entries) {

 dloop = entries - loop;
 // Default record length: property "ihex_default_reclen" (default value: 16)
 if (dloop > ihex_default_reclen) dloop = ihex_default_reclen;

 // DATA
 for (loop2 = 0; loop2 < dloop; loop2++) {

  d = (unsigned char)(get_byte(loop+loop2));
  fprintf(fp, "%02X", d);

  }; // for

 // CR
 fprintf(fp,"\n");

 loop = loop + dloop;
 }; // while
*/ 
    
    // .... but this is preferred
    this->write_hex(fp,"", 16, 0);

    fclose(fp);

    return errcode;
}

int vdchunk_base::write_ihex_file(string filename, unsigned long int start)

  /*
   * Write block buffer to Intel hex file 
   *
   */
   
{

#ifdef FFACOMMENT3
    cout << "vdchunk_base::write_ihex_file(" << filename << "," << start << ")" << endl;
#endif
  
    int errcode = 0;
    unsigned long int loop, clow, chigh;
    unsigned int      loop2,dloop;
    unsigned char     d, chksum;

    FILE *fp;
    if ((fp = fopen(filename.c_str(), "w")) == NULL) // "r" = read, "w" = write
        {
            // File error
    
#ifdef FFACOMMENTERROR
            fprintf(stderr, "VDCHUNK_BASE: File error. Can't open %s\n", filename.c_str());
#endif

            return 2; // Exit code 2: File error
        };

    clow  = start & 65535;          // Lower part of "start"
    chigh = (start >> 16) & 65535; // Higher part
    if (start+entries > 65535) {
 // Extended Linear Address Record necessary
 vdchunk_ihex_elar(fp, chigh);
 }; // if

    loop = 0;
    while (loop < entries) {

 chksum = (unsigned char)0;

 // RECORD MARK(:)
 fprintf(fp, ":");

 dloop = entries - loop;
 // Default record length: property "ihex_default_reclen" (default value: 16)
 if (dloop > ihex_default_reclen) dloop = ihex_default_reclen;
 if ((dloop + clow) > 65536) dloop = 65536-clow;

 // RECLEN(dloop)
 fprintf(fp, "%02X", (unsigned char)(dloop));
 chksum = (unsigned char)((chksum + dloop) & 255);

 // LOAD OFFSET
 fprintf(fp, "%04X", (unsigned int)clow);
 chksum = (unsigned char)((chksum + ((clow >> 8) & 255)) & 255);
 chksum = (unsigned char)((chksum + (clow & 255)) & 255);

 // RECTYP
 fprintf(fp, "00");

 // DATA
 for (loop2 = 0; loop2 < dloop; loop2++) {

  d = (unsigned char)(get_byte(loop+loop2));
  fprintf(fp, "%02X", d);
  chksum = (unsigned char)((chksum + d) & 255);

  }; // for

 // Two's complement
 chksum = (unsigned char)(((chksum^255)+1)&255);

 // CHKSUM
 fprintf(fp,"%02X\n", chksum);

 loop = loop + dloop;
 clow = clow + dloop;
 assert(clow <= 65536); // see above
 if (clow == 65536) {

  // Next "page": Extended Linear Address Record
  chigh++;
  clow = 0;
  vdchunk_ihex_elar(fp, chigh);
  }; // if

 }; // while
 
    // End of File Record
    vdchunk_ihex_final(fp);

    fclose(fp);

    return errcode;
}

void vdchunk_base::write_hex()

//! Write buffer in hex format (e.g. "A1 B2 03") to "stdout"
   
{
	this->write_hex(stdout);
}

void vdchunk_base::write_hex(string indent, unsigned long int charperline, unsigned char c)

//! Write buffer in hex format (e.g. "A1 B2 03") to "stdout"
   
//! \param indent String that contains the indention
//! \param charperline Number of characters before line feed
//! \param c Separating character between the numbers (or 0)

{
	this->write_hex(stdout, indent, charperline, 32);
}

void vdchunk_base::write_hex(FILE *fp)

//! Write buffer in hex format (e.g. "A1 B2 03") to stream
   
//! \param fp Stream

{
	this->write_hex(fp, "", this->entries+1, 32);
}

void vdchunk_base::write_hex(FILE *fp, string indent, unsigned long int charperline, unsigned char c)

//! Write buffer in hex format (e.g. "A1 B2 03") to stream
   
//! \param fp Stream
//! \param indent String that contains the indention
//! \param charperline Number of characters before line feed
//! \param c Separating character between the numbers (or 0)

{
	this->write_hex(fp, indent, charperline, 32, 0);
}

void vdchunk_base::write_hex(string indent, unsigned long int charperline, unsigned char c, unsigned long int chartotal)

//! Write buffer in hex format (e.g. "A1 B2 03") to stdout
   
//! \param indent String that contains the indention
//! \param charperline Number of characters before line feed
//! \param c Separating character between the numbers (or 0)
//! \param chartotal Number of bytes to be written (0: all)

{
	this->write_hex(stdout, "", this->entries+1, 32, 0);
}

void vdchunk_base::write_hex(FILE *fp, string indent, unsigned long int charperline, unsigned char c, unsigned long int chartotal)

//! Write buffer in hex format (e.g. "A1 B2 03") to stream
   
//! \param fp Stream
//! \param indent String that contains the indention
//! \param charperline Number of characters before line feed
//! \param c Separating character between the numbers (or 0)
//! \param chartotal Number of bytes to be written (0: all)

{
#ifdef FFACOMMENT3
    cout << "vdchunk_base::write_hex(...," << chartotal << ") [" << this->entries << "]" << endl;
#endif
  
 unsigned long int loop;
 unsigned long int count;
 unsigned long int total;
 vdcbyte              d;

 count = 0;
 total = this->entries;
 if (chartotal > 0) {
    if (chartotal < this->entries) {
       total = chartotal;
    };
 };

 // "total": number of bytes to be written
 for (loop = 0; loop < total; loop++) {

  // Indention string, e.g. "    : "
  if (count == 0) { 
	if (loop != 0) { fprintf (fp, "\n"); };
	fprintf(fp,"%s", indent.c_str()); 
  };

  d = (unsigned char)(get_byte(loop));
  if ((c >= 32) && (c <= 127)) {
    fprintf(fp, "%02X%c", d, c);
  }
  else {
    fprintf(fp, "%02X", d);
  };

  // Line feed?
  count++;
  if (count == charperline) { count = 0; };

 }; // for

 // Write "[xx bytes skipped]" unless
 // number of skipped bytes == 0
 if ((this->entries - total) > 0) {
    assert(chartotal != 0); // Otherwise there should be no skipped bytes
    fprintf(fp,"\n%s[0x%04X bytes skipped.]", indent.c_str(), (unsigned int)(this->entries - total)); 
 };

//  fprintf(fp, "\n");
 return;
}

void vdchunk_base::write_string_secure(FILE *fp)

//! Write buffer as a string into stream. Secure version, 
//! only valid ASCII codes will be written (no LF!).
   
{
 unsigned long int loop;
 vdcbyte              d;

 for (loop = 0; loop < entries; loop++) {

  d = (unsigned char)(get_byte(loop));
  if ((d >= 32) && (d <= 127)) {
	  fprintf(fp, "%c", d);
  };
 };

//  fprintf(fp, "\n");
 return;
}

void vdchunk_base::write_string_secure()

//! Write buffer as a string to "stdout". Secure version, 
//! only valid ASCII codes will be written (no LF!).
   
{
	this->write_string_secure(stdout);
}

void vdchunk_base::write_string(FILE *fp)

//! Write buffer as a string into stream
   
{
 unsigned long int loop;
 vdcbyte              d;

 for (loop = 0; loop < entries; loop++) {

  d = (unsigned char)(get_byte(loop));
  fprintf(fp, "%c", d);
 };

//  fprintf(fp, "\n");
 return;
}

void vdchunk_base::write_string()

//! Write buffer as a string to "stdout"
   
{
	this->write_string(stdout);
}

// *** Copying ***

int vdchunk_base::add_copy_bytes(vdchunk_base *source, unsigned long int start, unsigned long int length)

//! Adds some bytes to the buffer which are copied from another VDCHUNK instance

{
 unsigned long int loop;
 int               errcode = 0;
 vdcbyte              c;

 loop = start;

 if ((start + length) > source->entries) errcode = 6;

 while ((loop < (start+length)) && (errcode == 0)) {

  c = source->get_byte(loop);
  errcode = add_byte(c);

  loop++;
 }; // while

 return errcode;
}

int vdchunk_base::add_copy_bytes_backwards(vdchunk_base *source, unsigned long int start, unsigned long int length)

{
 unsigned long int loop;
 int               errcode = 0;
 vdcbyte              c;

 loop = start;

 if ((start < length) || (start > source->entries)) errcode = 6;

 while ((loop > (start-length)) && (errcode == 0)) {

  c = source->get_byte(loop-1);
  errcode = this->add_byte(c);

  loop--;
 }; // while

 return errcode;
}

int vdchunk_base::xor_operation2(vdchunk_base *source, vdchunk_base *mask)

//! Perform the "XOR" operation of two given data blocks and save the result in the buffer.

//! \param source Source data block 
//! \param mask Data block to be combined with the source data block.
//! 
//! The result will have the length of the first data block (source). If the second
//! data block is shorter it will be used several times; if it is longer, the remaining
//! parts will not be used.

{
	unsigned long int loop1, loop2, se, me;
	int               res;

	// Clear buffer
	clear();

	me = mask->entries; // Number of mask entries
	if (me == 0) return 0; // If zero, do nothing

	se = source->entries; // Number of mask entries
	if (se == 0) return 0; // If zero, do nothing

	loop2 = 0;
	for (loop1 = 0; loop1 < se; loop1++) {

		// read vdcbyte, calculate operation and write it back
		res = add_byte(source->get_byte(loop1) ^ mask->get_byte(loop2));
		if (res != 0) { return res; };

		// When end of mask buffer reached, start from the beginning
		assert(loop2 < me);
		loop2++; 
		if (loop2 == me) { loop2 = 0; };
		};

	// Result will be as long as the first block
	assert(entries == se);
	return 0;
}

int vdchunk_base::or_operation(vdchunk_base *source, vdchunk_base *mask)

//! Perform the "OR" operation of two given data blocks and save the result in the buffer.

//! \param source Source data block 
//! \param mask Data block to be combined with the source data block.
//! 
//! The result will have the length of the first data block (source). If the second
//! data block is shorter it will be used several times; if it is longer, the remaining
//! parts will not be used.

{
	unsigned long int loop1, loop2, se, me;
	int               res;

	// Clear buffer
	clear();

	me = mask->entries; // Number of mask entries
	if (me == 0) return 0; // If zero, do nothing

	se = source->entries; // Number of mask entries
	if (se == 0) return 0; // If zero, do nothing

	loop2 = 0;
	for (loop1 = 0; loop1 < se; loop1++) {

		// read vdcbyte, calculate operation and write it back
		res = add_byte(source->get_byte(loop1) | mask->get_byte(loop2));
		if (res != 0) { return res; };

		// When end of mask buffer reached, start from the beginning
		assert(loop2 < me);
		loop2++; 
		if (loop2 == me) { loop2 = 0; };
		};

	// Result will be as long as the first block
	assert(entries == se);
	return 0;
}

int vdchunk_base::and_operation(vdchunk_base *source, vdchunk_base *mask)

//! Perform the "AND" operation of two given data blocks and save the result in the buffer.

//! \param source Source data block 
//! \param mask Data block to be combined with the source data block.
//! 
//! The result will have the length of the first data block (source). If the second
//! data block is shorter it will be used several times; if it is longer, the remaining
//! parts will not be used.

{
	unsigned long int loop1, loop2, se, me;
	int               res;

	// Clear buffer
	clear();

	me = mask->entries; // Number of mask entries
	if (me == 0) return 0; // If zero, do nothing

	se = source->entries; // Number of mask entries
	if (se == 0) return 0; // If zero, do nothing

	loop2 = 0;
	for (loop1 = 0; loop1 < se; loop1++) {

		// read vdcbyte, calculate operation and write it back
		res = add_byte(source->get_byte(loop1) & mask->get_byte(loop2));
		if (res != 0) { return res; };

		// When end of mask buffer reached, start from the beginning
		assert(loop2 < me);
		loop2++; 
		if (loop2 == me) { loop2 = 0; };
		};

	// Result will be as long as the first block
	assert(entries == se);
	return 0;
}

int vdchunk_base::copy_content(vdchunk_base *source)

//! Copies a given data block into the buffer. 
//!
//! \param source Data block
//!
//! Does not append.
//! Return value: Zero if successful, transports the error code of the subroutines otherwise.

{
        // Clear buffer first; do not append
        this->clear();
        // Copy the entire content of "source"
        // into the buffer;
        return add_copy_bytes(source, 0, source->entries);
}


int vdchunk_base::add_vdchunk(vdchunk_base *source)

 /*
  * Add the content of another "vdchunk" into the buffer.
  *
	 * Parameters:
	 *             source   - source object
	 *
	 * Return value:
	 *             0        - if successful
	 *
	 */

{
	// Copy the entire content of "source"
	// into the buffer; 
	return add_copy_bytes(source, 0, source->entries);
}


int vdchunk_base::add_vdchunk_reversely(vdchunk_base *source)

//! Same as above but copies backwards

{
	// Copy the entire content of "source"
	// into the buffer, but reversely
	return add_copy_bytes_backwards(source, source->entries, source->entries);
}

// *** Comparing ***

bool vdchunk_base::equal(vdchunk_base *twin)

//! Checks whether two VDCHUNKs are the same; returns TRUE, if so.

{
    unsigned long int loop;
    
    if (this->entries != twin->entries) {return false; };
    return this->equal(twin, this->entries);
}

bool vdchunk_base::equal(vdchunk_base *twin, unsigned long int n)

//! Checks whether the first n bytes of two VDCHUNKs are the same; returns TRUE, if so.

{
    unsigned long int loop, max;
    
    // Normal case: n < entries, twin->entries
    max = n;

    if ((entries == twin->entries)) {
		// Special case: entries = twin->entries < n
		// New maximum value
		if (this->entries < max) { max = this->entries; };
    }
    else {
		// Special case: entries <> twin->entries and one of them smaller n
		// Not equal!
		if ((this->entries < max) || (twin->entries<max)) { return false; };
    };

    loop = 0;
    while (loop < max) {
        if (get_byte(loop) != twin->get_byte(loop)) {return false; };
        loop++;
    };
    return true;
}

/* Old version, replaced 2009-05-03, IN

bool vdchunk_base::equal(vdchunk_base *twin)
{
    unsigned long int loop;
    if (entries != twin->entries) {return false; };
    loop = 0;
    while (loop < entries) {
        if (get_byte(loop) != twin->get_byte(loop)) {return false; };
        loop++;
    };
    return true;
}
*/

bool vdchunk_base::operator==(vdchunk_base &twin)

//! Compares two instances.
//!
//! \param twin Instance for comparison
//! \return TRUE if equal, FALSE otherwise

{

#ifdef FFACOMMENT4
    cout << name << ".vdchunk_base::operator==(" << twin.name << ") Entries:" << entries << " " << twin.entries << endl;
#endif

    unsigned long int loop;

    if (this->entries != twin.entries) {return false; };
    loop = 0;
    while (loop < this->entries) {
        if (this->get_byte(loop) != twin.get_byte(loop)) {return false; };
        loop++;
    };

    return true;
}

unsigned long int vdchunk_base::crc16_sub(unsigned long int pipe, unsigned long int poly)

{
        int               loop;
        unsigned long int res;

        res = pipe;
        for (loop = 0; loop < 8; loop++) {
                res = res << 1;
                if ((res & 0x1000000) != 0) {
                        res = res ^ poly;
                };
        };

        res = res & 0x0ffffff;
        return res;
}

unsigned long int vdchunk_base::crc()

//! Calculates a CRC of the buffer

{
	return (unsigned long int)this->crc16_ccitt(); // Default implementation
}

unsigned short int vdchunk_base::crc16_ccitt()

//! Calculates a CRC of the buffer; CRC1-16-CCITT implementation

//! Please note: This is not an effcient implementation
//! Test vectors: "123456789" should yield 0x31C3; 0x01 should yield 0x1021

{
        unsigned long int pipe, polynom;
        unsigned long int loop;
        unsigned char     c; // unsigned !!

        pipe =    0x00000000;
        polynom = 0x01102100; // Normal: 1021 - reversed: 8408 
	                      // - reverse of reciprocal: 8810

        for (loop = 0; loop < this->entries; loop++) {
                c = (unsigned char)this->get_byte(loop);
                pipe = (pipe & 0x0ffff00) + c;
                pipe = crc16_sub(pipe, polynom);
        };
        pipe = crc16_sub(pipe, polynom);
        pipe = crc16_sub(pipe, polynom);

        pipe = (pipe >> 8) & 0x0ffff;

        return (unsigned short int)(pipe);
}

// **************************************************
// ***  Methods of the Class "vdchunk_base_del"   ***
// **************************************************

int vdchunk_base_del::insert_byte(unsigned long int position, vdcbyte value)

//! Insert byte and move all subsequent bytes

//! \param position Position where to insert the byte
//! \param value New value
//! \return 0 if successful, otherwise error codes from "add_byte" or "change_byte"
//!
//! Example a: [90 91 92 93 94] insert_byte(1, 5) --> [90 5 91 92 93 94]
//! Example b: [90 91 92 93 94] insert_byte(4, 9) --> [90 91 92 93 9 94]
//! Example c: [90 91 92 93 94] insert_byte(5, 7) --> [90 91 92 93 94 7]
//! Example d: [90 91 92 93 94] insert_byte(10, 8) --> [90 91 92 93 94 n.d. n.d. 8]
//! n.d. = not defined; if the class has inherited the base method, it will be 0

{
	int res;
	unsigned long int oldentries, loop;

	res = 0;
	oldentries = this->entries;

	if (position >= oldentries) {
		// Byte has to be appended
		while (position > this->entries) {
			// Case d: Fill remaining position with zero bytes
			res = this->add_byte(0);
			if (res != 0) return res;
		};
		// Cases c and d: append new value
		res = this->add_byte(value);
		if (res != 0) return res;
	}
	else {
		// Byte has to be inserted
		// Case b: position = 4, oldentries = 5
		assert(oldentries > 0);
		res = this->add_byte(get_byte(oldentries-1));
		if (res != 0) return res;
		assert(oldentries == this->entries-1);
		loop = oldentries - 1;
		while (loop > position) {
			res = this->change_byte(loop + 1, this->get_byte(loop));
			if (res != 0) return res;
			loop--;
		};
		assert(loop == position);
		res = this->change_byte(position, value);
		if (res != 0) return res;
	};

	assert(this->get_byte(position) == value);
	return 0;
}

// **************************************************
// *** Methods of the Class "vdchunk_base_cutout" ***
// **************************************************

vdchunk_base_cutout::vdchunk_base_cutout(string fname, vdchunk_base* fsource, unsigned long int fbegin, unsigned long int fend)

//! Constructor of the class "vdchunk_base_cutout"
//!
//! \param fname Name of cutout instance
//! \param fsource Pointer to source instance
//! \param fbegin Start position of cutout (included)
//! \param fend End position of cutout (excluded)
//!
//! Comment: If "fbegin" == "fend", size of cutout instance is zero.

{

#ifdef FFACOMMENT2
  cout << "Instance " << fname << " of class VDCHUNK_BASE_CUTOUT created." << endl;
#endif

  name = fname;        // Name of cutout
  source = fsource;    // Pointer to source instance

  cutout_begin = 0;
  cutout_end   = 0;
  if (source != NIL) {
     if (fbegin < source->entries) { 
		cutout_begin = fbegin; 
     } 
     else { 
		cutout_begin = source->entries; 
     };

     if (fend < source->entries) { 
		cutout_end = fend; 
     } 
     else { 
		cutout_end = source->entries; 
     };
  }; // source <> NIL

  entries = cutout_end - cutout_begin;

  this->parse_reset();
}

vdcbyte vdchunk_base_cutout::get_byte(unsigned long int position)

//! Gets one byte from the cutout instance, i.e. the corresponding byte from
//! the source instance.
//!
//! \param position Position of that byte in the cutout instance
//!

{
#ifdef FFACOMMENT7
    cout << name << ".vdchunk_base_cutout::get_byte(" << position << ")" << endl;
#endif
  
    if (position < entries)
        {
            if ((position + cutout_begin) < source->entries) {
                   return source->get_byte(position + cutout_begin);
            }
            else {
                   return (vdcbyte)0;
            };
        }
        else
        {
            return (vdcbyte)0;
        };
}

char vdchunk_base_cutout::is_dysfunctional()

//! Indicates whether an instance is dysfunctional. In the case of "vdchunk_simple" this could
//! be the case, for example, when there was not enough memory during initialisation. Here,
//! it means that the source instance is either NIL, or dysfunctional itself.
//!
//! \return 0 = ok, otherwise dysfunctional

{
	if (source == NIL) {
		return 2;
	}
	else {
		return source->is_dysfunctional();
	};
}

int vdchunk_base_cutout::cut_parsed_bytes()

//! Remove those bytes already parsed from the "cut-out" 
//! (but not from the original)

//! \return 0 == Ok. / 1 == Parser has reached the end of the class / 2 == error

{
#ifdef FFACOMMENT4
    cout << name << ".vdchunk_base_cutout::cut_parsed_bytes() Entries: " 
         << this->entries  << " Parsec: " << this-> parsec << endl;
#endif
  
	int res;

	if (parsec <= entries) {
		// New position in the original class
		this->cutout_begin = this->cutout_begin + this->parsec;
		assert(this->cutout_begin <= this->cutout_end);

		// Correct the number of entries
		entries = entries - this->parsec;
		res = 0;
		if (this->cutout_begin == this->cutout_end) { 
			res = 1; 
			assert(entries == 0);
			};
		this->parse_reset();
	}
	else {
		// This should not happen!
		res = 2;
	};
	return res;
}

// **************************************************
// *** Methods of the Class "vdchunk_base_empty"  ***
// **************************************************

vdchunk_base_empty::vdchunk_base_empty(string fname)

//! Constructor of "vdchunk_base_empty"
//!
//! \param fname Name of "empty" instance

{
#ifdef FFACOMMENT2
  cout << "Instance " << fname << " of class VDCHUNK_BASE_EMPTY created." << endl;
#endif

  name = fname;        // Name of cutout

  this->parse_reset();
}

// **************************************************
// ***           Basic functions                  ***
// **************************************************

// *** Parity ***

vdcbyte odd_parity(vdcbyte f)

 /*
  * Sets the parity bit of a vdcbyte to odd parity
  *
  */

{
#ifdef FFACOMMENTS7
    cout << "odd_parity(" << f << ")" << endl;
#endif

	vdcbyte g1,g2;
	int loop;

//	g1 = f; 
// I.N., 2010-10-18
	g1 = 0;
	g2 = f;

	for (loop = 1; loop < 8; loop++) {
		g2 = g2 >> 1;
		g1 = g1 ^ g2;

		};

	if ((g1 & 1) == 1) { return f; }
	else               { return (f^1); };
}

vdcbyte even_parity(vdcbyte f)

 /*
  * Sets the parity bit of a vdcbyte to odd parity
  *
  */

{
#ifdef FFACOMMENTS7
    cout << "even_parity(" << f << ")" << endl;
#endif

	vdcbyte g1,g2;
	int loop;

//	g1 = f; 
// I.N., 2010-10-18
	g1 = 0;
	g2 = f;

	for (loop = 1; loop < 8; loop++) {
		g2 = g2 >> 1;
		g1 = g1 ^ g2;

		};

	if ((g1 & 1) == 1) { return (f^1); }
	else               { return f; };
}

// *** Intel Hex Format ***

void vdchunk_ihex_elar(FILE* fp, unsigned long int ulba)

	/*
	 * Writes the command for the "Extended Linear Address Record",
	 * corresponding to the Intel Hex Format definition, into the
	 * output stream.
	 *
	 */

{
	unsigned char a,b, chksum;

	a = (unsigned char)((ulba >> 8) & 255);
	b = (unsigned char)(ulba & 255);

	// RECORD MARK(:), RECLEN(02), LOAD OFFSET(0000), RECTYP(04)
	fprintf(fp,":02000004");
	// ULBA
	fprintf(fp,"%02X", a);
	fprintf(fp,"%02X", b);

	// Sum of all vdcbytes from and including the RECLEN field
	chksum = (unsigned char)(2+4+a+b);
	// Two's complement
	chksum = (unsigned char)(((chksum^255)+1)&255);

	// CHKSUM
	fprintf(fp,"%02X\n", chksum);

	return;
}

void vdchunk_ihex_final(FILE* fp)

	/*
	 * Writes the final line of an Intel Hex Format file.
	 *
	 */

{

	// RECORD MARK(:), RECLEN(00), 
	// LOAD OFFSET(0000), RECTYP(01), CHKSUM(FF)
	fprintf(fp,":00000001FF");

	return;
}

} // namespace
