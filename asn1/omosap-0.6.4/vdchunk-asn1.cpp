
//
// VDCHUNK_ASN1 v2
//
// (C) 2008-2010 Ingo Naumann
//
// V0.3.0
//
// Revision history:
//        Nov. 06th, 2008       V0.1     - First version of this file.
//        July 23rd, 2009       V0.1.4   - Some action tags, UTCTime, etc.
//        Aug. 8th, 2009        V0.1.5   - UTCTime as string
//        Dec. 5th, 2009        V0.1.6   - VDCHUNK_ASN1_COMMENTS replaced by actions
//        Aug 15, 2010          v0.1.7   - Update, bug fixes
//        Aug 20, 2010          v0.1.8   - "parse_asn1" removes leading zero
//        1 Sep, 2010           v0.2.0   - Complete renewal of ASN1 parsing procedures
//        11 Sep, 2010          v0.3.0   - Indefinite tags
//
// ToDo:
//
//  - Create new class: vdchunk_asn1; convert functions xxx(tag) in tag->xxx
//  - Version number with #define
//  - Tags of more than two bytes (XXYc)
//  - String und UTCTime "aufraeumen"
//  - More efficient solution for "tabs", see "string_asn1_spaces"
//  - Support for tags with T longer than 2 bytes
//  - Still a bit messy: some functions reset parse counter, others don't (XXYa)
//        [DEPRECATED: "is" --> "pp" when parsing position (not yet implemented everywhere)]
//        "..._tag()" or "..._tags" when NOT parsing position, e.g. "check_and_parse_integer"
//        vs. "check_and_parse_integer_tag";
//  - Distinguish 'VDCHUNKSIMPLE' and "vdchunk_simple'
//  - Clean up the mess with "taglength"; not needed; (XXYd)
//  - Length of hex blocks in action
//  - Instead of "known_tags" include "dumpasn1.cfg"
//  - intest1.bin: 04 04 00 00 00 00
//  - intest1.bin: 30 02 30 00
//

#include "vdchunk-asn1.h"

// ** Global variables **
string vdc_global_string1;

string return_vdc_global_string1()
{
	return vdc_global_string1;
}

int parse_asn1(vdchunk_base *tag)

//! Parses ASN1 tag.
//!
//! \param tag Tag to be parsed 
//! \return 0, if all is ok

{
#ifdef VDCHUNK_ASN1_DEBUG_MSG2
	fprintf(VDCHUNK_ASN1_DEBUG_STREAM, "parse_asn1(%s) [a] Length: 0x%04X\n", 
		tag->name.c_str(), (int)(tag->entries));
#endif

	int res;
        vdchunk_simple *simple_action;
        simple_action = new vdchunk_simple("PARSEASN1_ACTION", 
		VDCHUNK_ASN1_ACTION_MAX+1); // "delete" command see below
	// No actions
	simple_action->add_fill1(0, VDCHUNK_ASN1_ACTION_MAX);

	res = parse_asn1(tag, simple_action);

        delete (simple_action);
	return res;
}

int parse_asn1(vdchunk_base *tag, VDCHUNK_ASN1_ACTION_TYPE_ABSTRACT *action)

//! Parses ASN1 tag.
//!
//! \param tag Tag to be parsed 
//! \param action Actions to be performed
//! \return 0, if all is ok

{
#ifdef VDCHUNK_ASN1_DEBUG_MSG3
	fprintf(VDCHUNK_ASN1_DEBUG_STREAM, 
	"parse_asn1(%s, action) [b] Length: 0x%04X\n", tag->name.c_str(), (int)(tag->entries));
#ifdef VDCHUNK_ASN1_DEBUG_MSG6
	if (action != NIL) {
		fprintf(VDCHUNK_ASN1_DEBUG_STREAM, " Action: %s\n", action->name.c_str()); 
		if (action->get_byte(VDCHUNK_ASN1_ACTION_ACCEPT_ZERO_TAGS) != 0) {
			fprintf(VDCHUNK_ASN1_DEBUG_STREAM, " Accepts zero tags: yes\n"); 
		} else {
			fprintf(VDCHUNK_ASN1_DEBUG_STREAM, " Accepts zero tags: no\n");  };
	} else {
		fprintf(VDCHUNK_ASN1_DEBUG_STREAM, " Action: NIL\n", action->name.c_str()); };
#endif
#endif

	long int len;
	// Default value for depth is zero
	return parse_asn1(tag, 0, &len, action);
}

int parse_asn1(vdchunk_base *tag, int depth, long int *taglength, VDCHUNK_ASN1_ACTION_TYPE_ABSTRACT *action)

//! Parses ASN1 tag.
//!
//! \param tag Tag to be parsed 
//! \param depth Parse depth
//! \param action Actions to be performed
//! \param taglength Returns pointer to tag length (TLV)
//! \return 0, if all is ok

