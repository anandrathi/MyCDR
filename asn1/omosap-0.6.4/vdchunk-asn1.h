
//
// VDCHUNK_ASN1
//
// (C) 2008-2011 Ingo Naumann
//
// Revision history and ToDo list, see ".cpp" file.
//

#ifndef VDCHUNK_ASN1_VERSION
#define VDCHUNK_ASN1_VERSION "v2-v0.4.1" // New branch (v2) on 2010-09-01

// #include <vdchunk-cryptopp.h> // 
#include <vdchunk-simple.h> // Includes "vdchunk-base.h"

#define VDCHUNKSIMPLE vdchunk_simple
#define VDCHUNK_ASN1_ACTION_TYPE_ABSTRACT vdchunk_base_change
#define VDCHUNK_ASN1_ACTION_TYPE_INSTANCE VDCHUNKSIMPLE

// see known_tags.h !! (Values 1-8)
// #define VDCHUNK_ASN1_ACTION_OUTPUT1 16 // Obsolete. Do not use.
// About the comments:
// Please note that you should use level 1 OR level 1 + level 2 or 
// level 1 + level 2 + level 3, ....
#define VDCHUNK_ASN1_ACTION_COMMENTS1 17 // Main actions (level 1)
#define VDCHUNK_ASN1_ACTION_COMMENTS2 18 // 
#define VDCHUNK_ASN1_ACTION_COMMENTS3 19 // Structure
#define VDCHUNK_ASN1_ACTION_COMMENTS4 20 // Hex dump
#define VDCHUNK_ASN1_ACTION_COMMENTS5 21 // Additional separators
#define VDCHUNK_ASN1_ACTION_COMMENTS6 22 // Additional comments (COMMENTS5 below!!)
#define VDCHUNK_ASN1_ACTION_COMMENTS_ERR 23 // Error messages
#define VDCHUNK_ASN1_ACTION_OID_IN_STRING 24
#define VDCHUNK_ASN1_ACTION_DISPLAY_LEAD0 25 // Displays a leading zero separately in a hex block
#define VDCHUNK_ASN1_ACTION_ACCEPT_ZERO_TAGS 26 // Accepts single zero bytes as tags.
#define VDCHUNK_ASN1_ACTION_READ_DUMPASN1CFG 27 // Check the values for OIDs in "dumpasn1.cfg"
// [....]
#define VDCHUNK_ASN1_ACTION_MAX 30 // This should be the highest number here

#define VDCHUNK_ASN1_DUMPASN1CFG_FILENAME "./dumpasn1.cfg" // Filename of "dumpasn1.cfg"

#define VDCASN1_EXTRA_INFORMATION VDCASN1_KNOWN_TAGS_DESCRIPTION

// These define the loquacity of the parser
#define VDCHUNK_ASN1_COMMENTS1 // Main actions
#define VDCHUNK_ASN1_COMMENTS2 // (Unused?)
#define VDCHUNK_ASN1_COMMENTS3 // Structure
#define VDCHUNK_ASN1_COMMENTS4 // Hex bytes
#define VDCHUNK_ASN1_COMMENTS_ERR // Error messages
#define VDCHUNK_ASN1_COMMENTS4a // Additional separators
// #define VDCHUNK_ASN1_COMMENTS5 // Additional comments 
#define VDCHUNK_ASN1_CHARPERLINE 16 // Number of hex bytes per line
#define VDCHUNK_ASN1_CHARTOTAL 80 // Maximum number of hex bytes displayed (0=all)
#define VDCHUNK_ASN1_STREAM stdout
#define VDCHUNK_ASN1_ERROR_STREAM stderr

// #define VDCHUNK_ASN1_DEBUG_MSG2 // Debug messages
// #define VDCHUNK_ASN1_DEBUG_MSG3 // Debug messages
// #define VDCHUNK_ASN1_DEBUG_MSG4 // Debug messages
// #define VDCHUNK_ASN1_DEBUG_MSG5 // Debug messages
// #define VDCHUNK_ASN1_DEBUG_MSG6 // Debug messages
#define VDCHUNK_ASN1_DEBUG_STREAM stdout

using namespace std;
using namespace vdchunk;

int parse_asn1(vdchunk_base *tag); // Ok
int parse_asn1(vdchunk_base *tag, VDCHUNK_ASN1_ACTION_TYPE_ABSTRACT *action); // Ok
int parse_asn1(vdchunk_base *tag, int depth, long int *taglength, 
		VDCHUNK_ASN1_ACTION_TYPE_ABSTRACT *action); // Ok

// int parse_asn1_set(vdchunk_base *tag, int depth, 
// 	VDCHUNK_ASN1_ACTION_TYPE_ABSTRACT *action);
// int check_and_parse_asn1_set(vdchunk_base *tag, int depth, 
//	VDCHUNK_ASN1_ACTION_TYPE_ABSTRACT *action);

// Additional functions

// Check whether this might be a valid ASN1 tag
bool is_asn1_tag(vdchunk_base *tag);
// Check whether this is a sequence of ASN1 tags
bool is_sequence_of_asn1_tags(vdchunk_base *tag, VDCHUNK_ASN1_ACTION_TYPE_ABSTRACT *action);
bool is_sequence_of_asn1_tags_not_strict(vdchunk_base *tag, VDCHUNK_ASN1_ACTION_TYPE_ABSTRACT *action);
// Returns the length of the first TLV tag (two versions, see parameters)
unsigned long int tlv_length(vdchunk_base *tag, VDCHUNK_ASN1_ACTION_TYPE_ABSTRACT *action);
long int tlv_length2(vdchunk_base *tag);

