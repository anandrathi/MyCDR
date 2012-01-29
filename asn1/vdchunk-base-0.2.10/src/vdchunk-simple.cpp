//
// VDChunk -- The Versatile Data Chunk
//
// Copyright (C) 2005-2008 Ingo Naumann
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


#include "vdchunk-simple.h"

using namespace std;

namespace vdchunk {

// *****************************************
// ***           Methods                 ***
// *****************************************

// *****************************************
// *** Methods of the Class "vdchunk_simple" ***
// *****************************************

vdchunk_simple::vdchunk_simple(string fname) 

//! Constructor of the class "vdchunk_simple".

//! \param fname Name of the instance

{ 

#ifdef VDCSIMPLECOMMENT4
  cout << "Instance " << fname << " of class VDCHUNK_SIMPLE created." << endl;
#endif

  // please see also below (copy)
  name = fname;
  dysfunctional = 0;
  initialbuffersize = 1024;
  init_list(initialbuffersize);

  this->parse_reset();
}

vdchunk_simple::vdchunk_simple(string fname, unsigned long int size) 

//! Constructor of the class "vdchunk_simple".

//! \param fname Name of the instance
//! \param size Initial buffer size

{ 

#ifdef VDCSIMPLECOMMENT4
  cout << "Instance " << fname << " of type VDCHUNK_SIMPLE (size " << size <<") created." << endl;
#endif

  // please see also above (copy)
  name = fname;
  dysfunctional = 0;
  initialbuffersize = size;
  init_list(initialbuffersize);

  this->parse_reset();
}

vdchunk_simple::~vdchunk_simple()

//! Destructor of the class "vdchunk_simple".

{ 
#ifdef VDCSIMPLECOMMENT4
  cout << "Instance of class VDCHUNK_SIMPLE " << this->name << " deleted." << endl;
#endif

  delete_list();
}

// *** Data Structure ***

void vdchunk_simple::init_list(unsigned long int initialsize)

//!  Initialise list (= memory for buffer)

//! \param initialsize Initial size of buffer
     
{
    maxvalu = initialsize; // initial size of list
    entries = 0;           // empty at this point
    
    
    try {
    	BLK = new vdcbyte [maxvalu];
    }
    catch (std::bad_alloc what) {
	// Memory full

	// Declare instance as dysfunctional
	this->dysfunctional = 1;

#ifdef VDCSIMPLECOMMENTERROR
	cout << name << " : Memory full! " << endl;
#endif
    };

    return;
}

void vdchunk_simple::delete_list()

//! Delete the list
     
{
#ifdef VDCCRYPTOCOMMENT6
    cout << name << ".vdchunk_simple::delete_list()" << endl;
#endif

    delete[] BLK;
    BLK = NIL;

    return;
}

int vdchunk_simple::clear_list(unsigned long int size)

//! Clear buffer and set up data structure again (subroutine of "clear")

//! \param size New list size
     
{
#ifdef VDCSIMPLECOMMENT5
    cout << name << ".vdchunk_simple::clear_list(" << size << ")" << endl;
#endif
  
    delete_list(); // Delete all data
    init_list(size);   // Set up data structure
  
    // Let's just assume that "init_list" always works ....
    return 0;
}

int vdchunk_simple::clear()

//! Clear buffer and set up data structure again
     
{
#ifdef VDCSIMPLECOMMENT5
    cout << "vdchunk::clear() [initialbuffersize:" << initialbuffersize << "]" << endl;
#endif
  
    this->clear_list(initialbuffersize);
    this->parse_reset();
    
    return 0;
}

int vdchunk_simple::make_more_space()

//! Increases the size of the buffer.

//! \return 0 = ok, 1 = Error (not enough space)

{
#ifdef VDCCRYPTOCOMMENT7
    cout << name << ".vdchunk_simple::make_more_space()";
    cout << " [maxvalu: " << maxvalu << "]" << endl;
#endif

  int               errcode = 0;
  unsigned long int l, newmv;

  // New size of list
  newmv = 2*maxvalu+2;

  // Temporary pointer to list
  vdcbyte* BLKtemp;

  try {
  // Allocate memory
	BLKtemp = new vdcbyte[newmv];
  }
  catch (std::bad_alloc what) {
	errcode = 1;
  };

  if (errcode != 0) {
   // Memory full?
   
	// Try this one ...
	newmv = (unsigned long int)(1.5*maxvalu+2);
	try {
		BLKtemp = new vdcbyte [newmv];
		errcode = 0;
	}
	catch (std::bad_alloc what) {
	errcode = 1;

#ifdef VDCSIMPLECOMMENTERROR
	cout << name << " : Memory full! " << endl;
#endif

	}; // catch
  }; // if errcode<>0

  if (errcode == 0) {
	// If one of the two commands has been successful....
	// .... copy contents of old list into new one

	 // Loop over old data
	for (l = 0; l<maxvalu; l++) {
		BLKtemp[l] = BLK[l];                 
	}; // for

	// Delete old list
	delete[] BLK;
	// Change pointers
	BLK = BLKtemp;
	// Keep new size in mind
	maxvalu = newmv;
    
#ifdef VDCSIMPLECOMMENT3
    cout << name << " : New maxvalu: " << maxvalu << endl;
#endif

  }; 

  return errcode;
}

char vdchunk_simple::is_dysfunctional()

//! Indicates whether instance is dysfunctional or not (e.g. because there was not
//! enough memory during initialisation)

//! \return 0 = ok, otherwise dysfunctional

{
	return this->dysfunctional;
}

int vdchunk_simple::add_byte(vdcbyte c)

//! Adds one byte to buffer.

//! \param c Byte
//! \return 0 = success, otherwise error

{

    int errcode = 0;

    if (this->dysfunctional > 0) {
	// Instance is dysfunctional, no adding possible

	errcode = 23; // see list of error codes in "vdchunk-simple.h"
	return errcode;
    };

    // Check whether block size is big enough
    // to store one more vdcbyte
    if (entries >= maxvalu) { 
            // call "make_more_space" if not
            errcode = make_more_space();
            // return exit code if this fails
            if (errcode != 0) return errcode;
    };

    assert(entries < maxvalu);
    
    // Store byte
    BLK[entries] = c;
    // Increase byte counter
    entries++;
    
    // Operation successful
    return 0;
}

/* 
XXX-INSERT

int vdchunk_simple::insert_byte(unsigned long int position, vdcbyte value)

//! Inserts one byte at a given position. All subsequent bytes will be 
//! moved.
//! 
//! \param position Position
//! \param value Byte that will be inserted.

{
    if (position <= entries) {
    }
    else {
    	// "position" outside of block
    	// behaviour t.b.d.
    	return XXX;
    };
}

*/

int vdchunk_simple::del_byte(unsigned long int position)

//! Deletes one byte and moves all subsequent bytes one position backwards.
//! If "position" is outside of the buffer, the command will be ignored.

//! \return 0

{
 unsigned long int loop;

 if (position < entries) {
  for (loop = position; loop < entries-1; loop++) {
   BLK[loop] = BLK[loop+1]; };
  entries--;
		if (entries < parsec) {
			parsec = entries;
		};
 };
}

int vdchunk_simple::del_bytes(unsigned long int from, unsigned long int to)

//! Deletes some bytes and moves all subsequent bytes 
//! several position backwards; both, "to" and "from",
//! are included, i.e. "del_bytes(10,14)" would remove
//! five bytes (if there are at least 15 entries in
//!  the buffer)
  
//! Return value: 0 -- ok, 19 -- position outside of buffer

{
 unsigned long int loop;

 if (from <= to) {
	if (from < entries) {
		if (to >= entries) to = entries-1;
			for (loop = from; loop < entries; loop++) {
				BLK[loop] = BLK[loop+(to-from+1)]; 
			};
	  		entries = entries - (to-from+1);
	 	};
		if (entries < parsec) {
			parsec = entries;
		};
		return 0;
	} 
	else {
		return 19; // Error: Illegal positions in data block ("from" > "to")
	};
}

vdcbyte vdchunk_simple::get_byte(unsigned long int position)

//! Gets one byte from a given position in the data block
//! (return value). Returns zero, if position is outside of
//! data block.
//! 
//! \param position Position

{
    if (position < entries)
        {
            return BLK[position];
        }
        else
        {
            return (vdcbyte)0;
        };
}

/*
vdcbyte *vdchunk_simple::get_pointer()
// *** DEPRECATED *** I.N., June 26th, 2009
{ return BLK; }
*/

int vdchunk_simple::change_byte(unsigned long int position, vdcbyte value)

//! Change one value of the field; note that this field must 
//! already "contain" a byte, here that means that "position" 
//! must be smaller than "entries".

{
    if (position < entries)
        {
     BLK[position] = value;
            return 0; // ok
        }
        else
        {
            return 14; // Error: Incorrect position
        };
}

// *** Copying ***


int vdchunk_simple::copy(vdchunk_base *source)