{
#ifdef VDCHUNK_ASN1_DEBUG_MSG3
	fprintf(VDCHUNK_ASN1_DEBUG_STREAM, "parse_asn1(%s) [c] Length: 0x%04X, Depth: %d\n", 
		tag->name.c_str(), (int)(tag->entries), depth);
#endif

	int               c, d, r;
	unsigned long int loop, count, rlen;
	long int len, slen, dummy;
	string tabs, oidstring, oidname, s;
	vdchunk_base_cutout *next, *next2;
	bool finished, isseq, isset;

	tabs = asn1_simple_spaces(depth);

	tag->parse_reset();

	// Settings:
	action->change_byte(VDCHUNK_ASN1_ACTION_DISPLAY_LEAD0, 1);
	action->change_byte(VDCHUNK_ASN1_ACTION_ACCEPT_ZERO_TAGS, 1);

	if (pp_asn1_high_tag_number_form(tag)) {
		// ** High tag number form
		parse_known_asn1_tag_as_sequence_or_hex(tag, "Tag", depth, taglength, action);
	}
	else {
	// ** Low tag number form
	
	isset = is_asn1_set(tag);
	isseq = is_asn1_sequence(tag);

	if ((isseq) || (isset)) {
		// * SEQUENCE (OF) / SET (OF)

		if (isseq) {
			s = "SEQUENCE";
		}
		else {
			s = "SET";
		};

		r = parse_known_asn1_tag_as_sequence_or_hex(tag, s, depth, 
			taglength, action);

		if (r != 0) {
	        	if (action->get_byte(VDCHUNK_ASN1_ACTION_COMMENTS3) != 0) {
			 	fprintf(VDCHUNK_ASN1_STREAM,
				"%s!! : Error in sequence/set! (%d)\n", tabs.c_str(), r);
			};
			return -1;
		};

	} // SEQUENCE
	else { 
		if (is_asn1_oid(tag)) {

			// * Object Identifier (OID)

			parse_asn1_oid(tag, depth, &dummy, action);

/*
			count = tag->remaining_parse_length();

			c = tag->parse_look_ahead();
			oidstring = tag->parse_object_identifier();

			count = count - tag->remaining_parse_length();
			*taglength = count; // return value

//			IN, 2010-12-23
//			Old function for OID names removed.
//			oidname = oid(oidstring);
			
			oidname = dumpasn1(oidstring);

	        	if (action->get_byte(VDCHUNK_ASN1_ACTION_OID_IN_STRING) != 0) {
				vdc_global_string1 = oidname;
			};

       		 	if (action->get_byte(VDCHUNK_ASN1_ACTION_COMMENTS3) != 0) {
				// Comments level 3
				fprintf(VDCHUNK_ASN1_STREAM,
				"%s%02X : Object Identifier (OID): %s ", 
				tabs.c_str(), c, oidstring.c_str());
				if (oidname != "Unknown") {
					fprintf(VDCHUNK_ASN1_STREAM,"%s", oidname.c_str());
				}	
				fprintf(VDCHUNK_ASN1_STREAM,"\n");
			};
*/

		}
		else {
		
		if (is_asn1_null(tag)) {

			// * NULL

			count = tag->remaining_parse_length();

			c = tag->parse_read();
//			len = tag->parse_asn1_length(); // Should be zero....
			len = parse_asn1_length(tag);

			count = count - tag->remaining_parse_length();
			*taglength = len + count; // return value

        		if (action->get_byte(VDCHUNK_ASN1_ACTION_COMMENTS3) != 0) {
				// Comments level 3
				fprintf(VDCHUNK_ASN1_STREAM,"%s%02X : NULL\n", 
					tabs.c_str(), c);
			};
	        	if (action->get_byte(VDCHUNK_ASN1_ACTION_COMMENTS_ERR) != 0) {
				if (len != 0) {
					fprintf(VDCHUNK_ASN1_STREAM,
					"%s   : Warning: NULL tag without zero length!\n",
					tabs.c_str());
				}
			};

			// No action with the content
			tag->parse_skip_bytes(len);
		}
		else {

		if (is_eoc_tag(tag)) {

			// * EOC tag (00 00)
			// (This has to come before 'zero tag')

			c = tag->parse_read();
			c = tag->parse_read();
			len = 0;
			*taglength = 2; // return value

        		if (action->get_byte(VDCHUNK_ASN1_ACTION_COMMENTS3) != 0) {
				// Comments level 3
				fprintf(VDCHUNK_ASN1_STREAM,"%s00 00 : EOC\n", 
					tabs.c_str(), c);
			};
		} 
		else {
		
		if ((action->get_byte(VDCHUNK_ASN1_ACTION_ACCEPT_ZERO_TAGS) != 0) && (is_zero_tag(tag))) { 
			
			// * Zero tag, not ASN1 compliant

			c = tag->parse_read();
			len = 0;
			*taglength = 1; // return value

        		if (action->get_byte(VDCHUNK_ASN1_ACTION_COMMENTS3) != 0) {
				// Comments level 3
				fprintf(VDCHUNK_ASN1_STREAM,"%s00 : Zero tag\n", 
					tabs.c_str(), c);
			};

			// No action, no content
		}
		else {

		if ((is_asn1_string(tag) || is_asn1_UTCTime(tag))) {

			// * PrintableString, UTF8String
			// * or UTCTime

			count = tag->remaining_parse_length();

			c = tag->parse_read(); // 0x013
//			len = tag->parse_asn1_length(); // Length of String
			len = parse_asn1_length(tag);

			count = count - tag->remaining_parse_length();
			*taglength = len + count; // return value

        		if (action->get_byte(VDCHUNK_ASN1_ACTION_COMMENTS3) != 0) {
				// Comments level 3
				fprintf(VDCHUNK_ASN1_STREAM,
				"%s%02X : String (Length: 0x%02X) ", tabs.c_str(), c, 
				(unsigned int)(len));

				next = new vdchunk_base_cutout(tag->name + "CO2", tag, 
					tag->get_parse_counter(), tag->get_parse_counter()+len);
					// "delete" see below (aa)

				// If the string is not too long, print it
				if (next->entries < 80) {
					next->write_string_secure(VDCHUNK_ASN1_STREAM); 
				}
				else {
					// String too long
					fprintf(VDCHUNK_ASN1_STREAM,"[String omitted.]");
				};

				delete(next); // "new" command see above (aa)
				fprintf(VDCHUNK_ASN1_STREAM,"\n");
			}
			else {
				// No comments level 3
			};

			tag->parse_skip_bytes(len);
		}
		else {

		if (check_and_parse_asn1_integer(tag, depth, taglength, action)) {

			// * Integer
		}
		else {
		
		if (check_and_parse_asn1_octet_string(tag, depth, taglength, action)) {

			// * Octet String
		}
		else {
		
		if (check_and_parse_asn1_bit_string(tag, depth, taglength, action)) {

			// * Bit String
		}
		else {
		
		// * Unknown (low number tag)

		c = tag->parse_look_ahead();

		if ((c & 0x0c0) == 0x000) { s = "universal"; };
		if ((c & 0x0c0) == 0x040) { s = "application-specific"; };
		if ((c & 0x0c0) == 0x080) { s = "context-specific"; };
		if ((c & 0x0c0) == 0x0C0) { s = "private"; };

		parse_known_asn1_tag_as_sequence_or_hex(tag, "Unknown (" + s + ")", depth, 
			taglength, action);

		}; // Unknown
		}; // not Octet String
		}; // not Integer
		}; // not PrintableString
		}; // not ASN1 NULL tag
		}; // not EOC tag
		}; // not zero tag
		}; // not OID
	}; // not sequence
	}; // low number tag

	return 0;
}

string asn1_simple_spaces(int depth)

{
	string tabs;
	int loop;

        tabs = "";
        for (loop = 0; loop < depth; loop++) {
                tabs = tabs + " ";
                };

	return tabs;
}

/*

A Layman's Guide to a Subset of ASN.1, BER, and DER 
An RSA Laboratories Technical Note Burton S. Kaliski Jr.  Revised November 1, 1993
 
--

In each method, the BER encoding has three or four parts: 

Identifier octets. These identify the class and tag number of the ASN.1 
value, and indicate whether the method is primitive or constructed. 

Length octets. For the definite-length methods, these give the number of 
contents octets. For the constructed, indefinite-length method, these 
indicate that the length is indefinite. 

Contents octets. For the primitive, definite-length method, these give a 
concrete representation of the  value. For the constructed methods, 
these give the concatenation of the BER encodings of the 
components of the value. 

End-of-contents octets. For the constructed, indefinite-length method, these 
denote the end of the contents. For the other methods, these are 
absent. 

--

Type Tag number 
(decimal) Tag number(hexadecimal) 
INTEGER 2 02 
BIT STRING 3 03 
OCTET STRING 4 04 
NULL 5 05 
OBJECT IDENTIFIER 6 06 
SEQUENCE and SEQUENCE OF 16 10 
SET and SET OF 17 11 
PrintableString 19 13 
T61String 20 14 
IA5String 22 16 
UTCTime 23 17 
Table 1

*/

bool is_asn1_tag(vdchunk_base *tag)

//! Check whether the buffer content might be a valid ASN1 tag

{
#ifdef VDCHUNK_ASN1_DEBUG_MSG3
	fprintf(VDCHUNK_ASN1_DEBUG_STREAM, "is_asn1_tag(%s) Length: 0x%04X\n", tag->name.c_str(), (int)(tag->entries));
#endif

	int c,d;
	long int len;
	vdchunk_base_cutout *next;
	bool res;
	unsigned long int parse_temp;

	if (tag->entries < 2) return false;

	parse_temp = tag->get_parse_counter();
	tag->parse_reset();

	// Remove a leading zero, if necessary
        if (tag->get_byte(0) == 0) {
		c = tag->parse_read();
		assert(c == 0);
	};

	// XXYh This could use "parse_asn1_tag_number"....
	if (pp_asn1_high_tag_number_form(tag)) {
		// * High tag number form
		c = tag->parse_read();
		d = tag->parse_read();
		while ((d & 0x80) == 0x80) {
			d = tag->parse_read();
		}
	}
	else {
		// * Low tag number form
		c = tag->parse_read();
	};
//	len = tag->parse_asn1_length();
	len = parse_asn1_length(tag);

	if (len == tag->remaining_parse_length()) { 
		res = true; 
	}
	else {
		res = false;
	};

//	tag->parse_reset();
	tag->set_parse_counter(parse_temp);

	return res;
}

long int parse_asn1_length(vdchunk_base *tag)

//!  Parse BER-encoded length (parsing position)

//!
//!  Result: -1               --  if DER length is encoded incorrectly
//!         -2                --  if DER encoding is corrupt due to EOF
//          -9                --  BER encoding (indefinite length) is incorrect
//!
//!         resulting length  --   otherwise
//!
//! Documentation: 
//!          a) ISO 7816-4, 5.2.2.2, BER-TLV length fields
//!          b) ITU-T X.690
//!

