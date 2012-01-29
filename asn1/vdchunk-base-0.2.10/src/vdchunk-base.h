 
//
// VDChunk -- The Versatile Data Chunk
//
// Copyright (C) 2005-2010 Ingo Naumann
//
// Last revision: see below
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
// History:
//
//      [....]
//      Apr. 25th, 2009      - GET_WORD commands
//      May 3rd, 2009        - ToDo, equal(x)
//      July 1st, 2009       - Class "vdchunk_base_empty"
//      July 18th, 2009      - Bug fix
//      Nov. 18th, 2009      - Bug fix: "delete [] BLK"
//      Apr. 7th, 2010       - Valgrind, "insert_bytes"
//      31 Aug, 2010     - v0.3.8     - "parse_der_encoded_length" now "parse_asn1_length"
//                                     and knows indefinite lengths and three-byte lengths
//      16 Sep, 2010     - v0.3.9     - Bug fix ASN1 indefinite lengths
//      9 Oct 2010       - v0.3.10    - "read_hex_file"
//      Oct. 23rd, 2010  - v0.3.11    - "write_binary_to_port"
//      Nov. 23rd, 2010  - v0.3.12    - Changes in "add_hexstring()" --> Ignore spaces
//      Dec. 19th, 2010  - v0.3.13    - Flag: "dysfunctional"
//      Jan 9th, 2010    - v0.3.16    - Bugfix: "read_hex_file" / fclose
//
// To do:
//      - namespace VDCHUNK
//      - "insert_vdchunk" command
//      - Inconsistency in class "vdchunk_base_del"
//      - additional classes: vdchunk_parse, vdchunk_asn1
//      - proper distinction between BER and DER encoded length
//      - fix "xor_operation" problem (see inheritance3.cpp)
//      - Remove "ihex_default_reclen"
//
// Error codes: see "vdchunk-simple.h"
//

#ifndef VDC_ABSTRACT_VERSION
#define VDC_ABSTRACT_VERSION "0.3.16"

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <assert.h>

using namespace std;

#include "lhexlib.h"

namespace vdchunk {

#define NIL 0
#define vdcbyte unsigned char

// #define FFACOMMENT2     // Comments level 2
// #define FFACOMMENT3     // Comments level 3
// #define FFACOMMENT4     // Comments level 4
// #define FFACOMMENT5     // Comments level 5
// #define FFACOMMENT7     // Comments level 7
// #define FFACOMMENTERROR // Error messages
#define VDCHUNKABSTRACTASN1 // Support methods for ASN1 parsing

// ***************************************
// ***         Class Definitions       ***
// ***************************************

class vdchunk_sub_sub_base {

public:

// *** Version ***

    virtual int version () { return 2; };

// *** Constructor and Destructor ***

    virtual ~vdchunk_sub_sub_base() { }; // virtual destructor is needed for derived classes

// *** Properties ***

    string name;
};

class vdchunk_sub_base : public vdchunk_sub_sub_base {

public:

// *** Data Structure ***

    // Delete all data of instance
    virtual int clear() = 0; // ABSTRACT METHOD: must delete all data from instance
    
    // Add bytes to buffer in different ways ....

    // One byte
    virtual int add_byte(vdcbyte c) = 0; // ABSTRACT METHOD: must save byte, must correct "entries"
    
    // Some bytes given as a string (e.g. "Hello")
    virtual int add_string(string s);
    // Some bytes given as a string in hex format (e.g. "AB0F3400")
    virtual int add_hexstring(string s);
    // Fill the buffer up with a constant value
    // (add "num" times the value "c")
    virtual int add_fill1(vdcbyte c, unsigned long int num);

// Some applications need a pointer to the buffer where the data is
// stored. Of course, depending on the implementation of the VDChunk class
// this pointer exists or not. As a default this pointer does not exist.
// However, an application might request this pointer from the VDChunk class.
// The VDChunk class might either refuse this request or temporarily copy
// the data into a linear part of the memory (if necessary) and provide
// the pointer. In many cases, this pointer might be available anyway.
// The application should release this pointer as soon as possible in order
// not to block some operations of the VDChunk class.
//
// Is this an ugly work-around? Only if you use it. :)
//

