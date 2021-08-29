#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define SIZE_WORD 31
#define SIZE_LINE 80
#define START_MEMORY 100
#define MACHINE_RAM 2000
#define EXT_READ ".as"
#define EXT_OBJECT ".ob"
#define EXT_EXTERN ".ext"
#define EXT_ENTRY ".ent"


typedef struct{
        char LabelName [SIZE_WORD];/*name of the label*/
        int address;
        int isExternal;/* put a 0 if it's not external, 1 if it is external, 2 if it is internal.*/
        int isCode;
           }sign;

struct extLabelStruct {/*sturct for an external and enternal lable!!*/
         char label[SIZE_WORD];
         short address;
          } ;

struct data{/*struct for the datae and variables*/
   signed variable : 14;
           };


struct cmd { /* stores commands such as mov, cmp, inc etc. */
char name[5]; /* 4 chars + '0' */
int code; /* code between 0 and 15 */
int operands; /* how many operands for this command (0-2) */
int permit[2][4]; /* table of 2*4 permittable source/destinations operands: immediate, direct, indirect, register */
};

 typedef union /* stores the data of ogrim*/
 {
   struct
   {
  unsigned kind : 2;
  unsigned destination : 6;/*source oger*/
  unsigned source : 6;/*destination oger*/
    }reg;
   unsigned short fullReg;
  }regist;
 
 typedef union /*stores data of variables types shitat miyun 1 and 2.*/
 {
   struct
   {
  unsigned kind : 2;
  unsigned num : 12;
    }a;
   unsigned short fullReg;
  }addressing;

typedef union /*stores the name of code instruction*/
 {
struct
{
unsigned era : 2;
unsigned opDest : 2;
unsigned opSource : 2;
unsigned opcode : 4;
unsigned param1 : 2;
unsigned param2 : 2;
} u;
unsigned short fullReg;
}cpuOpcode;




void address(sign [] );
int checkcommand(char []);
int checkguide(char []);
int CheckLabel(char []);
int Checkextern(char []);
int direct (char[]);
int instant(char []);
int jump(FILE * );
int LabelInList(char []);
void readfile1(FILE *);
void readfile2 (FILE *);
int registerDirect(char []);
int saveNum (char []);
int typeParam(char []);
int whitespace(FILE *);
void writeEntry(FILE *);
void writeExternal(FILE *);
void writeObject(FILE *);