bool is_asn1_UTCTime_tag(char c);
bool is_asn1_UTCTime(vdchunk_base *tag);
bool is_asn1_set_tag(char c);
bool is_asn1_set(vdchunk_base *tag);
bool is_asn1_sequence_tag(char c);
bool is_asn1_sequence(vdchunk_base *tag);
bool is_asn1_null_tag(char c);
bool is_asn1_null(vdchunk_base *tag);
bool is_eoc_tag(vdchunk_base *tag);
bool is_zero_tag(vdchunk_base *tag);

bool is_asn1_high_tag_number_form(vdchunk_base *tag);
// Deprecated version of the same command:
#define pp_asn1_high_tag_number_form is_asn1_high_tag_number_form

// Helper functions

string asn1_simple_spaces(int depth);

// Cleaning up ...

// bool remove_leading_zero_of_hex_block(vdchunk_base* tag, int depth, VDCHUNK_ASN1_ACTION_TYPE_ABSTRACT *action);
bool check_and_parse_sequence_of_asn1_tags(vdchunk_base *tag, int depth, bool strict, 
	VDCHUNK_ASN1_ACTION_TYPE_ABSTRACT *action);
void parse_known_asn1_tag_as_hex(vdchunk_base *tag, string tagname, int depth, 
                                 long int *taglength, VDCHUNK_ASN1_ACTION_TYPE_ABSTRACT *action);
void parse_known_asn1_tag(vdchunk_base *tag, string tagname, int depth, long int *taglength, VDCHUNK_ASN1_ACTION_TYPE_ABSTRACT *action);
int parse_known_asn1_tag_as_sequence_or_hex(vdchunk_base *tag, string tagname, int depth, long int *taglength, VDCHUNK_ASN1_ACTION_TYPE_ABSTRACT *action);

void vdchunk_asn1_write_hex(vdchunk_base *tag, int depth);
long int parse_asn1_length(vdchunk_base *tag);
long int parse_asn1_length_of_eoc_terminated_sequence(vdchunk_base *tag);
long int parse_asn1_tag_number(vdchunk_base *tag);

// 02: Integer
bool is_asn1_integer_tag(char c);
bool is_asn1_integer(vdchunk_base *tag);
bool check_and_parse_asn1_integer(vdchunk_base *tag, int depth, 
	long int *taglength, VDCHUNK_ASN1_ACTION_TYPE_ABSTRACT *action);
void parse_asn1_integer(vdchunk_base *tag, int depth, long int *taglength, 
                         VDCHUNK_ASN1_ACTION_TYPE_ABSTRACT *action);

// 03: Bit String
bool is_asn1_bit_string_tag(char c);
bool is_asn1_bit_string(vdchunk_base *tag);
bool check_and_parse_asn1_bit_string(vdchunk_base *tag, int depth, 
	long int *taglength, VDCHUNK_ASN1_ACTION_TYPE_ABSTRACT *action);
void parse_asn1_bit_string(vdchunk_base *tag, int depth, long int *taglength, 
				VDCHUNK_ASN1_ACTION_TYPE_ABSTRACT *action);

// 04: Octet String
bool is_asn1_octet_string_tag(char c);
bool is_asn1_octet_string(vdchunk_base *tag);
bool check_and_parse_asn1_octet_string(vdchunk_base *tag, int depth, 
	long int *taglength, VDCHUNK_ASN1_ACTION_TYPE_ABSTRACT *action);
void parse_asn1_octet_string(vdchunk_base *tag, int depth, long int *taglength, 
				VDCHUNK_ASN1_ACTION_TYPE_ABSTRACT *action);

// 03/04: Bit String or Octet String
bool is_asn1_boo_string_tag(char c);
bool is_asn1_boo_string(vdchunk_base *tag);
bool check_and_parse_asn1_boo_string(vdchunk_base *tag, int depth, 
	long int *taglength, VDCHUNK_ASN1_ACTION_TYPE_ABSTRACT *action);
void parse_asn1_boo_string(vdchunk_base *tag, int depth, long int *taglength, 
				VDCHUNK_ASN1_ACTION_TYPE_ABSTRACT *action);

// 06: Object Identifier (OID)
bool is_asn1_oid_tag(char c);
bool is_asn1_oid(vdchunk_base *tag);
bool check_and_parse_oid(vdchunk_base *tag, int depth, long int *taglength,
				VDCHUNK_ASN1_ACTION_TYPE_ABSTRACT *action);
void parse_asn1_oid(vdchunk_base *tag, int depth, long int *taglength, 
				VDCHUNK_ASN1_ACTION_TYPE_ABSTRACT *action);

// "ASN1 string", e.g. 16: IA5String, 13: PrintableString, 0C: UTF8String
bool is_asn1_string_tag(char c);
bool is_asn1_string(vdchunk_base *tag);
/*
bool check_and_parse_asn1_string(vdchunk_base *tag, int depth, long int *taglength,
     vdchunk_base *action);
void parse_asn1_string(vdchunk_base *tag, int depth, long int *taglength, 
     vdchunk_base *action);
*/
	
// OID

string dumpasn1(string oid);

// ** Global variables **
string return_vdc_global_string1();

// IN, 2011-01-16
// Old function for OID names removed
// #include "known_oids.h"

#include "known_tags.h"

#endif
