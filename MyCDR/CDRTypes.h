#ifndef AR_CDRTYPES_H
#define AR_CDRTYPES_H

#include <map>
typedef std::map<std::string, std::string> RECKEYS;
#define CONFIG_FILE "CDR.CNF"
#define CONFIG_FILE_IN "e:/ani/MyCDR/CDR_IN_FORMAT.config"
#include <ace/Basic_Types.h>

typedef ACE_INT8	AR_INT8 ;
typedef ACE_UINT8	AR_UINT8;
typedef ACE_INT16	AR_INT16;
typedef ACE_UINT16	AR_UINT16;
typedef ACE_INT32	AR_INT32;
typedef ACE_UINT32	AR_UINT32;
typedef ACE_INT64	AR_INT64;
typedef ACE_UINT64	AR_UINT64;
enum PARSESTATE { FIELDEND, LINEEND , FILEEND};

#define ONELOGGERKB 1024
#define ONELOGGERMB (1024*1024)
#define ONELOGGERGB (1024*1024*1024)

#endif