    virtual int request_vdcpointer() { return -1; };
    virtual vdcbyte *vdcpointer() { return NIL; };
    virtual void release_vdcpointer() { };

// To check general availability of "vdcpointer" during compile time, 
// define this in your class:
//
// virtual void check_vdcpointer_always_available() { return;  } 
// 
// Please note: if this is defined, "request_vdcpointer" MUST always
//              return zero;
// 
// To check availability of "vdcpointer" in principle during compile time, 
// define this in your class:
//
// virtual void check_vdcpointer_sometimes_available() { return;  } 

};

// **********************************************
// *** Class Definition of "vdchunk_base" ***
// **********************************************

class vdchunk_base : public vdchunk_sub_base {

private:

    // CRC
    unsigned long int crc16_sub(unsigned long int pipe, unsigned long int poly);

protected:

// *** Data Structure ***

    // Counter for parsing
    unsigned long int parsec;

public:

// *** Constructor and Destructor ***

// Does not exist --> base class

    virtual ~vdchunk_base() { }; // virtual destructor is needed for derived classes

// *** Data Structure ***

    // Add another VDCHUNK to the buffer (append)
    virtual int add_vdchunk(vdchunk_base *source);
    // Same as above but starts with the last byte and copies backwards
    virtual int add_vdchunk_reversely(vdchunk_base *source);

    // Delete vdcbytes (and move all subsequent vdcbytes backwards)
    // virtual int del_vdcbyte(unsigned long int position);
    // virtual int del_vdcbytes(unsigned long int from, unsigned long int to);

    // Read one byte; returns zero, if position is outside of data block
    virtual vdcbyte get_byte(unsigned long int position) = 0; // ABSTRACT METHOD: must read byte
    
    // Read two (vdc-) bytes and interpret them as a 16-bit integer
    // Little Endian
    virtual unsigned int get_word16_LE(unsigned long int position);
    // Big Endian
    virtual unsigned int get_word16_BE(unsigned long int position);
    // Read two (vdc-) bytes and interpret them as a 32-bit integer
    // Little Endian
    virtual unsigned int get_word32_LE(unsigned long int position);
    // Big Endian
    virtual unsigned int get_word32_BE(unsigned long int position);

// *** Input ***

    // Read binary file into block buffer
    virtual int read_binary_file(string filename);
    virtual int read_binary_stream(FILE *fp);
    virtual int read_binary_from_port(int fd);

    // Read hex file into block buffer
    virtual int read_hex_file(string filename);

// *** Output ***
    
    // Write block buffer in hex values
    // fp     : Output stream (default: stdout)
    // indent : indention from the left
    // charperline : number of bytes per line
    // c      : character for indention/separation
    // chartotal    : number of bytes written (default: all)
    virtual void write_hex();
    virtual void write_hex(FILE *fp);
    virtual void write_hex(string indent, unsigned long int charperline, unsigned char c);
    virtual void write_hex(FILE *fp, string indent, unsigned long int charperline, unsigned char c);
    virtual void write_hex(string indent, unsigned long int charperline, unsigned char c, 
		unsigned long int chartotal);
    virtual void write_hex(FILE *fp, string indent, unsigned long int charperline, unsigned char c, 
		unsigned long int chartotal);

    // Write block buffer as a string
    virtual void write_string();
    virtual void write_string(FILE *fp);
    virtual void write_string_secure(); // Secure version(s): no control characters
    virtual void write_string_secure(FILE *fp);

    // Write block buffer to binary file
    virtual int write_binary_file(string filename);
    virtual int write_binary_stream(FILE *fp);
    virtual int write_binary_to_port(int fd);

    // Write block buffer to Intel hex file
    virtual int write_ihex_file(string filename, unsigned long int start);
    // Write block buffer to hex file (similar to Intel
    // hex file but without additional information)
    virtual int write_hex_file(string filename);

// *** Copying ***

