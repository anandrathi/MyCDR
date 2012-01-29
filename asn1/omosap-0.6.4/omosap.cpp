
//
// OMOSAP
// One More Open-Source ASN1 Parser
//
// (C) 2007-2010 by Ingo Naumann
//
// Version number see below.
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
// Revision history:
//
// Nov. 18th, 2008          -- Project re-started, now called "OMOSAP"
// Dec. 8th, 2008           -- Now functional
// Sep. 25th, 2009          -- Slight changes (valgrind, makefile)
// July 28, 2010            -- Bug fixes, OIDs
// 4 September, 2010        -- Major revision, "omosap_light2"
// January 16th, 2011       -- Now uses file "dumpasn1.cfg" for OID names
// 
//
// ToDo:
//       - Tag 0C: UTF8 String
//       - Tag 17: Time
//       - Tag A0 ..... A* 
//       - Bring order in "parse_reset()" commands
//       - Change tag length from "long int" to TAGLENGTHTYPE (unsigned long int)
//       - Length of hex blocks as parameter in command line (default: 16)
//

#include <stdlib.h> 

#include "vdchunk-asn1.h"       // Parsing procedures for certain data groups

// IN, 2011-01-16
// Old function for OID names removed
// #include "known_oids.h"         // Known OIDs
#include "known_tags.h"         // Known tags

using namespace std;
using namespace vdchunk;

// ** MAIN **

int main(int argc, char* argv[])

{
  int res;
  res = 0;

  fprintf(stdout, "** OMOSAP -- One More Open-Source ASN1 Parser (%s)**\n", 
         VDCASN1_EXTRA_INFORMATION);
  fprintf(stdout, "(C) 2007-2011 Ingo Naumann ");
//  printf("-- Version %s\n\n",PACKAGE_VERSION);
  fprintf(stdout, "-- Version 0.6.3\n\n");
// Please edit also above!

  if (argc > 2) {
                fprintf(stderr, "OMOSAP: Too many parameters.\n");
                return -1;
  }; // if argc>2

  if (argc < 2) {
                fprintf(stderr, "OMOSAP: Too few parameters.\n");
                return -1;
  }; // if argc<2

  assert(argc == 2);

  if (((string)(argv[1])) == "--help") {
                fprintf(stdout, "Parses ASN1 data file.\n\n");
        	return 0;
  };

  string filename;
  VDCHUNKSIMPLE *erg;
  VDCHUNKSIMPLE *action;
  int c,d;

  filename = (string)(argv[1]);

  erg = new VDCHUNKSIMPLE("ERG", 32768);

  action = new VDCHUNKSIMPLE("PARSEASN1_ACTION", 
	VDCHUNK_ASN1_ACTION_MAX+1); // "delete" command see below
  action->add_fill1(0, VDCHUNK_ASN1_ACTION_MAX);

  action->change_byte(VDCHUNK_ASN1_ACTION_ACCEPT_ZERO_TAGS, 1);
  action->change_byte(VDCHUNK_ASN1_ACTION_COMMENTS3, 1);
  action->change_byte(VDCHUNK_ASN1_ACTION_COMMENTS4, 1);
  action->change_byte(VDCHUNK_ASN1_ACTION_COMMENTS5, 1);
//   action->change_byte(VDCHUNK_ASN1_ACTION_COMMENTS6, 1);
  action->change_byte(VDCHUNK_ASN1_ACTION_COMMENTS_ERR, 1);

  // Read the OID values from "dumpasn1.cfg" ....
  action->change_byte(VDCHUNK_ASN1_ACTION_READ_DUMPASN1CFG, 1);

  fprintf(stdout, "Reading file %s\n", filename.c_str());

  res = erg->read_binary_file(filename);

  if (res == 0) {

  erg->write_hex_file("omosap.hex"); // 
//  res = parse_known_tags(erg);

  if (res == 0) {
		fprintf(stdout,"Parsing unknown ASN1 object ....\n");

		parse_asn1(erg,action); 
  };
	
	delete(action);
  	delete(erg);
  	return 0;
  } // no error in file
  else {
        fprintf(stderr, "OMOSAP: File error.\n");

	delete(action);
	delete(erg);
  	return -1;
  }; // error in file
}