        /*
         * Make this instance a copy of another "vdchunk" instance.
         * Do not append. The size of "this" will be the number of
         * entries of the other instance.
         *
         * Parameters:
         *             source   - source object
         *
         * Return value:
         *             0        - if successful
         *
         */

{
        // Clear buffer first; do not append
        this->clear_list(source->entries);
        // Copy the entire content of "source"
        // into the buffer;
        return add_copy_bytes(source, 0, source->entries);
}

// *** Modifying ***

void vdchunk_simple::xor_vdchunk(vdchunk_base *mask)

//! Deprecated. Do not use.

{
	unsigned long int loop, me;
	int               res;

	// If buffer lengths are different, choose the smaller one
	me = mask->entries;
	if (entries < me) { me = entries; };

	for (loop = 0; loop < me; loop++) {

		// read vdcbyte, calculate operation and write it back
		res = change_byte(loop, (get_byte(loop) ^ mask->get_byte(loop)));

		assert(res == 0); // shouldn't be a problem
		};

	return;
}

int vdchunk_simple::xor_operation(vdchunk_base *mask)

//! Perform the "XOR" operation of the buffer with a given data block.
//! 
//! \param mask Data block to be combined with the buffer.

{
        unsigned long int loop1, loop2, me;
        int               res;

        me = mask->entries; // Number of mask entries
        if (me == 0) return 0; // If zero, do nothing

        loop2 = 0;
        for (loop1 = 0; loop1 < entries; loop1++) {

                // read vdcbyte, calculate operation and write it back
                res = change_byte(loop1, (get_byte(loop1) ^ mask->get_byte(loop2)));
                if (res != 0) { return res; };

                // When end of mask buffer reached, start from the beginning
                assert(loop2 < me);
                loop2++;
                if (loop2 == me) { loop2 = 0; };
                };

        return 0;
}

void vdchunk_simple::or_vdchunk(vdchunk_base *mask)

//! Deprecated. Do not use.

{
	unsigned long int loop, me;
	int               res;

	// If buffer lengths are different, choose the smaller one
	me = mask->entries;
	if (entries < me) { me = entries; };

	for (loop = 0; loop < me; loop++) {

		// read vdcbyte, calculate operation and write it back
		res = change_byte(loop, (get_byte(loop) | mask->get_byte(loop)));

		assert(res == 0); // shouldn't be a problem
		};

	return;
}

int vdchunk_simple::or_operation(vdchunk_base *mask)

//! Perform the "OR" operation of the buffer with a given data block.
//! 
//! \param mask Data block to be combined with the buffer.

{
        unsigned long int loop1, loop2, me;
        int               res;

        me = mask->entries; // Number of mask entries
        if (me == 0) return 0; // If zero, do nothing

        loop2 = 0;
        for (loop1 = 0; loop1 < entries; loop1++) {

                // read vdcbyte, calculate operation and write it back
                res = change_byte(loop1, (get_byte(loop1) | mask->get_byte(loop2)));
                if (res != 0) { return res; };

                // When end of mask buffer reached, start from the beginning
                assert(loop2 < me);
                loop2++;
                if (loop2 == me) { loop2 = 0; };
                };

        return 0;
}

void vdchunk_simple::and_vdchunk(vdchunk_base *mask)

//! Deprecated. Do not use.

{
	unsigned long int loop, me;
	int               res;

	// If buffer lengths are different, choose the smaller one
	me = mask->entries;
	if (entries < me) { me = entries; };

	for (loop = 0; loop < me; loop++) {

		// read vdcbyte, calculate operation and write it back
		res = change_byte(loop, (get_byte(loop) & mask->get_byte(loop)));

		assert(res == 0); // shouldn't be a problem
		};

	return;
}

int vdchunk_simple::and_operation(vdchunk_base *mask)

//! Perform the "AND" operation of the buffer with a given data block.
//! 
//! \param mask Data block to be combined with the buffer.

{
        unsigned long int loop1, loop2, me;
        int               res;

        me = mask->entries; // Number of mask entries
        if (me == 0) return 0; // If zero, do nothing

        loop2 = 0;
        for (loop1 = 0; loop1 < entries; loop1++) {

                // read vdcbyte, calculate operation and write it back
                res = change_byte(loop1, (get_byte(loop1) & mask->get_byte(loop2)));
                if (res != 0) { return res; };

                // When end of mask buffer reached, start from the beginning
                assert(loop2 < me);
                loop2++;
                if (loop2 == me) { loop2 = 0; };
                };

        return 0;
}

void vdchunk_simple::inc()

{
	unsigned long int loop;
	vdcbyte              c;

	loop = entries;
	while (loop != 0) {
		c = get_byte(loop-1);
		if (c == 255) { 
			change_byte(loop-1, 0);
			loop--; }
		else {
			change_byte(loop-1, c+1);
			loop=0; }
		}; // while
	return;
}

// *** Writing ***

int vdchunk_simple::write_binary_to_port(int fd)

//! Writes the content of the buffer to an open port. This method overloads the
//! method defined in "vdchunk_base".

//! \param fd File descriptor of port

{

#ifdef FFACOMMENT5
	cout << "vdchunk_simple::write_binary_to_port()" << endl;
#endif

	int res;

	res = write(fd, BLK, this->entries);

	return res; // Exit: No errors
}

} // namespace