{
	long int res;
	int c;
	unsigned long int parsec;
	res = tag->parse_der_encoded_length();
	// This returns -1 if incorrect, -2 if EOF, -8 if indefinite

	if (res == (-8)) {
		// Indefinite length
#ifndef NDEBUG
		parsec = tag->get_parse_counter();
#endif
		res = parse_asn1_length_of_eoc_terminated_sequence(tag);
//		tag->set_parse_counter(parsec);
		assert(tag->get_parse_counter() == parsec);
	}
 
	assert ((res >= 0) || (res == (-1)) || (res == (-2)) || (res == (-9)));
	return res;
}

long int parse_asn1_length_of_eoc_terminated_sequence(vdchunk_base *tag)

//! Returns the length of a sequence of ASN1 tags terminated by an EOC (00 00).
//! Please note: Does not change the parse counter.

//! \param tag Tag to be parsed 
//! \return Length or -9, if error

{
#ifdef VDCHUNK_ASN1_DEBUG_MSG4
	fprintf(VDCHUNK_ASN1_DEBUG_STREAM, 
		"parse_asn1_length_of_eoc_terminated_sequence(%s) Length: 0x%04X\n", 
		tag->name.c_str(), (int)(tag->entries));
#endif

	long int          res, iter;
	int               c, d;
	unsigned long int parsec;
	
	res = 0;
	c = tag->parse_look_ahead(0); 
	d = tag->parse_look_ahead(1); 

	if ((c == 0) && (d == 0)) {
		// EOC: Length is two bytes

#ifdef VDCHUNK_ASN1_DEBUG_MSG5
	fprintf(VDCHUNK_ASN1_DEBUG_STREAM, "  EOC, Return value: 2\n");
#endif

		return 2;
	} 
	else {
	if ((c < 0) || (d < 0)) {
		// EOF: Something is wrong

#ifdef VDCHUNK_ASN1_DEBUG_MSG5
	fprintf(VDCHUNK_ASN1_DEBUG_STREAM, "  EOF, Return value: -9\n");
#endif

		return -9;
	}
	else {

	// Parse counter must not be changed; keep the value
	parsec = tag->get_parse_counter();

	// Iterative procedure: This is the length of the first tag
	res = tlv_length2(tag);

	// Check consistency
	assert(parsec+res <= tag->entries);
	assert(tag->get_parse_counter() == parsec);

	if (res < 0) {
		// Whenever there is an error, the return value -9
		// will be given back the chain.

#ifdef VDCHUNK_ASN1_DEBUG_MSG5
	fprintf(VDCHUNK_ASN1_DEBUG_STREAM, "  Error 1, Return value: -9\n");
#endif

		return -9;
	} // if
	else {
	assert(res > 1);

	// Next tag
	tag->set_parse_counter(parsec+res);

	// Iterative procedure: This is the length of all subsequent tags
	iter = parse_asn1_length_of_eoc_terminated_sequence(tag);

	// Restore parse counter
	tag->set_parse_counter(parsec);
	
	if (iter < 0) {
		assert (iter == (-9));

#ifdef VDCHUNK_ASN1_DEBUG_MSG5
	fprintf(VDCHUNK_ASN1_DEBUG_STREAM, "  Error 2, Return value: -9\n");
#endif

		return -9;
	} 
	else {

#ifdef VDCHUNK_ASN1_DEBUG_MSG5
	fprintf(VDCHUNK_ASN1_DEBUG_STREAM, "  Return value: %d\n", (int)(res + iter));
#endif

	// Total value is length of first tag + subsequent tags
	res = res + iter;

	}; // else iter < 0
	}; // else res < 0
	}; // else c,d < 0
	}; // else EOC

	return res;
}

long int parse_asn1_tag_number(vdchunk_base *tag)

//! Parses the next tag number (parsing position)

//! \param tag Tag to be parsed 
//! \return Tag number, -1 if error

// Checked 2011-10-14

{
	int      c, d;
	long int res;
	// XXYh This could use "parse_asn1_tag_number"....
        if (pp_asn1_high_tag_number_form(tag)) {
                // * High tag number form
                c = tag->parse_read();
		res = (long int)(c);
                d = tag->parse_read();
		if (d < 0) {
			res = -1;
		}
		else {
			res = (res << 8) + (long int)(d);
		};
                while (((d & 0x80) == 0x80) && (d > 0)) {
                        d = tag->parse_read();
			if (d < 0) {
				res = -1;
			}
			else {
				res = (res << 8) + (long int)(d);
			};
                }
        }
        else {
        // * Low tag number form
        c = tag->parse_read();
	res = (long int)(c);
        };

	return res;
}

unsigned long int tlv_length(vdchunk_base *tag, VDCHUNK_ASN1_ACTION_TYPE_ABSTRACT *action)

//! Returns the length of an entire ANS1 tag (T + L + V) at parsing position. If the
//! content of the buffer is not a tag, it returns the number of entries.
//! Please note: Does not change the parse counter.

//! \param tag Tag to be parsed 
//! \param action Actions to be performed

{
#ifdef VDCHUNK_ASN1_DEBUG_MSG5
	fprintf(VDCHUNK_ASN1_DEBUG_STREAM, "tlv_length(%s, ...) Length: 0x%04X\n", 
		tag->name.c_str(), (int)(tag->entries));
#endif

	int           c,d;
	long int          v_len;
	unsigned long int res, ptemp, tl_len;

	ptemp = tag->get_parse_counter();

	c = tag->parse_look_ahead(0);
	d = tag->parse_look_ahead(1);
	if ((c == 0) && (d == 0)) {
		// EOC
		c = tag->parse_read();
		d = tag->parse_read();
		v_len = 0;
	}
	else {

	d = 0;
	if ((c == 0) && (action->get_byte(VDCHUNK_ASN1_ACTION_ACCEPT_ZERO_TAGS) != 0)) {
		// A zero tag might be considered as a one-byte tag.
		c = tag->parse_read();
		v_len = 0;
	} // if zero tag
	else {

	if (tag->entries < 2) return tag->entries;

	// XXYh This could use "parse_asn1_tag_number"....
	if (pp_asn1_high_tag_number_form(tag)) {
		// * High tag number form
		c = tag->parse_read();
		d = tag->parse_read();
		while (((d & 0x80) == 0x80) && (d > 0)) {
			d = tag->parse_read();
		}
	}
	else {
	// * Low tag number form
	c = tag->parse_read();
	};

	// EOF?
	if ((c < 0) || (d < 0)) return tag->entries;

	// Length of V element
//	v_len = tag->parse_asn1_length();
	v_len = parse_asn1_length(tag);

	}; // else zero tag
	}; // else EOC

	// Length of T and L element
	assert(((long int)tag->get_parse_counter()-(long int)ptemp) > 0);
	tl_len = tag->get_parse_counter()-ptemp;

	// Consistenty
	// Last byte of tag correctly formed
	assert((d & 0x080) == 0);

	tag->set_parse_counter(ptemp);

	res = tl_len + v_len;
	if (res > tag->entries) return tag->entries;

	return res;
}

long int tlv_length2(vdchunk_base *tag)

//! Alternative version:
//! Returns the length of an entire ANS1 tag (T + L + V) at parsing position. If the
//! content of the buffer is not a tag, it returns -1.
//! Please note: Does not change the parse counter.

//! \param tag Tag to be parsed 
//! \param action Actions to be performed

