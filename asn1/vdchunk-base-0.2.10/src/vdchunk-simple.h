  
//
// VDCHunk -- The Versatile Data Chunk
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
// You should have received a copy of the GNU General Public
// License along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
//
// History:
//
//      Feb. 26th, 2006           - V0.9 - Class VDCHUNK_CRYPTO created. This is
//                                         a work-around. A realization of VDCHUNK_CRYPTO
//                                         as a derived class from VDCHUNK would be better.
//      Mar. 3rd, 2006        - V0.9a - Added a lot of things.
//      April 20th, 2006      - Separated VDCHUNK_CRYPTO and VDCHUNK_ABSTRACT
//      July 30th, 2006       - Renamed from  to VDCHUNK
//      Aug 6th, 2006         - Functions OR_VDCHUNK and AND_VDCHUNK
//      September 3rd, 2006   - Separated VDCHUNK-SIMPLE from VDCHUNK-CRYPTO 
//                              (VDCHUNK-BASE version 0.1.1)
//      Sept. 30th, 2008      - Integrated into VDCHUNK-CRYPTOPP
//      Oct. 2nd, 2008        - XOR_OPERATION, etc.
//      Oct. 17th, 2008       - "add_padding02", "delete_padding02"
//      Nov. 17th, 2009       - Bug-fix: "delete [] BLK" in "vdchunk-simple"
//      Oct. 12th, 2010       - Bug-fix: "reset_parse" in all contructors 
//                              of "vdchunk_simple" (vdcs-0.2.4)
//      Oct. 23th, 2010       - "vdchunk_write_binary_stream2()"
//      Oct. 28th, 2010       - Bug fix "del_bytes" / parsec
//
// To do:
//      - proper library
//      - function "even_parity_all"
//

#ifndef VDC_SIMPLE_VERSION
#define VDC_SIMPLE_VERSION "vdcs-0.2.6"

#include "vdchunk-base.h" // Base class

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <assert.h>

using namespace std;

namespace vdchunk {

#define NIL 0
// #define vdcbyte unsigned char
// Already defined in "vdchunk_base"

// #define VDCSIMPLECOMMENT3     // Comments level 3
// #define VDCSIMPLECOMMENT4     // Comments level 4: Contructors, destructors
// #define VDCSIMPLECOMMENT5     // Comments level 5
// #define VDCSIMPLECOMMENT7     // Comments level 7
// #define VDCSIMPLECOMMENTERROR // Error messages

// ***************************************
// ***         Class Definitions       ***
// ***************************************

// *********************************************
// *** Class Definition of "vdchunk_simple"  ***
// *********************************************

class vdchunk_simple : public vdchunk_base_del {

protected:

// *** Data Structure ***

    // Current buffer size
    unsigned long int maxvalu;
    
    // List that contains data
    vdcbyte *BLK;
    // Please note: do not move this pointer into "public"; use
    //              "vdcpointer" instead (see "vdchunk-base.h")

    // Initial value for buffer size
    unsigned long int initialbuffersize;
    
    // Initialise list
    virtual void init_list(unsigned long int initialsize);
    // Delete list
    virtual void delete_list();
        
    // Resize list
    virtual int make_more_space();
 
    // Delete list and create a new one with given
    // size as parameter (subroutine of "clear")
    virtual int clear_list(unsigned long int size);

    // Indicates whether instance is dysfunctional or not
    // (e.g. because there wasn't enough space)
    char dysfunctional;

public:

// *** Constructor and Destructor ***

    // The easiest way ...
    vdchunk_simple(string fname);
    // Suggest buffer size
    vdchunk_simple(string fname, unsigned long int size);
    virtual ~vdchunk_simple();

// *** Data Structure ***

    // Delete all data of instance
    virtual int clear();
    
    // Add one byte to buffer
    virtual int add_byte(vdcbyte c);
    // (see "vdchunk-base.h" for other ways to add content to the buffer)

    // Delete bytes (and move all subsequent bytes backwards)
    virtual int del_byte(unsigned long int position);
    virtual int del_bytes(unsigned long int from, unsigned long int to);

    // Read one byte
    virtual vdcbyte get_byte(unsigned long int position);

    // virtual vdcbyte *get_pointer();
    // Removed IN, June 26th, 2009

    virtual int request_vdcpointer() { return 0; };
    virtual vdcbyte *vdcpointer() { return BLK; };
    virtual void release_vdcpointer() { };
    virtual void check_vdcpointer_always_available() { return; } 
    virtual void check_vdcpointer_sometimes_available() { return; } 
    
    // Change one byte (must have been set before, i.e. "position<entries")
    virtual int change_byte(unsigned long int position, vdcbyte value);

    // Check whether instance is dysfunctional (e.g. because there is not enough memory)
    virtual char is_dysfunctional();

// *** Copying ***

    // Copy one "vdchunk" into another (do not append)
    virtual int copy(vdchunk_base *source);

// *** Modifying ***

    // Deprecated. Do not use.
    virtual void xor_vdchunk(vdchunk_base *mask);
    virtual void or_vdchunk(vdchunk_base *mask);
    virtual void and_vdchunk(vdchunk_base *mask);

    // Perform logical operations of the buffer with a given data block.
    virtual int xor_operation(vdchunk_base *mask);
    virtual int or_operation(vdchunk_base *mask);
    virtual int and_operation(vdchunk_base *mask);

    virtual void inc();

// *** Writing ***

    // Write block buffer to binary file
    virtual int write_binary_to_port(int fd);

// *** Subroutines ***

};

// ***************************************
// *** Error codes of VDCHUNK_BASE     ***
// ***************************************

// 2 - "read_..." : File error

// ***************************************
// *** Error codes of VDCHUNK_SIMPLE   ***
// ***************************************

// 1 - "make_more_space" : Memory full
// 4 - "add_hex_string" : Unexpected end of string
// 5 - "add_hex_string" : Error in hex number
// 6 - "add_copy_bytes" : Wrong "start" and "length" values (source object too small)
// 14 - "change_value" : Incorrect position
// 18 - "delete_padding.." : Error removing pad bytes (since v 0.2.0)
// 19 - "del_bytes" : Illegal positions in block
// 20 - "add..." : User tried to add bytes to an instance of "vdchunk_base_cutout"
//                 or "vdchunk_empty"
// 23 - "add_byte" : Instance is dysfunctional (due to errors during initialization)

// 99 - Function not implemented yet

// *****************************************
// *** Error codes of VDCHUNK_CRYPTOPP   ***
// *** (informative, do not add here)    ***
// *****************************************

// 11 - "[...]en-/decipher" : incorrect key length 
// 99 - Function not implemented yet

// ***************************************
// *** Error codes of VDCHUNK_CRYPTO   ***
// *** (informative, do not add here)  ***
// ***************************************

// 8 - "hash" : unable to open hash procedure
// 9 - "hash" : hash error
// 10 - "en-/decipher" : unable to open cipher procedure
// 12 - "en-/decipher" : cipher error
// 13 - "en-/decipher" : class of "key" or "IV" does not support "pointer" method
// 14 - "change_value" : incorrect position
// 15 - "icao_mac/icao_mac2" : wrong key length
// 16 - "icao_mac2" : wrong SSC length
// 17 - "hash" : Unknown hashing algorithm

// 50 - "hash" : Parameter is NIL
// 51 - "hash" : Pointer to data block is NIL

} // namespace vdchunk

#endif


