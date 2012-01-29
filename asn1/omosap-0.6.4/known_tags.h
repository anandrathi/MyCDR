
// XXX

#ifndef VDCASN1_KNOWN_TAGS_VERSION
#define VDCASN1_KNOWN_TAGS_VERSION "0.3"
#define VDCASN1_KNOWN_TAGS_DESCRIPTION "Light Version"

// #include <vdchunk-cryptopp.h> // The Versatile Data Chunk for Crypto++
#include <vdchunk-simple.h> // The Versatile Data Chunk (Base Version)
#define VDCHUNKSIMPLE vdchunk_simple

using namespace std;

int parse_known_tags(vdchunk_base *tag);

#endif