{
#ifdef VDCHUNK_ASN1_DEBUG_MSG5
	fprintf(VDCHUNK_ASN1_DEBUG_STREAM, "tlv_length2(%s, ...) Length: 0x%04X\n", 
		tag->name.c_str(), (int)(tag->entries));
#endif
	int           c,d;
	long int          v_len;
	long int          res, sum;
	unsigned long int ptemp, tl_len;

	res = 0;
	ptemp = tag->get_parse_counter();

	c = tag->parse_look_ahead(0);
	d = tag->parse_look_ahead(1);

	if ((c == 0) && (d == 0)) {
		// EOC
		c = tag->parse_read();
		d = tag->parse_read();
		v_len = 0;
	}
	else {
		d = 0;

		if (c == 0) {
			// A zero tag might be considered as a one-byte tag.
			c = tag->parse_read();
			v_len = 0;
		} // if zero tag
		else {
			if (tag->entries < 2) {
				res = -1;
			}
			else {
	// XXYh This could use "parse_asn1_tag_number"....
				if (pp_asn1_high_tag_number_form(tag)) {
					// * High tag number form
					c = tag->parse_read();
					d = tag->parse_read();
					while (((d & 0x80) == 0x80) && (d > 0)) {
						d = tag->parse_read();
					}
				}
				else {
					// * Low tag number form
					c = tag->parse_read();
				};

				// EOF?
				if ((c < 0) || (d < 0)) {
					res = -1;
				}
				else {
					// Length of V element
					v_len = parse_asn1_length(tag);
				}; // else EOF
			}; // else tag<2
		}; // else zero tag
	}; // else EOC

	assert((res == 0) || (res == (-1)));

	if (v_len < 0) { res = -1; };

	if (res == 0) {
		// Length of T and L element
		assert(((long int)tag->get_parse_counter()-(long int)ptemp) > 0);
		tl_len = tag->get_parse_counter()-ptemp;

		// Consistenty
		// If zero tag, "tl_len" equals 1
		assert(!((c ==0) && (tl_len != 1)));
		// Last byte of tag correctly formed
		assert((d & 0x080) == 0);

		sum = tl_len + v_len;
		if (sum > tag->entries) {
			res = -1;
		}
		else {
			res = sum;
		};
	};

	tag->set_parse_counter(ptemp);
	return res;
}

bool is_sequence_of_asn1_tags_not_strict(vdchunk_base *tag, VDCHUNK_ASN1_ACTION_TYPE_ABSTRACT *action)

//! Checks whether the buffer contains a sequence of valid ASN1 tags. 
//! Not strict: Single zero tags (00) and single tags in low-tag form with zero 
//! length (xx 00) do
//! XXX Does that make sense?
//! not count as a valid sequence here.
//! Please note: Resets parse counter. XXYa

//! \param tag Tag to be parsed 
//! \param action Actions to be performed

{
#ifdef VDCHUNK_ASN1_DEBUG_MSG4
	fprintf(VDCHUNK_ASN1_DEBUG_STREAM, "is_sequence_of_asn1_tags_not_strict(%s) Length: 0x%04X, Action: %s\n", tag->name.c_str(), (int)(tag->entries), action->name.c_str());
#endif

	tag->parse_reset();

	if (tag->entries < 2) {
		return false;
		}
	else {
	if (tag->entries == 2) {
		if (tag->parse_look_ahead(1) == 0) {
			return false;
		};
	}
	else {
	return is_sequence_of_asn1_tags(tag, action); // XXX Does that make sense? Wouldn't it just apply top the first item?
	};
	};
}

bool is_sequence_of_asn1_tags(vdchunk_base *tag, VDCHUNK_ASN1_ACTION_TYPE_ABSTRACT *action)

//! Checks whether the buffer contains a sequence of valid ASN1 tags. 
//! Please note: Resets parse counter. XXYa

//! \param tag Tag to be parsed 
//! \param action Actions to be performed

{
#ifdef VDCHUNK_ASN1_DEBUG_MSG4
	fprintf(VDCHUNK_ASN1_DEBUG_STREAM, "is_sequence_of_asn1_tags(%s, ...) Length: 0x%04X\n", 
		tag->name.c_str(), (int)(tag->entries));
#ifdef VDCHUNK_ASN1_DEBUG_MSG6
	if (action != NIL) {
		fprintf(VDCHUNK_ASN1_DEBUG_STREAM, " Action: %s\n", action->name.c_str()); 
		if (action->get_byte(VDCHUNK_ASN1_ACTION_ACCEPT_ZERO_TAGS) != 0) {
			fprintf(VDCHUNK_ASN1_DEBUG_STREAM, " Accepts zero tags: yes\n"); 
		} else {
			fprintf(VDCHUNK_ASN1_DEBUG_STREAM, " Accepts zero tags: no\n");  };
	} else {
		fprintf(VDCHUNK_ASN1_DEBUG_STREAM, " Action: NIL\n", action->name.c_str()); };

//	cout << "Q "; tag->write_hex(); cout << endl;
#endif
#endif

	int c,d;
	long int len;
	vdchunk_base_cutout *next;
	bool res;

	tag->parse_reset(); // 

	c = tag->parse_look_ahead();
	if (c == 0) {
// cout << "U " << len << endl;
		// If there is a zero byte ...
		if (action->get_byte(VDCHUNK_ASN1_ACTION_ACCEPT_ZERO_TAGS) != 0) {
			// .... I might ignore it ....
			c = tag->parse_read();
			len = 0;
		}
		else {
			// .... or not
			return false;
		};
	} // if zero tag
	else {
		if (tag->entries < 2) return false;

	// XXYh This could use "parse_asn1_tag_number"....
		if (pp_asn1_high_tag_number_form(tag)) {
			// * High tag number form
			c = tag->parse_read();
			d = tag->parse_read();
			while (((d & 0x80) == 0x80) && (d > 0)) {
				d = tag->parse_read();
			}
		}
		else {
			// * Low tag number form
			c = tag->parse_read();
			d = 0;
		};

		if ((c < 0) || (d < 0)) return false;

//		len = tag->parse_asn1_length();
		len = parse_asn1_length(tag);

// cout << "W " << len << endl;
	}; // else zero tag

	if (len == tag->remaining_parse_length()) { 
		res = true; 
	}
	else {
		if ((len >= 0) && (len < tag->remaining_parse_length())) {

// cout << "Z " << tag->remaining_parse_length() << endl;
			// Next iteration
			tag->parse_skip_bytes(len);		
			next = new vdchunk_base_cutout(tag->name + "CO7", tag, 
				tag->get_parse_counter(), tag->entries);
			if (is_sequence_of_asn1_tags(next, action)) {
				res = true; 
			} else {
				res = false;
			};
			delete (next);
		}
		else {
			res = false; 
		};
	};

	tag->parse_reset();
	return res;
}

bool is_asn1_string_tag(char c)

//! Returns true if input is the identifier of an ASN1 string.

{
	if (((c & 0x09f) == 0x013) // PrintableString
	   || ((c & 0x09f) == 0x00c) // UTF8 String
           || ((c & 0x09f) == 0x016)) // IA5String
           { return true; } else {return false; }
}

bool is_asn1_string(vdchunk_base *tag)

//! Returns true if next tag in buffer (parsing position) begins with an ASN1 
//! string.

{
	int c;
	c = tag->parse_look_ahead();
	return is_asn1_string_tag(c);
}

bool is_asn1_UTCTime_tag(char c)

//! Returns true if input is the identifier of an ASN1 "UTCTime".

{
	if ((c & 0x09f) == 0x17) { return true; } else {return false; }
}

bool is_asn1_UTCTime(vdchunk_base *tag)

//! Returns true if next tag in buffer (parsing position) begins with an ASN1 
//! "UTCTime".

{
	int c;
	c = tag->parse_look_ahead();
	if (c < 0) {
		return false;
	}
	else {
		return is_asn1_UTCTime_tag((char)c);
	};
}

bool is_asn1_set_tag(char c)

//! Returns true if input is the identifier of an ASN1 "SET".

{
	if ((c & 0x09f) == 0x11) { return true; } else {return false; }
}


bool is_asn1_set(vdchunk_base *tag)

//! Returns true if next tag in buffer (parsing position) begins with an ASN1 "SET".

