/*
  twitpunch - a short message mechanism using 2 EBCDIC punch cards

   Copyright 2015 Smithee, Spelvin, Agnew & Plinge, Inc.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#define PUNCHED_HOLE 'O'

#define PUNCH12 (0x800)
#define PUNCH11 (0x400)
#define PUNCH10 (0x200)
#define PUNCH01 (0x100)
#define PUNCH02 (0x080)
#define PUNCH03 (0x040)
#define PUNCH04 (0x020)
#define PUNCH05 (0x010)
#define PUNCH06 (0x008)
#define PUNCH07 (0x004)
#define PUNCH08 (0x002)
#define PUNCH09 (0x001)

typedef struct ebcdic_cardpunch
{
  char c;
  unsigned short int punches;
} EBCDIC_CARDPUNCH;

char
  punch
    (int
      row,
     unsigned short int
       card_column);
int
  punch_column
    (char
      c,
    unsigned short int
      cards [],
    int
      *card,
    int
      *column);

unsigned short int
  cards [2*80];
char
  card_image [12*80];

EBCDIC_CARDPUNCH codes [] = {

{' ', 0},
{'.', PUNCH12 | PUNCH08 | PUNCH03},
{'&', PUNCH12},
{'!', PUNCH11 | PUNCH08 | PUNCH02},
{',', PUNCH10 | PUNCH08 | PUNCH03},
{'-', PUNCH11},
{'0', PUNCH10},
{'1', PUNCH01},
{'2', PUNCH02},
{'3', PUNCH03},
{'4', PUNCH04},
{'5', PUNCH05},
{'6', PUNCH06},
{'7', PUNCH07},
{'8', PUNCH08},
{'9', PUNCH09},
{'/', PUNCH10 | PUNCH01},
{'a', PUNCH12 | PUNCH10 | PUNCH01},
{'b', PUNCH12 | PUNCH10 | PUNCH02},
{'c', PUNCH12 | PUNCH10 | PUNCH03},
{'d', PUNCH12 | PUNCH10 | PUNCH04},
{'e', PUNCH12 | PUNCH10 | PUNCH05},
{'f', PUNCH12 | PUNCH10 | PUNCH06},
{'g', PUNCH12 | PUNCH10 | PUNCH07},
{'h', PUNCH12 | PUNCH10 | PUNCH08},
{'i', PUNCH12 | PUNCH10 | PUNCH09},
{'j', PUNCH12 | PUNCH11 | PUNCH01},
{'k', PUNCH12 | PUNCH11 | PUNCH02},
{'l', PUNCH12 | PUNCH11 | PUNCH03},
{'m', PUNCH12 | PUNCH11 | PUNCH04},
{'n', PUNCH12 | PUNCH11 | PUNCH05},
{'o', PUNCH12 | PUNCH11 | PUNCH06},
{'p', PUNCH12 | PUNCH11 | PUNCH07},
{'q', PUNCH12 | PUNCH11 | PUNCH08},
{'r', PUNCH12 | PUNCH11 | PUNCH09},
{'s', PUNCH11 | PUNCH10 | PUNCH02},
{'t', PUNCH11 | PUNCH10 | PUNCH03},
{'u', PUNCH11 | PUNCH10 | PUNCH04},
{'v', PUNCH11 | PUNCH10 | PUNCH05},
{'w', PUNCH11 | PUNCH10 | PUNCH06},
{'x', PUNCH11 | PUNCH10 | PUNCH07},
{'y', PUNCH11 | PUNCH10 | PUNCH08},
{'z', PUNCH11 | PUNCH10 | PUNCH09},

{'A', PUNCH12 | PUNCH01},
{'B', PUNCH12 | PUNCH02},
{'C', PUNCH12 | PUNCH03},
{'D', PUNCH12 | PUNCH04},
{'E', PUNCH12 | PUNCH05},
{'F', PUNCH12 | PUNCH06},
{'G', PUNCH12 | PUNCH07},
{'H', PUNCH12 | PUNCH08},
{'I', PUNCH12 | PUNCH09},

{'"', PUNCH07 | PUNCH08},
{'z', PUNCH01|PUNCH12|PUNCH02|PUNCH11|PUNCH03|PUNCH10|PUNCH04|PUNCH09},
};


int main (int argc, char * argv [])
{
  int
    card;
  int
    column;
  int i;
  int
    j;
  char message [1024];
  int
    msg_length;
  char
    prefix [1024];
  int
    status;


  status = 0;
  column = 1;
  card = 1;
  /*
    initialize
  */ 
  strcpy (message, "Hello, world!");
  msg_length = 2*80;
