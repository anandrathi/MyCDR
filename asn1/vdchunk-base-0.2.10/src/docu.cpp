
// Last modified: December 13th, 2011

/*! \mainpage VDChunk -- The Versatile Data Chunk 
 *
 * \section intro_sec Introduction
 *
 * VDChunk is an multi-purpose C++ library that provides some classes which 
 * allow reading, writing and manipulating data blocks ("chunks") without 
 * have to take care too much about strings, streams and numbering formats. 
 *
 * Ingo Naumann
 *
 * December 2011
 *
 * Project homepage: http://research.naumachiarius.com/software/vdchunk.html
 * 
 * \section why Why VDChunk?
 *
 * You are probably here because VDChunk is required by some other program
 * that you want to compile.
 *
 * \section Installation
 *
 * It's the classical Automake/Autoconf installation.
 * The simplest way to compile this package is:
 *
 *  1. `cd' to the directory containing the VDChunk's source code and type
 *     `./configure' to configure the package for your system.  
 *
 *  2. Type `make' to compile the package.
 *
 *  3. Type `make install' to install the programs and any data files and
 *     documentation.
 *
 *  4. Type `make install' to install the programs and any data files and
 *     documentation.
 *
 *  5. Type 'sudo make install' (or, if you are root: 'make install')
 *
 * This will install the library in the directory '/usr/local/lib/'. If 
 * you want it to be installed in '/usr/lib/', you can give an option 
 * to the "configure" command:
 * 
 * "./configure --prefix=/usr"
 * 
 * This is necessary for some Linux distributions that do not look into 
 * '/usr/local/lib/" when searching for libraries (e.g. some versions of
 * Ubuntu, kubuntu).
 * Alternatively, you could soft-link the library to the proper place, 
 * e.g.:
 * 
 * "sudo ln -s /usr/local/lib/libvdchunk-base.so.0 /usr/lib/libvdchunk-base.so.0"
 * 
 * Another option would be to change the environment variable "LD_LIBRARY_PATH" 
 * (however, this is usually not recommended anymore).
 *
 * You can remove the program binaries and object files from the
 * source code directory by typing 'make clean'.  
 *       
 */