{
	vdcbyte c;
	c = tag->parse_look_ahead();
	return is_asn1_set_tag(c);
}

bool is_asn1_sequence_tag(char c)

//! Returns true if input is the identifier of an ASN1 "SEQUENCE".

{
	if ((c & 0x09f) == 0x10) { return true; } else {return false; }
	// Why 1F and not 3F ?
}


bool is_asn1_sequence(vdchunk_base *tag)

//! Returns true if next tag in buffer (parsing position) begins with an ASN1 "SEQUENCE".

{
	vdcbyte c;
	c = tag->parse_look_ahead();
	return is_asn1_sequence_tag(c);
}

bool is_asn1_oid_tag(char c)

//! Returns true if input is the identifier of an OID

{
	if ((c & 0x09f) == 0x06) { return true; } else {return false; }
	// Why 1F and not 3F ?
}


bool is_asn1_oid(vdchunk_base *tag)

//! Returns true if next tag in buffer (parsing position) begins with an OID

{
	vdcbyte c;
	c = tag->parse_look_ahead();
	return is_asn1_oid_tag(c);
}

bool check_and_parse_asn1_oid(vdchunk_base *tag, int depth, long int *taglength, 
     VDCHUNK_ASN1_ACTION_TYPE_ABSTRACT *action)

//! Checks whether first tag in buffer (parsing position) begins with an ASN1 
//! object identifier (OID) and parses it, if so. All subsequent tags in 
//! the buffer will be ignored.

{
	if (is_asn1_oid(tag)) { 
		parse_asn1_oid(tag, depth, taglength, action); 
		return true;}
	else {
		return false;
	};
}

void parse_asn1_oid(vdchunk_base *tag, int depth, long int *taglength, 
     VDCHUNK_ASN1_ACTION_TYPE_ABSTRACT *action)

//! Parses the buffer (parsing position) under the assumption 
//! that it is an ASN1 object identifier (OID).
//! All subsequent tags in buffer will be ignored.

{
	vdcbyte c;
	string tagname, oidstring, oidname;
	tagname = "OID";
	unsigned long int count;

	// * Object Identifier (OID)

	count = tag->remaining_parse_length();

	c = tag->parse_look_ahead();
	oidstring = tag->parse_object_identifier();

	count = count - tag->remaining_parse_length();
	*taglength = count; // return value

	oidname = dumpasn1(oidstring);

       	if (action->get_byte(VDCHUNK_ASN1_ACTION_OID_IN_STRING) != 0) {
		vdc_global_string1 = oidname;
	};

       	if (action->get_byte(VDCHUNK_ASN1_ACTION_COMMENTS3) != 0) {

		string tabs;
		tabs = asn1_simple_spaces(depth);
	
		// Comments level 3
		fprintf(VDCHUNK_ASN1_STREAM,
		"%s%02X : Object Identifier (OID): %s ", 
		tabs.c_str(), c, oidstring.c_str());
		if (oidname != "Unknown") {
			fprintf(VDCHUNK_ASN1_STREAM,"%s", oidname.c_str());
		}	
		fprintf(VDCHUNK_ASN1_STREAM,"\n");
	};

	return;
}

bool is_asn1_null_tag(char c)

//! Returns true if input is an ASN1 NULL tag.

//! Please note: For this purpose, we distinguish between "ASN1 NULL tags" 
//! and zero tags which are non-ASN1 compliant and who might or might 
//! not be accepted, depending on VDCHUNK_ASN1_ACTION_ACCEPT_ZERO_TAGS.
//! (ASN1 NULL tag: "05 00"; zero tag: "00")

{
	if ((c & 0x09f) == 0x05) { return true; } else {return false; }
}


bool is_asn1_null(vdchunk_base *tag)

//! Returns true if next tag in buffer (parsing position) begins with an ASN1 NULL

//! Please note: For this purpose, we distinguish between "ASN1 NULL tags", 
//! EOC tags, and zero tags which are non-ASN1 compliant and who might or might 
//! not be accepted, depending on VDCHUNK_ASN1_ACTION_ACCEPT_ZERO_TAGS.
//! (ASN1 NULL tag: "05 00"; zero tag: "00", EOC tag: "00 00")

{
	int c;
	c = tag->parse_look_ahead();
	return is_asn1_null_tag(c);
}

bool is_eoc_tag(vdchunk_base *tag)

//! Returns true if input is a eoc tag.

//! Please note: For this purpose, we distinguish between "ASN1 NULL tags", 
//! EOC tags, and zero tags which are non-ASN1 compliant and who might or might 
//! not be accepted, depending on VDCHUNK_ASN1_ACTION_ACCEPT_ZERO_TAGS.
//! (ASN1 NULL tag: "05 00"; zero tag: "00", EOC tag: "00 00")

{
	int c, d;
	c = tag->parse_look_ahead(0);
	d = tag->parse_look_ahead(1);
	return ((c == 0) && (d == 0));
}

bool is_zero_tag(vdchunk_base *tag)

//! Returns true if input is a zero tag.

//! Please note: For this purpose, we distinguish between "ASN1 NULL tags", 
//! EOC tags, and zero tags which are non-ASN1 compliant and who might or might 
//! not be accepted, depending on VDCHUNK_ASN1_ACTION_ACCEPT_ZERO_TAGS.
//! (ASN1 NULL tag: "05 00"; zero tag: "00", EOC tag: "00 00")

{
	int c;
	c = tag->parse_look_ahead();
	return (c == 0);
}

// Return value should be integer (for tags with more than two bytes)
// XXYa: rename in "is_asn1_high_tag_number_form"

bool is_asn1_high_tag_number_form(vdchunk_base *tag)

//! Returns true if next tag in buffer (parsing position) is in high tag number form.

//! Identifier octets. There are two forms: low tag number (for tag numbers 
//! between 0 and 30) and high tag number (for tag numbers 31 and greater). 
//! 
//! Low-tag-number form. One octet. Bits 8 and 7 specify the class (see Table 2), 
//! bit 6 has value "0," indicating that the encoding is primitive, and 
//! bits 5-1 give the tag number.
//! 
//! High-tag-number form. Two or more octets. First octet is as in low-tag- 
//! number form, except that bits 5-1 all have value "1." Second and 
//! following octets give the tag number, base 128, most significant 
//! digit first, with as few digits as possible, and with the bit 8 of each 
//! octet except the last set to "1." 
//! 
//! (Source: A Layman's Guide to a Subset of ASN.1, BER, and DER 
//! An RSA Laboratories Technical Note Burton S. Kaliski Jr.  Revised November 1, 1993)
//!

{
	int c;
	c = tag->parse_look_ahead();
	if ((c & 0x01f) == 0x01f) { return true; } else { return false; };
}

// ******** Cleaning up .....

// This will should only be called by "parse_as_sequence_or_hex...."
// Should be renamed: "check_and_parse_sequence_or_hex" or "...sub" or similar....
bool check_and_parse_sequence_of_asn1_tags(vdchunk_base *tag, int depth, bool strict, 
	VDCHUNK_ASN1_ACTION_TYPE_ABSTRACT *action)

//! Parses the buffer (zero position) under the assumption that 
//! it might contain a sequence of tags or otherwise has to be 
//! displayed as hex code.
//! In principle, this function parses the "V" part of a tag (TLV)
//! that contains a sequence of nested tags.
//! Please note: Resets parse counter (XXYa)

//! \param tag Buffer to be parsed 
//! \param depth Number of spaces for indention
//! \param action Actions to be performed
//! \return TRUE, if sequence/set; FALSE, otherwise

