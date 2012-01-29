
#include <iostream>
#include "vdchunk-simple.h"

using namespace std;
using namespace vdchunk;

// ** MAIN **


int main(int argc, char* argv[])

{
  cout << "The Versatile Data Chunk (Base Version)" << endl;
  cout << "(C) 2006-2010 Ingo Naumann" << endl;
  cout << "Version: " << PACKAGE_VERSION << endl;

  vdchunk_simple *x;
  x = new vdchunk_simple("TEST", 384);

  cout << "Ok." << endl;
  return 0;
}

/*

**********

  vdchunk_simple *x;
  x = new vdchunk_simple("TEST", 384);

  x->add_string("abcdefghijklmnopqrstuvwxyz");
  x->write_string();
  cout << endl;

  vdchunk_base_cutout *y;
  y = new vdchunk_base_cutout("Y", x, 2, 24);
  y->write_string();
  cout << endl;

  unsigned char c;
  c = y->parse_read();
  c = y->parse_read();
  c = y->parse_look_ahead();

  y->cut_parsed_bytes();
  y->write_string();
  cout << endl;
  
Result:

abcdefghijklmnopqrstuvwxyz
cdefghijklmnopqrstuvwx
efghijklmnopqrstuvwx

**********

*/