memset (cards, 0, sizeof (cards));

  /*
    process arguments and run parameters
  */
  if (argc > 1)
    strcpy (message, argv [1]);
  if (msg_length > strlen (message))
    msg_length = strlen (message);

  fprintf (stderr, "twitpunch 0.0.0 EP 01\n");
  fprintf (stderr, "Message (length %d):\n%s\n",
    msg_length, message);

  /*
    for every character in the message "punch" a value in the cards array.
  */
  for (i=0; i<msg_length; i++)
  {
int thing;
thing = column-1;
    status = punch_column (message [i], cards, &card, &column);
  };
{
  int col;
  int row;

for (row=0; row<12; row++)
{
  for (col=0; col<80; col++)
  {
    // first card only (cards index is not adjusted for which card)
    card_image [(row*80)+col] = punch (row, cards [col]);
  };
};

  printf ("   _______________________________________________________________________________\n");
// for each row
for (i=0; i<12; i++)
{
  if (i == 0)
    strcpy (prefix, "  /");
  if (i == 1)
    strcpy (prefix, " / ");
  if (i > 1)
    strcpy (prefix, "|  ");
  printf ("%s", prefix);
  // for each column
  for (j=0; j<80; j++)
{
    printf ("%c", card_image [(i*80)+j]);
};
  printf ("|\n");
};
  printf ("__________________________________________________________________________________\n");
};
  return (0);
}

int
  punch_column
    (char
      c,
    unsigned short int
      cards [],
    int
      *card,
    int
      *column)
{
  int
    max_codes;
  int
    done;
  int
    i;
  int
    index;
  int
    status;


  status = 0;
  index = 0;
  max_codes = sizeof (codes) / sizeof (EBCDIC_CARDPUNCH);
  if (*card > 1)
    index = index + 80;
  index = index + *column - 1;
  done = 0;
  i = 0;
  while (!done)
  {
    if (c == codes[i].c)
    {
      cards [index] = codes[i].punches;
      done = 1;
    };
    if (!done)
    {
      i++;
      if (i == max_codes)
      {
        done = 1;
        status = -1;
      };
    };
  };
  if (status == 0)
  {
    if (*column == 80)
    {
      *column = 1;
      *card = 2;
    }
    else
    {
      *column = *column + 1;
    };
  };
  return (0);
}

char
  punch
    (int
      row,
    unsigned short int
      card_column)

{
  char
    returned_punch;


  returned_punch = ' ';
  if (row ==  0) { if (card_column & 0x800) returned_punch = 'O'; else returned_punch = ' '; }
  if (row ==  1) { if (card_column & 0x400) returned_punch = 'O'; else returned_punch = ' '; }
  if (row ==  2) { if (card_column & 0x200) returned_punch = 'O'; else returned_punch = ' '; }
  if (row ==  3) { if (card_column & 0x100) returned_punch = 'O'; else returned_punch = ' '; }
  if (row ==  4) { if (card_column & 0x080) returned_punch = 'O'; else returned_punch = ' '; }
  if (row ==  5) { if (card_column & 0x040) returned_punch = 'O'; else returned_punch = ' '; }
  if (row ==  6) { if (card_column & 0x020) returned_punch = 'O'; else returned_punch = ' '; }
  if (row ==  7) { if (card_column & 0x010) returned_punch = 'O'; else returned_punch = ' '; }
  if (row ==  8) { if (card_column & 0x008) returned_punch = 'O'; else returned_punch = ' '; }
  if (row ==  9) { if (card_column & 0x004) returned_punch = 'O'; else returned_punch = ' '; }
  if (row == 10) { if (card_column & 0x002) returned_punch = 'O'; else returned_punch = ' '; }
  if (row == 11) { if (card_column & 0x001) returned_punch = 'O'; else returned_punch = ' '; }
  return (returned_punch);
}