{

#ifdef VDCHUNK_ASN1_DEBUG_MSG3
	fprintf(VDCHUNK_ASN1_DEBUG_STREAM, 
	  "check_and_parse_sequence_of_asn1_tags(%s, ...) [c] Length: 0x%04X, Depth: %d\n", 
	  tag->name.c_str(), (int)(tag->entries), depth);
#endif

	long int            slen;
	bool                finished, result, isseq;
	unsigned long int   tlv_len;
	vdchunk_base_cutout *next2;

	string tabs;
	tabs = asn1_simple_spaces(depth);

	tag->parse_reset();

	// No action if tag is empty
	if (tag->entries == 0) return false; 

	isseq = is_sequence_of_asn1_tags(tag, action); 
/* Temporarily not available

	// Check if the tag is a sequence of valid tags....
	if (strict) {
		isseq = is_sequence_of_asn1_tags(tag, action); }
	else {
		isseq = is_sequence_of_asn1_tags_not_strict(tag, action); }
*/

	tag->parse_reset();

	if (isseq) {
		
		// If yes ....
		result = true;

       	if (action->get_byte(VDCHUNK_ASN1_ACTION_COMMENTS6) != 0) {
		fprintf(VDCHUNK_ASN1_STREAM,
		"%s     Check whether content might be a sequence of embedded ASN1 tags .... yes\n", 
		tabs.c_str());
	};

		tag->parse_reset();
		finished = false;
		do {
			tlv_len = tlv_length(tag, action);

			next2 = new vdchunk_base_cutout(tag->name + "OS3", tag, 
				tag->get_parse_counter(), tag->get_parse_counter()+tlv_len);

			parse_asn1(next2, depth, &slen, action);

			// This part of "tag" has now been parsed; correct the parse 
			// counter accordingly

			delete(next2);

			tag->parse_skip_bytes(tlv_len);
			if (tag->parse_counter() >= tag->entries) finished = true;

		} while (!finished);
	}
	else {
		// Buffer does not contain a sequence of ASN1 tags.
		result = false;

	       	if (action->get_byte(VDCHUNK_ASN1_ACTION_COMMENTS6) != 0) {
			fprintf(VDCHUNK_ASN1_STREAM,
			"%s     Check whether content might be a sequence of embedded ASN1 tags .... no\n", 
			tabs.c_str());
		};

		// Just print the buffer, if required.
       		if (action->get_byte(VDCHUNK_ASN1_ACTION_COMMENTS4) != 0) {
	        	if (action->get_byte(VDCHUNK_ASN1_ACTION_DISPLAY_LEAD0) != 0) {
				// DISPLAY LEAD0 defines whether leading zero bytes 
				// should be displayed separately
				if (tag->parse_look_ahead(0) == 0) {
					tag->parse_read();
		 			fprintf(VDCHUNK_ASN1_STREAM,"%s    00\n", tabs.c_str());
				}; // if leading zero byte
			};
			assert(tag->get_parse_counter() <= tag->entries);
			if (tag->get_parse_counter() < tag->entries) {
				next2 = new vdchunk_base_cutout(tag->name + "OS3", tag, 
					tag->get_parse_counter(), tag->entries);

				assert(depth > 0);
				vdchunk_asn1_write_hex(next2, depth-1);
//			next2->write_hex(VDCHUNK_ASN1_STREAM, tabs + "    ", 
//			VDCHUNK_ASN1_CHARPERLINE, 32, VDCHUNK_ASN1_CHARTOTAL); 
//			fprintf(VDCHUNK_ASN1_STREAM,"\n");

				delete(next2);
			};
		};
	}; // else

	return result;
}

// Do we need this?
// XXX
void parse_known_asn1_tag(vdchunk_base *tag, string tagname, int depth, long int *taglength, VDCHUNK_ASN1_ACTION_TYPE_ABSTRACT *action)

//! Parses the first tag (name given). 
//! Content will be displayed depending on the "action".
//! All subsequent tags in buffer will be ignored.

{
	long int  len, slen, count;
	char      c, d;
	bool      finished, ht;
	string    tabs, indef;

	tabs = asn1_simple_spaces(depth);
	indef = "";

	tag->parse_reset();
	count = tag->remaining_parse_length();
	assert(count == tag->entries);

	// High or low tag number form?
	// XXYh This could use "parse_asn1_tag_number"....
	ht = pp_asn1_high_tag_number_form(tag);
	if (ht) {
		c = tag->parse_read();
		d = tag->parse_read();
	}
	else {
		c = tag->parse_read();
	};
	
	if (tag->parse_look_ahead() == 0x080) { indef = " (indefinite) "; };
//	len = tag->parse_asn1_length();
	len = parse_asn1_length(tag);

	count = count - tag->remaining_parse_length();
	*taglength = len + count; // return value

       	if (action->get_byte(VDCHUNK_ASN1_ACTION_COMMENTS4) != 0) {
		if (ht) {
	 	fprintf(VDCHUNK_ASN1_STREAM,"%s%02X %02X : %s (Length%s: 0x%02X)\n", tabs.c_str(), 
			c, d, tagname.c_str(), indef.c_str(), (unsigned int)(len));
		}
		else {
 		fprintf(VDCHUNK_ASN1_STREAM,"%s%02X : %s (Length%s: 0x%02X)\n", tabs.c_str(), c, 
			tagname.c_str(), indef.c_str(), (unsigned int)len);
		};
	};

	tag->parse_skip_bytes(len); // Set parse counter behind the end of the tag
}

void parse_known_asn1_tag_as_hex(vdchunk_base *tag, string tagname, int depth, 
                                 long int *taglength, VDCHUNK_ASN1_ACTION_TYPE_ABSTRACT *action)

//! Parses the next tag (parsing position, name given) and displays it as hex code.

//! \param tag Tag to be parsed 
//! \param tagname Name of the tag
//! \param depth Number of spaces for indention
//! \param taglength Return value: tag length

{
#ifdef VDCHUNK_ASN1_DEBUG_MSG3
	fprintf(VDCHUNK_ASN1_DEBUG_STREAM, "parse_known_asn1_tag_as_hex(%s) Length: 0x%04X, Depth: %d Action: %s\n", tag->name.c_str(), (int)(tag->entries), depth, action->name.c_str());
#endif

	long int       len, slen, count;
	char           c, d;
	bool           finished, ht;
	vdchunk_base_cutout *zahl;

	string tabs;
	tabs = asn1_simple_spaces(depth);

	count = tag->remaining_parse_length();

	// High or low tag number form?
	// XXYh This could use "parse_asn1_tag_number"....
	ht = pp_asn1_high_tag_number_form(tag);
	if (ht) {
		c = tag->parse_read();
		d = tag->parse_read();
	}
	else {
		c = tag->parse_read();
	};
	
//	len = tag->parse_asn1_length();
	len = parse_asn1_length(tag);

	count = count - tag->remaining_parse_length();
	*taglength = len + count; // return value

	zahl = new vdchunk_base_cutout(tag->name + "OS2", tag, 
			tag->get_parse_counter(), tag->get_parse_counter()+len);

       	if (action->get_byte(VDCHUNK_ASN1_ACTION_COMMENTS4) != 0) {
		// Print hex block --> Yes

		if (ht) {
 			fprintf(VDCHUNK_ASN1_STREAM,"%s%02X%02X : %s (Length: 0x%02X)\n", 
			tabs.c_str(), c, d, tagname.c_str(), (unsigned int)len);
		}
		else {
		 	fprintf(VDCHUNK_ASN1_STREAM,"%s%02X : %s (Length: 0x%02X)\n", 
			tabs.c_str(), c, tagname.c_str(), (unsigned int)len);
		};
        }; // if COMMENTS4

        if (action->get_byte(VDCHUNK_ASN1_ACTION_DISPLAY_LEAD0) != 0) {
		// DISPLAY LEAD0 defines whether leading zero bytes should be displayed separately
		// ** 
		// Remove a leading zero byte if there is one
		if (zahl->get_byte(0) == 0) {

			// "get_byte(0)" is equivalent to look-ahead....
			assert(zahl->parse_look_ahead() == 0);

			zahl->parse_read();
			zahl->cut_parsed_bytes();

       			if (action->get_byte(VDCHUNK_ASN1_ACTION_COMMENTS4) != 0) {
			// Print Bit String? --> Yes
		 	fprintf(VDCHUNK_ASN1_STREAM,"%s     00\n", tabs.c_str());
			}; // if OUTPUT1

		}; // if leading zero byte
		// **
	};

	// Print the buffer as hex code (if designated)
	if (action->get_byte(VDCHUNK_ASN1_ACTION_COMMENTS4) != 0) {
		vdchunk_asn1_write_hex(zahl, depth);
	}; // if COMMENTS4

	// Cleaning up ....
	delete(zahl);
	tag->parse_skip_bytes(len); // Set parse counter behind the end of the tag
}