    // Copy the content of one "vdchunk" into another (do not append; do not change the size)
    virtual int copy_content(vdchunk_base *source);

// *** Modifying ***

//    virtual int xor_operation(vdchunk_base *source, vdchunk_base *mask);
    virtual int xor_operation2(vdchunk_base *source, vdchunk_base *mask);
// Temporary Work-around
    virtual int or_operation(vdchunk_base *source, vdchunk_base *mask);
    virtual int and_operation(vdchunk_base *source, vdchunk_base *mask);

    // virtual void inc();

    // Change the parity bit of all bytes to odd
    // virtual void odd_parity_all();

// *** Comparing ***

    // Compare two VDCHUNKs
    virtual bool equal(vdchunk_base *twin);
    // Compare the first n bytes of two VDCHUNKs
    virtual bool equal(vdchunk_base *twin, unsigned long int n);
    
    // Compare two instances
    virtual bool operator== (vdchunk_base &twin);

    // Calculate CRC
    virtual unsigned long int crc();
    virtual unsigned short int crc16_ccitt();

// *** Parsing ***

    // Reset parse function
    virtual void parse_reset();

    // Read next value (-1 for EOF)
    virtual int parse_read();
    // Look ahead one value but do not "read" it ("peek")
    virtual int parse_look_ahead();
    // Look ahead, but skip some bytes (POS)
    // (PARSE_LOOK_AHEAD(0) equals PARSE_LOOK_AHEAD())
    virtual int parse_look_ahead(unsigned long int pos);
    // Skip some bytes
    virtual int parse_skip_bytes(unsigned long int num);
    // Copy N bytes from current postion into new object
    // (-1 if EOF has been reached); new object must exist already
    // (-2 if not)
    virtual int parse_copy_bytes(unsigned long int n, vdchunk_base *neu);
    // Parse counter
    virtual unsigned long int parse_counter(); // Deprecated, IN 2010-08-21, please use "get_parse_counter"
    virtual unsigned long int get_parse_counter();
    virtual void set_parse_counter(unsigned long int value);
    // Remaining parse length
    virtual unsigned long int remaining_parse_length();

// *** ASN1 Parsing ***

#ifdef VDCHUNKABSTRACTASN1

    // Parse DER or BER length encoding and return value
    // (-1 if wrong encoding)
    virtual long int parse_asn1_length();
    virtual long int parse_der_encoded_length();
    virtual long int parse_ber_encoded_length();

    string parse_object_identifier();

#endif

// *** Important properties ***

    // Current number of entries in list (list size)
    unsigned long int entries;
    // In the future, this will be moved to "protected"; use the following two functions
    // instead:
    virtual unsigned long int get_entries() { return this->entries; };
    virtual void set_entries(unsigned long int value) { this->entries = value; };

    // Flag that indicates whether instance is dysfunctional or not
    // (e.g. because there wasn't enough space to allocate the memory)
    // char dysfunctional;
    virtual char is_dysfunctional() { return 0; };

// *** Unimportant Properties ***

    // Default record length (RECLEN) when exporting
    // Intel Hex Format files (16)
    // DEPRECATED: This will disappear soon.....
    int ihex_default_reclen;

// *** Subroutines ***

// ** Copying **

    // Subroutine of "copy":
    // copy parts of one "vdchunk" into this one
    virtual int add_copy_bytes(vdchunk_base *source, 
	unsigned long int start, unsigned long int length);
    // Same as above but backwards
    virtual int add_copy_bytes_backwards(vdchunk_base *source, 
	unsigned long int start, unsigned long int length);
};

// *************************************************
// *** Class Definition of "vdchunk_base_change" ***
// *************************************************

//! Abstract class based on "vdchunk_base" but contains the command
//! "change_byte"

class vdchunk_base_change : public vdchunk_base {

public:

// *** Constructor and Destructor ***

    // Constructor does not exist --> abstract base class

    virtual ~vdchunk_base_change() { }; // virtual destructor is needed for derived classes

    // Change one byte (must have been set before, i.e. "position<entries")
    virtual int change_byte(unsigned long int position, vdcbyte value) = 0;

};

// **********************************************
// *** Class Definition of "vdchunk_base_del" ***
// **********************************************

//! Abstract class based on "vdchunk_change" but contains the commands
//! "del_byte", "del_bytes", "insert_byte" and "insert_vdchunk" (tbd)

class vdchunk_base_del : public vdchunk_base_change {

public:

// *** Constructor and Destructor ***

