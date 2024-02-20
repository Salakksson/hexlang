#include <stdint.h>

#define FLAG_HELP           "-help"
#define FLAG_FILENAME       "-f"
#define FLAG_INPUT          "-i"
#define FLAG_INFFILE        "-if"

#define FLAG_DEBUG          "-debug"   
#define FLAG_ARRAY16        "-a16"
#define FLAG_INT16          "-i16"
#define FLAG_OUTD           "-od"
#define FLAG_OUTX           "-ox"
#define FLAG_IND            "-id"
#define FLAG_INX            "-ix"

typedef enum
{
    FLAGB_DEBUG = 1,         
    FLAGB_ARRAY16 = 2,
    FLAGB_INT16 = 4,
    FLAGB_OUTD = 8,
    FLAGB_OUTX = 16,
    FLAGB_IND = 32,
    FLAGB_INX = 64

} flag;