int parse_known_asn1_tag_as_sequence_or_hex(vdchunk_base *tag, string tagname, int depth, long int *taglength, VDCHUNK_ASN1_ACTION_TYPE_ABSTRACT *action)

//! Parses the next tag (parsing position) under the assumption that 
//! it might contain a sequence of tags or otherwise has to be 
//! displayed as hex code.

//! \param tag Tag to be parsed 
//! \param tagname Name of the tag (will be displayed)
//! \param depth Number of spaces for indention
//! \param taglength Return value: tag length (T + L + V) XXYd
//! \param action Actions to be performed
//! \return -1 : Error; 0: Ok, sequence; 1: Ok, Hex

{
#ifdef VDCHUNK_ASN1_DEBUG_MSG3
        fprintf(VDCHUNK_ASN1_DEBUG_STREAM, 
		"parse_known_asn1_tag_as_sequence_or_hex(%s,%s,%d,...) Length: 0x%04X\n", 
		tag->name.c_str(), tagname.c_str(), depth, (int)(tag->entries));
#endif

	long int       len, slen, count;
	int            c, d, res;
	bool           finished, ht, chk;
	vdchunk_base_cutout *zahl, *next2;
	string              tabs, indef;

        indef = "";
	tabs = asn1_simple_spaces(depth);

	count = tag->remaining_parse_length();

	res = -1;
//	c = 0; 
	d = 0;

	// High or low tag number form?
	// XXYh This could use "parse_asn1_tag_number"....
	ht = pp_asn1_high_tag_number_form(tag);

	if (ht) {
		c = tag->parse_read();
		if (c < 0) return -1; // EOF
		d = tag->parse_read();
		if (d < 0) return -1; // EOF

		assert((d && 0x080) == 0x080); // Tags of more than two bytes not (yet) 
		                               // supported (XXYc)
	}
	else {
		c = tag->parse_read();
		if (c < 0) return -1; // EOF
	};

	if (tag->parse_look_ahead() == 0x080) { indef = " (indefinite) "; };
//	len = tag->parse_asn1_length();
	len = parse_asn1_length(tag);

	count = count - tag->remaining_parse_length();
	*taglength = len + count; // return value

	// Print ?
       	if (action->get_byte(VDCHUNK_ASN1_ACTION_COMMENTS3) != 0) {
		// Yes
		if (ht) {
	 	fprintf(VDCHUNK_ASN1_STREAM,"%s%02X %02X : %s (Length%s: 0x%02X)\n", tabs.c_str(), 
			c, d, tagname.c_str(), indef.c_str(), (unsigned int)(len));
		}
		else {
 		fprintf(VDCHUNK_ASN1_STREAM,"%s%02X : %s (Length%s: 0x%02X)\n", tabs.c_str(), c, 
			tagname.c_str(), indef.c_str(), (unsigned int)len);
		};
	}; // if COMMENTS3

	if (len == 0) {
		// Tag with zero length (e.g. "05 00" or "7F 4E 00")
		res = 0;
		}
	else {
	
	// Content (V) of the tag:
	zahl = new vdchunk_base_cutout(tag->name + "V", tag, 
			tag->get_parse_counter(), tag->get_parse_counter()+len);

	// Checking whether the content is a sequence of ASN1 tags
	chk = check_and_parse_sequence_of_asn1_tags(zahl, depth+1, false, action);

	if (chk) { 
		// Sequence/Set
		res = 0; 
	} 
	else { 
		// Hex bytes
		res = 1; 
	};

	// Cleaning up ....
	delete(zahl);

	tag->parse_skip_bytes(len); // Set parse counter after the end of the tag

	}; // else len = 0

	// There shouldn't be anything else in the tag
	// Strict: Assertion
//	assert(tag->get_parse_counter() == tag->entries);
	// Robust: If not, then return -1;
	if (!(tag->get_parse_counter() == tag->entries)) { return -2; }

 	return res;
}

// *** 02: Integer

bool is_asn1_integer_tag(char c)

//! Returns true if input is the identifier of an ASN1 Integer

{
	if ((c & 0x09f) == 0x02) { return true; } else {return false; }
}


bool is_asn1_integer(vdchunk_base *tag)

//! Checks whether tag is an ASN1 integer tag (parsing position)
//!
//! \param tag Tag to be parsed 
//! \return TRUE, if tag is an ASN1 integer tag, FALSE otherwise

{
	int c;
	c = tag->parse_look_ahead();
	assert(c < 256);

	if (c < 0) {
		// EOF
		return false;
	}
	else {
		return is_asn1_integer_tag((char)c);
	};
}

bool check_and_parse_asn1_integer(vdchunk_base *tag, int depth, long int *taglength, 
     VDCHUNK_ASN1_ACTION_TYPE_ABSTRACT *action)

//! Checks whether tag is an ASN1 integer tag (parsing position) and parses it, if so.
//!
//! \param tag Tag to be parsed 
//! \param depth Parse depth
//! \param taglength Returns pointer to tag length
//! \param action Actions to be performed
//! \return TRUE, if tag is an ASN1 integer tag, FALSE otherwise

{
	if (is_asn1_integer(tag)) { 
		parse_asn1_integer(tag, depth, taglength, action); 
		return true;}
	else {
		return false;
	};
}

void parse_asn1_integer(vdchunk_base *tag, int depth, long int *taglength, 
     VDCHUNK_ASN1_ACTION_TYPE_ABSTRACT *action)

//! Parses the next tag (parsing position) assuming that it is an ASN1
//! integer. All subsequent tags in buffer will be ignored.

{
#ifdef VDCHUNK_ASN1_DEBUG_MSG3
        fprintf(VDCHUNK_ASN1_DEBUG_STREAM, "parse_asn1_integer(%s,%d,...) Length: 0x%04X\n", 
		tag->name.c_str(), depth, (int)(tag->entries));
#endif

	// Output as hex block (if designated)
	parse_known_asn1_tag_as_hex(tag, "Integer", depth, taglength, action);
}

// *** 03: Bit String

bool is_asn1_bit_string_tag(char c)

//! Returns true if input is the identifier of an ASN1 octet string

{
	if ((c & 0x09f) == 0x03) { return true; } else {return false; }
}


bool is_asn1_bit_string(vdchunk_base *tag)

//! Checks whether tag is an ASN1 integer tag (parsing position) and parses it, if so.
//!
//! \param tag Tag to be parsed 
//! \param depth Parse depth
//! \param taglength Returns pointer to tag length
//! \param action Actions to be performed
//! \return TRUE, if tag is an ASN1 bit string, FALSE otherwise

{
	vdcbyte c;
	c = tag->parse_look_ahead();
	assert(c < 256);

	if (c < 0) {
		// EOF
		return false;
	}
	else {
		return is_asn1_bit_string_tag((char)c);
	};
}

bool check_and_parse_asn1_bit_string(vdchunk_base *tag, int depth, long int *taglength, 
     VDCHUNK_ASN1_ACTION_TYPE_ABSTRACT *action)

//! Checks whether first tag in buffer (parsing position) begins with an ASN1 integer
//! and parses it, if so. All subsequent tags in buffer will be ignored.

