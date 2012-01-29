
// Doxygen documentation for OMOSAP
// Last modified: January 16th, 2011

// (C) 2008-2011 Ingo Naumann

/*! \mainpage OMOSAP -- One More Open-Source ASN1 Parser
 *
 * \section intro_sec Introduction
 *
 * Here are some introductory questions:
 *
 * What is ASN1? See: http://en.wikipedia.org/wiki/Asn1
 *
 * What is a parser? See: http://en.wikipedia.org/wiki/Parser
 *
 * What is an ASN1 parser? Ok, now it gets interesting. An ASN1 parser
 * can be one of two things: either a tool to convert
 * ASN1 descriptions like
 *
 * \code
 *
 * MyBlob ::= SEQUENCE {
 *        nonce     INTEGER
 *        field     BIT STRING
 * }
 *
 * \endcode
 *
 * into a programming language that can be compiled into an executable,
 * e.g. C++, or a tool to read and interpret (=parse) cryptic data blobs like
 *
 * \code
 *
 * 30 07 02 01 42 03 02 47 11
 * 
 * \endcode
 *
 * which were generated according to an ASN1 syntax. I will use
 * the terms "ASN1 compiler" for the former and "ASN1 parser" for the
 * latter. Basically, an ASN1 compiler generates an ASN1 parser using 
 * the formal ASN1 description as input.
 * 
 * According to these terms OMOSAP is an ASN1 parser.
 * In that sense, if you need more than just visualizing ASN1 tags,
 * it is your task to adapt the OMOSAP source code to your
 * specific needs. By doing so, you can 
 * create a parser which follows your specific ASN1 syntax rules.
 * OMOSAP already contains a number of functions that can parse 
 * (and display) standard ASN1 tags.
 * A possible output of OMOSAP parsing an RSA key could for example look 
 * like the following:
 * 
 * \code
 *
 * 30 : SEQUENCE (Length: 0x941)
 *  02 : Integer (Length: 0x01)
 *       00
 *  30 : SEQUENCE (Length: 0x0D)
 *   06 : Object Identifier (OID): 1.2.840.113549.1.1.1 rsaEncryption
 *   05 : NULL
 *  04 : Octet String (Length: 0x92B)
 *   30 : SEQUENCE (Length: 0x927)
 *    02 : Integer (Length: 0x01)
 *         00
 *    02 : Integer (Length: 0x201)
 *         00
 *         98 E3 26 19 3A CC 3C A5 FD 4F 54 DA 93 00 4F D9 0D BD 40 6B 
 *         FA D4 40 AE 78 B7 5A CB E2 0A FF 6A 89 7C EE E8 C2 B0 63 AD 
 *         85 3C 6A 81 8C 61 46 A2 7F 69 0A 93 9E F5 55 58 D2 CB 13 40 
 *         E7 F8 37 F2 82 A9 EE D8 17 37 F4 52 5F 57 CF 9F AB 90 35 D1 
 *         [0x1B0 bytes skipped.]
 *    02 : Integer (Length: 0x01)
 *         11 
 *    02 : Integer (Length: 0x200)
 *         35 F5 D1 36 14 C0 8D E0 3B 49 2D 01 D9 87 A3 B6 04 D9 62 07 
 *         FE 2C CB 88 DF 4F C5 B1 5E D6 B4 7F F4 4A 36 34 08 7A 7D 88 
 *         89 60 9E 0F 9A F5 27 FD 1D E8 D6 8E 74 56 96 97 D1 ED 52 16 
 *         E8 75 B9 64 A6 96 54 4C 44 6E 19 FE F4 79 58 56 78 C9 7C 67 
 *         [0x1B0 bytes skipped.]
 *    02 : Integer (Length: 0x101)
 *         00
 *         B6 5E 95 E4 75 8C 2F BF 71 83 44 40 AE DB CB BE AB 8C 5A 62 
 *         A5 7E 54 55 67 82 0C AE AE DB 8A 2F B1 8C 05 D3 37 5C 1A FA 
 *         75 A7 BB DA 33 67 97 ED 41 7C 59 D5 94 ED D9 43 E0 E5 DA 72 
 *         4A 5E DC 4E BE A9 0E 9F 8B 00 9F EB 20 4E 49 D7 47 F6 26 FA 
 *         [0xB0 bytes skipped.]
 *    02 : Integer (Length: 0x101)
 *         00
 *         D6 9D 55 55 84 4D FF AC A7 A5 91 E2 A6 A7 7F F7 2F 85 B7 27 
 *         6B 36 A6 63 E5 3F 43 6F 5D E6 FC 61 D9 57 FE 39 7B 8A 1D 37 
 *         94 A8 27 92 27 65 EF 3C 6B C2 44 AF FF BF A8 B8 17 0E 52 F6 
 *         BD FC 9B 4F 45 50 B6 8C 1A 37 3F F2 53 F7 50 DE 83 93 D3 E9 
 *         [0xB0 bytes skipped.]
 *    02 : Integer (Length: 0x100)
 *         15 74 8A 1A E0 A7 14 AD 1C 69 CB CB 5F DD 9F 7F D7 F2 64 FC 
 *         8B F0 BE A0 A2 C4 01 7D F6 74 2E 5F F6 C5 2D DC 9D 19 E5 0E 
 *         68 31 D9 DD 6F 75 99 67 34 E1 73 FB 02 76 55 CB C0 1B 0A A4 
 *         08 BF DD AE E9 41 10 C7 79 C3 D6 94 21 EB 17 BE F9 68 40 D2 
 *         [0xB0 bytes skipped.]
 *    02 : Integer (Length: 0x101)
 *         00
 *         B0 BD CD CD F4 7C 78 33 D5 5B 2C D8 C5 7A E1 DA 9F 9B 4B 89 
 *         DF D2 A7 25 17 25 0A 5B B6 BE 39 41 85 CF FE 89 B1 08 54 4B 
 *         E3 D5 C6 3C 20 72 10 4F E0 45 A1 FA 5A 25 5D C4 C7 B1 71 7F 
 *         E7 C0 F8 5F 66 42 78 37 24 A5 F8 6D 36 16 F7 4D D5 C5 08 DE 
 *         [0xB0 bytes skipped.]
 *    02 : Integer (Length: 0x101)
 *         00
 *         80 3F 1C E1 87 3C F6 41 97 EB BC 64 BF 61 D7 B4 9E 09 EC 94 
 *         34 9E A3 FD A8 6D 02 BD D1 E3 65 9B DB FB 6A 40 3C 8B AD 5F 
 *         8F 71 38 4E F3 F2 1E 5E D2 BD 6E AE EA ED 12 C8 CC 63 ED C6 
 *         56 12 A4 26 8C 23 A7 EA BF E4 09 B7 23 85 68 FC 3E 93 6A F5 
 *         [0xB0 bytes skipped.]
 * 
 * \endcode
 *
 * This could have been the output of these two lines (apart from some
 * lines of initialization):
 *
 * \code
 *
 *  data->read_binary_file(filename);     // Read ASN1 tag from file
 *  parse_asn1(data);                     // Parse ASN1 tag
 *
 * \endcode
 *
 * However, printing the contents of an ASN1 tag is only the default action.
 * It is possible to provide the parser with a list of tasks (actions) which
 * can be implemented into the OMOSAP source code (if they don't exist). In
 * this case, calling the parser could look like this:
 *
 * \code
 *
 *  action->change_byte(SAVE_EXPONENT, 1); // Save exponent of RSA key
 *  parse_asn1(data, action);              // Parse ASN1 tag
 *
 * \endcode
 *
 * OMOSAP can display the names of object identifiers 
 * (as it does in the above example)
 * if the file "dumpasn1.cfg"
 * from Peter Gutmann's homepage exists in the current directory:
 * http://www.cs.auckland.ac.nz/~pgut001/dumpasn1.cfg 
 *
 * OMOSAP uses Distinguished Encoding Rules (DER). It should be noted that
 * clear, understandable source code ranks first in the order of priority
 * for this project (and, immediately, the disclaimer: some parts of this
 * software are still under construction). The second focus lies on stability.
 *
 * Performance issues are considered but play a minor role.
 *
 * Ingo Naumann
 *
 * January 2011
 *
 * Project homepage: http://research.naumachiarius.com/software/omosap.html
 * 
 * \section install_sec Installation
 *
 * It's simple. Just follow those steps:
 *
 *  1. Make sure you have installed the package "vdchunk_base" or
 *     install it if this is not the case (at least version 0.2.9). 
 *     You can download the library from
 *     http://research.naumachiarius.com/software/vdchunk.html   
 *
 *  2. 'cd' to the directory containing the Omosap's source code 
 *
 *  3. Type 'make' to compile the package.
 *
 *  4. Parse your binary file using the command './omosap filename'
 *	 
 *  5. If you like, copy the Omosap executable into '/usr/local/bin'.
 * 
 *  6. If you need the names of object identifiers just copy the file 'dumpasn1.cfg'
 *     into the directory where you want to parse (see above).
 */