    // Constructor does not exist --> abstract base class

    virtual ~vdchunk_base_del() { }; // virtual destructor is needed for derived classes

    // Delete bytes (and move all subsequent bytes backwards)
    virtual int del_byte(unsigned long int position) = 0;
    virtual int del_bytes(unsigned long int from, unsigned long int to) = 0;

    // Insert bytes (and move all subsequent bytes)
    virtual int insert_byte(unsigned long int position, vdcbyte value);
//    virtual int insert_vdchunk(unsigned long int position, vdchunk_base* source);

// It is somehow not logical that the "del" functions are purely abstract
// and the "insert" functions are not.

};

// *************************************************
// *** Class Definition of "vdchunk_base_cutout" ***
// *************************************************

//! The idea of this class is to provide the means to iteratively 
//! read (or parse) blocks of an VDChunk instance without having 
//! to copy the blocks into other instances. An instance of the 
//! class "vdchunk_base_cutout" (or "cutout instance") represents 
//! therefore an cut-out of the "source instance". In order to
//! instantiate a cutout instance you have to provide the first
//! and the last position of the section you want to "cut out".
//! This class is not an abstract class. However, you
//! need a pointer to an existing instance to create an 
//! instance of this class. You cannot add bytes to this class. 
//! If you change he content of the source instance, the changes
//! will be reflected in the content of the cutout instance (the
//! content of the source instance will not be copied but just 
//! referred to).

class vdchunk_base_cutout : public vdchunk_base {

private:

// *** Data Structure ***

    // Pointer to other VDChunk
    vdchunk_base* source;

    // Where it starts end ends
    unsigned long int cutout_begin; // Start (included)
    unsigned long int cutout_end;   // End (not included; if "cutout_begin == cutout_end" size is zero)

protected:

public:

// *** Constructor and Destructor ***

    // Suggest buffer size
    vdchunk_base_cutout(string fname, vdchunk_base* fsource, unsigned long int fbegin, 
	unsigned long int fend);

    virtual ~vdchunk_base_cutout() { }; // virtual destructor is needed for derived classes

// *** Data Structure ***

    // Read one byte
   virtual vdcbyte get_byte(unsigned long int position); // Read one byte from cutout instance

    // Add one byte to buffer
    virtual int add_byte(vdcbyte c) { return 20; }; // Add one byte to "cutout" instance -> Error!
    
    // Delete all data of instance
    virtual int clear() { return -1; }; // Delete all data from instance -> Error!
    
    // Check whether instance is dysfunctional (e.g. because there is not enough memory)
    virtual char is_dysfunctional();

// *** Parsing ***

    // Remove those bytes already parsed from the "cut out" (but not from the original)
    virtual int cut_parsed_bytes();

};


// *************************************************
// *** Class Definition of "vdchunk_base_empty"  ***
// *************************************************

//! Data container which is always empty, therefore
//! in a way dysfunctional but nevertheless sometimes
//! helpful.

class vdchunk_base_empty : public vdchunk_base {

private:

protected:

public:

// *** Constructor and Destructor ***

    vdchunk_base_empty(string fname);
    virtual ~vdchunk_base_empty() { }; // virtual destructor is needed for derived classes

// *** Data Structure ***

    // Read one byte
   virtual vdcbyte get_byte(unsigned long int position) 
           { return (vdcbyte)0; }; // Read one byte from "empty" instance: always zero

    // Write one byte
    virtual int add_byte(vdcbyte c) { return 20; }; // Add one byte to "empty" instance -> Error!
    
    // Delete all data of instance
    virtual int clear() { return 0; }; // Delete all data from instance 
};


// ***************************************
// ***       Basic functions           ***
// ***************************************

// *** Parity ***

vdcbyte odd_parity(vdcbyte f);
vdcbyte even_parity(vdcbyte f);

// *** Intel Hex Format

void vdchunk_ihex_elar(FILE *fp, unsigned long int ubla);
void vdchunk_ihex_final(FILE *fp);

} // namespace vdchunk

#endif

