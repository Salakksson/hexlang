#include "interpret.h"

struct bsinfo
{
    int idc1;
    int line;
    int codeptr;

    struct bsinfo* last;

};

void msleep(int l)
{
    usleep(1000 * l);
}

bool isHexDigit(char c) 
{
    return isxdigit((unsigned char)c) != 0;
}

void clean(char* code)
{
    int setptr = 0;
    int codeptr = 0;
    bool comment = false;
    while (code[codeptr])
    {
        code[setptr] = code[codeptr];
        if (comment && (code[codeptr] != '\n'))
        {
            codeptr++;
            continue;
        }  
        switch(code[codeptr])
        {
        case '\n':
            comment = false;
            codeptr++;
            break;
        case ';':
            comment = true;
            codeptr++;
            break;
        default:
            codeptr++;
            break;
        }
        
    }
    code[setptr] = code[codeptr];
}

void outC(unsigned int c)
{
    printf("%c ", (char)c);
}
void outD(unsigned int d)
{
    printf("%d ", d);
}
void outX(unsigned int x)
{
    printf("%x ", x);
}

void inC(void* c)
{
    scanf("%c", (char*)c);
}
void inD(void* d)
{
    scanf("%hhd ", (unsigned char*)d);
}
void inX(void* x)
{
    scanf("%hhx ", (unsigned char*)x);
}
typedef void (*outF)(unsigned int);
typedef void (*inF)(void*);

void interpret(char* code, uint32_t tags)
{
    //Array setup
    int array_size = 0x100;
    if(tags % 2)
        array_size = 0x10000;
        
    unsigned char* array = (unsigned char*)malloc(array_size);
    for (unsigned char* carr = array; carr++ - array <= array_size; *carr = 0);
    array[0] = 1;
    if ((tags >> 2) % 2)
        array[1] = array_size - 1;

    //Essencials
    int codeptr = 0;
    int idc1 = 0;
    int idc2 = 0;
    int length = 0;
    struct bsinfo* bracket = 0;
    int line = 1;

    outF outf = outC;
    if(tags & FLAGB_OUTX)
        outf = outX;
    if(tags & FLAGB_OUTD)
        outf = outD;

    inF inf = inC;
    if(tags & FLAGB_INX)
        inf = inX;
    if(tags & FLAGB_IND)
        inf = inD;
      
    clean(code);
    while (code[codeptr])
    {
        if (isHexDigit(code[codeptr]))
        {   
            
            sscanf(code+codeptr, "%x%n", &idc1, &length);
            if (idc1 >= array_size)
            {
                printf("Line %d: ERROR: Pointer too large\n", line);
                return;
            }
            codeptr+=length;
        }
        else
        {
            switch(code[codeptr])
            {
            case '+':
                sscanf(code + (++codeptr), "%x%n", &idc2, &length);
                codeptr+=length;
                array[idc1] += array[idc2];
                break;
            case 'i':
                inf((void*)&array[idc1]);
                codeptr++;
                break;
            case 'o':
                outf(array[idc1]);
                codeptr++;
                break;
            case '{':
                struct bsinfo nbracket = {idc1, line, codeptr, bracket};
                bracket = &nbracket;
                codeptr++;
                break;
            case '}':
                if (bracket == 0)
                {
                    printf("Line %d: ERROR: Closing non existing brace\n", line);
                    return;
                }
                if (array[bracket->idc1] == 0)
                {
                    codeptr++;
                    break;
                }
                idc1 = bracket->idc1;
                line = bracket->line;
                codeptr = bracket->codeptr;
                bracket = bracket->last;
                break;
            case '\n':
                line++;
                codeptr++;
                break;
            default:
                codeptr++;
                break;
                
            }
            

        }
        
    }
    putc(0xA, stdout);
    
}

void interpret_debug(char* code, uint32_t tags)
{
    int array_size = 0x100;
    if(tags % 2)
        array_size = 0x10000;

    unsigned char* array = (unsigned char*)malloc(array_size);
    for (unsigned char* carr = array; carr++ - array <= array_size; *carr = 0);
    array[0] = 1;
    if ((tags >> 2) % 2)
        array[1] = array_size - 1;
    int codeptr = 0;
    int idc1 = 0;
    int idc2 = 0;
    int length = 0;
    int line = 1;
    bool comment = false;
    struct bsinfo* bracket = 0;
    int i = 0;
    printf("line 1: ");
    while (code[codeptr])
    {
        i++;
        if(i > 500)
            break;
        
        fflush(stdout);
        msleep(10);
        if (comment && (code[codeptr] != '\n'))
        {
            codeptr++;
            continue;
        }  
        printf("%c", code[codeptr]);
        if (isHexDigit(code[codeptr]))
        {   
            
            sscanf(code+codeptr, "%x%n", &idc1, &length);
            if (idc1 >= array_size)
            {
                printf("Line %d: ERROR: Pointer too large\n", line);
                return;
            }
            codeptr+=length;
        }
        else
        {
            switch(code[codeptr])
            {
            case '+':
                sscanf(code + (++codeptr), "%x%n", &idc2, &length);
                codeptr+=length;
                array[idc1] += array[idc2];
                printf("%x", idc2);
                break;
            case 'o':
                printf("%d ", array[idc1]);
                codeptr++;
                break;
            case '{':
                struct bsinfo nbracket = {idc1, line, codeptr, bracket};
                bracket = &nbracket;
                codeptr++;
                break;
            case '}':
                if (bracket == 0)
                {
                    printf("Line %d: ERROR: Closing non existing brace\n", line);
                    return;
                }
                if (array[bracket->idc1] == 0)
                {
                    codeptr++;
                    break;
                }
                idc1 = bracket->idc1;
                line = bracket->line;
                codeptr = bracket->codeptr;
                bracket = bracket->last;
                break;
            case '\n':
                line++;
                printf("line %d: ", line);
                comment = false;
                codeptr++;
                break;
            case ';':
                comment = true;
                codeptr++;
                break;
            default:
                codeptr++;
                break;
                
            }
            

        }
        
    }
    putc(0xA, stdout);
    
}