{
	if (is_asn1_bit_string(tag)) { 
		parse_asn1_bit_string(tag, depth, taglength, action); 
		return true;}
	else {
		return false;
	};
}

void parse_asn1_bit_string(vdchunk_base *tag, int depth, long int *taglength, 
     VDCHUNK_ASN1_ACTION_TYPE_ABSTRACT *action)

//! Parses the first tag of the buffer und the assumption that it is 
//! an ASN1 Bit String. All subsequent tags in buffer will be ignored.

//! \param tag Tag to be parsed 
//! \param depth Parse depth (for output)
//! \param taglength Returns the tag length (length of T+L+V)

{
#ifdef VDCHUNK_ASN1_DEBUG_MSG3
        fprintf(VDCHUNK_ASN1_DEBUG_STREAM, "parse_asn1_bit_string(%s,%d,...) Length: 0x%04X\n", 
		tag->name.c_str(), depth, (int)(tag->entries));
#ifdef VDCHUNK_ASN1_DEBUG_MSG6
	if (action != NIL) {
		fprintf(VDCHUNK_ASN1_DEBUG_STREAM, " Action: %s\n", action->name.c_str()); 
		if (action->get_byte(VDCHUNK_ASN1_ACTION_ACCEPT_ZERO_TAGS) != 0) {
			fprintf(VDCHUNK_ASN1_DEBUG_STREAM, " Accepts zero tags: yes\n"); 
		} else {
			fprintf(VDCHUNK_ASN1_DEBUG_STREAM, " Accepts zero tags: no\n");  };
	} else {
		fprintf(VDCHUNK_ASN1_DEBUG_STREAM, " Action: NIL\n", action->name.c_str()); };
#endif
#endif

	// Output as hex block (if designated)
	parse_known_asn1_tag_as_sequence_or_hex(tag, "Bit String", depth, taglength, action);
}

// *** 04: Octet String

bool is_asn1_octet_string_tag(char c)

//! Returns true if input is the identifier of an ASN1 octet string

{
	if ((c & 0x09f) == 0x04) { return true; } else {return false; }
}


bool is_asn1_octet_string(vdchunk_base *tag)

//! Returns true if next tag in buffer (parsing position) begins with an ASN1 octet string

{
	vdcbyte c;
	c = tag->parse_look_ahead();
	return is_asn1_octet_string_tag(c);
}

bool check_and_parse_asn1_octet_string(vdchunk_base *tag, int depth, long int *taglength, 
     VDCHUNK_ASN1_ACTION_TYPE_ABSTRACT *action)

//! Checks whether first tag in buffer (parsing position) begins with an ASN1 octet string
//! and parses it, if so. All subsequent tags in buffer will be ignored.

{
	if (is_asn1_octet_string(tag)) { 
		parse_asn1_octet_string(tag, depth, taglength, action); 
		return true;}
	else {
		return false;
	};
}

void parse_asn1_octet_string(vdchunk_base *tag, int depth, long int *taglength, 
     VDCHUNK_ASN1_ACTION_TYPE_ABSTRACT *action)

//! Parses the first tag of the buffer under the assumption 
//! that it is an ASN1 Octet String.
//! All subsequent tags in buffer will be ignored.

{
	string tagname;
	tagname = "Octet String";
	parse_known_asn1_tag_as_sequence_or_hex(tag, tagname, depth, taglength, action);
	return;
}

// *** 03/04: Bit String or Octet String

bool is_asn1_boo_string_tag(char c)

//! Returns true if input is the identifier of an ASN1 Octet String
//! OR an ASN1 Bit String

{
	if (((c & 0x09f) == 0x04) || ((c & 0x09f) == 0x03)) { return true; } else {return false; }
}


bool is_asn1_boo_string(vdchunk_base *tag)

//! Returns true if next tag in buffer (parsing position) begins with an ASN1 Octet String
//! OR an ASN1 Bit String

{
	vdcbyte c;
	c = tag->parse_look_ahead();
	return is_asn1_boo_string_tag(c);
}

bool check_and_parse_asn1_boo_string(vdchunk_base *tag, int depth, long int *taglength, 
     VDCHUNK_ASN1_ACTION_TYPE_ABSTRACT *action)

//! Checks whether first tag in buffer (parsing position) begins with an ASN1 Octet String
//! OR an ASN1 Bit String and parses it, if so. All subsequent tags in buffer will be ignored.

{
	if (is_asn1_octet_string(tag)) { 
		parse_asn1_boo_string(tag, depth, taglength, action); 
		return true;}
	else {
		return false;
	};
}

void parse_asn1_boo_string(vdchunk_base *tag, int depth, long int *taglength, 
     VDCHUNK_ASN1_ACTION_TYPE_ABSTRACT *action)

//! Parses the first tag of the buffer under the assumption 
//! that it is an ASN1 Octet String OR an ASN1 Bit String
//! All subsequent tags in buffer will be ignored.

{
	string tagname;
	tagname = "Bit/Octet String";
	parse_known_asn1_tag_as_sequence_or_hex(tag, tagname, depth, taglength, action);
	return;
}

void vdchunk_asn1_write_hex(vdchunk_base *tag, int depth)

//! Prints the tag as hex; indents spaces according to OMOSAP output format;
//! the number of bytes per line, the number of bytes not skipped is 
//! as well as the output stream are defined in "vdchunk_asn1.h"

//! \param tag Tag to be displayed
//! \param depth Parse depth

// XXXf Some calls should be linked here

{
	string tabs;
	if (tag->entries > 0) {
		tabs = asn1_simple_spaces(depth);

		tag->write_hex(VDCHUNK_ASN1_STREAM, tabs + "     ", 
			VDCHUNK_ASN1_CHARPERLINE, 32, VDCHUNK_ASN1_CHARTOTAL); 
		fprintf(VDCHUNK_ASN1_STREAM,"\n");
	};
}

string dumpasn1(string oid)

//! Parses the file "dumpasn1.cfg" from Peter Gutmann (if it exists) and returns the string
//! value of the comment corresponding to the OID

//! \param oid OID

{
	string res = "Unknown";
	string line, term, oidc;
	bool finished, found;
	int pos, count;

	ifstream rein;
	rein.open(VDCHUNK_ASN1_DUMPASN1CFG_FILENAME);

	finished = false;
	if (rein.good()) {
		// File exists
#ifdef VDCHUNK_ASN1_DEBUG_MSG3
		cout << "Parsing file containing OID definitions...." << endl;
#endif

		// Replace "."s by spaces in the reference string
		oidc = oid;
		finished = false; count = 0;
		while (!finished) {
			// Loop over all "."s
			pos = oidc.find(".",1);
			if ((pos != string::npos) && (count < 256)) {
				// Replace
				oidc.replace(pos, 1, " ");
				count++; // Resilient programming: no indefinite loops
			}
			else {
				finished = true;
			};
		}; // while

		finished = false; found = false; count = 0;
		while (rein.good() && !finished) {

			getline(rein, line, '\n');
			if (line == "") {
				// Ignore empty lines
			}
			else if ((line.substr(0,1) == "#") ||
				(line.substr(0,1) == "%") ||
				(line.substr(0,1) == "$")) {
				// Ignore comments
			}
			else if (line.substr(0,6) == "OID = ") {
				term = line.substr(6, line.size()-6);
				if (term == oidc) {
#ifdef VDCHUNK_ASN1_DEBUG_MSG4
					cout << line << endl;
					cout << "Match!" << endl;
#endif
					found = true;
				};
			}
			else if (found && (line.substr(0,14) == "Description = ")) {
				term = line.substr(14, line.size()-14);
#ifdef VDCHUNK_ASN1_DEBUG_MSG3
				cout << line << endl;
#endif
				res = term;
				finished = true;
			}
			else {
//				cout << line << endl;
			};
		}; // while
	};

	rein.close();
	return res;
}

#include "known_tags.cpp"

// IN 2011-01-16
// Old function for OID names removed
// #include "known_oids.cpp"

