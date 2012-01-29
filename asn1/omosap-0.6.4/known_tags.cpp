
//
// KNOWN_TAGS.CPP
//
// Special Parsing Methods 
//
//

int parse_known_tags(vdchunk_base *tag)

{
	int res;
	char c,d;
	
	res = 0;

	tag->parse_reset();
	c = tag->parse_look_ahead(0);
	d = tag->parse_look_ahead(1);

	// Put your special parsing functions here:

/*
	// Example:
	if ((c == 0x07f) && (d == 0x021)) {
		fprintf(stdout,"Parsing CV Certificate ....\n");
  		parse_card_certificate(tag);
		res = 1;
	}
*/

	return res;
}
