/**
 * Aviate, created by James Phillips <james@jamesphillipsuk.com> 2016 - 2019.  
 * Aviate is a terminal-based text-editor, designed for *Nix systems.  
 * 
 * Copyright (C) 2016, 2019 James Phillips, Released under the GNU GPL v3.0 or later.
 * 
 * This program is free software: you can redistribute it and/or modify it under the terms of the 
 * GNU General Public License as published by the Free Software Foundation, either version 3 of the License, 
 * or (at your option) any later version.  This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.  
 * You should have received a copy of the GNU General Public License along with this program.  
 * If not, see <http://www.gnu.org/licenses/>.
 *
 * Initially created: April 2016
 * Rewritten: December 2018 - April 2019
**/

#include <stdio.h> /*I/O Library*/
#include <stdlib.h>/*Standard (File Handling) Library*/
#include <stdbool.h>/*Standard Boolean Library*/
#include <string.h>/*String Handling Library*/
#include <errno.h>/*Error Handling Library*/
#include <unistd.h>/*POSIX OS Interaction API Library*/
#include <ncurses.h>/*NCurses (text editing interface) Library*/

#define CLEAR() printf("\e[2J\e[H");/*Output keycode to clear UNIX Terminal*/
#define CLEARLN() printf("\e[2K\r");/*Output keycode to clear one line in UNIX Terminal*/
#define CTRL(c) ((c)&037)/*The key modifier for the control key.*/

typedef struct textFile
{
  char *text;
  int length;
  int cursorPosition;
} textFile;

void printUIFrame()/*Prints the UI to the screen.  Title bar at the top, then top and bottom boundaries of the content area.*/
{
  printf("╔══════════════════════════════════════════════════════════════════════════════╗\n");
  printf("║                                    Aviate                                    ║\n");
  printf("╚══════════════════════════════════════════════════════════════════════════════╝\n");
  printf("╔══════════════════════════════════════════════════════════════════════════════╗\n");
  printf("\033[24;0H╚══════════════════════════════════════════════════════════════════════════════╝");
  printf("\033[5;0H");
}

void printUIIntroduction()/*This function prints an intro to Aviate to the screen.*/
{
  CLEAR();
  printUIFrame();
  printf("\033[5;26H Welcome to Aviate Alpha 3.0!\033[6;9H Aviate: the C-based text-editor designed for UNIX-like systems.\033[7;30H - by James Phillips.\033[8;19H Donate to me at: paypal.me/JamesPhillipsUK\033[23;27H Press [ENTER] to continue.");
  getchar();
}

void printHowToNotice()/*This function prints the instructions to the screen.*/
{
  CLEAR();
  printUIFrame();
  printf("\033[5;26H Welcome to Aviate Alpha 3.0!\033[6;2H To use Aviate, follow these simple instructions, and you'll be on your way:\n 1| To start writing a new file, use the command: \"Aviate Write x.y\" in your\n     Bash terminal.\n 2| To read or write to a pre-existing file, use: \"Aviate Read x.y\".\n 3| Once Aviate has started, to cut your current line of text, use: \"[CTRL]+K\".\n     To paste, use: \"[CTRL]+U\".\n 4| To save your work, use: \"[F1}\".  To Exit, use \"[F2]\".\033[23;27H Press [ENTER] to continue.");
  getchar();
}

void printCopyrightNotice()/*This function prints the copyright/left notice to the screen.*/
{
  CLEAR();
  printUIFrame();
  printf("\033[5;2H Copyleft Notice: \n");
  printf("Aviate, created by James Phillips <james@jamesphillipsuk.com> on 3rd April 2016\n\n  Aviate is a terminal-based text-editor, designed for *Nix systems.\n\n  This program is free software: you can redistribute it and/or modify\n it under the terms of the GNU General Public License as published by\n the Free Software Foundation, either version 3 of the License, or\n (at your option) any later version.\n\n  This program is distributed in the hope that it will be useful,\n but WITHOUT ANY WARRANTY; without even the implied warranty of\n MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n GNU General Public License for more details.\n\n  You should have received a copy of the GNU General Public License\n along with this program.  If not, see <http://www.gnu.org/licenses/>.\033[23;25H Press [ENTER] to Exit.");
  getchar();
}

void getFileContents(char (**fileNamePointer)[256], textFile *text)/*This prints the content of the file called by the user to the screen.*/
{
  FILE *filePointer = fopen(*(fileNamePointer + 0 + 0)[0], "r");/*Read the file*/
  char c;
  if (filePointer == NULL)/*If it can't access or find the file*/
  {
    printf("Error opening file: %s!\n", strerror(errno));
    exit(0);/*Throw it.*/
  }
  
  while (c != EOF)/*Read the file to the textFile text.*/
  {
    c = fgetc(filePointer);
    text->text = realloc(text->text, text->length + 1);
    text->text[text->length] = c;
    text->length++;
  }
  fclose(filePointer);/*Save the computer, close the file.*/
}

void saveFile(char (***fileNamePointer)[256], textFile **text)
{
  FILE *filePointer = fopen(**(fileNamePointer + 0 + 0)[0], "w");
  if (filePointer == NULL)
  {
    printf("Error opening file: %s!\n", strerror(errno));
    exit(1);
  }
  text[0]->cursorPosition = 0;/*Point to the start of the file.*/
  while (text[0]->length > text[0]->cursorPosition) /*Read through one char at a time.*/
  {
    if (text[0]->text[text[0]->cursorPosition] != '\0')
      fprintf(filePointer, "%c", text[0]->text[text[0]->cursorPosition]);
    text[0]->cursorPosition++;
  }
  fclose(filePointer);
}

void rewriteFile(char (**fileNamePointer)[256], textFile *text)/*This allows the user to edit a pre-existing file.*/
{
  initscr();/*Initialise NCurses.*/
  cbreak();/*Enable Character-at-a-time input.*/
  noecho();/*Don't auto-output to the screen.  I'll handle that.*/
  keypad(stdscr, TRUE);/*Take special key inputs as well.*/
  refresh();/*Refresh the screen*/
  move(0,0);
  text->cursorPosition = 0;
  while (text->length > text->cursorPosition) /*Read through one char at a time.*/
  {
    addch(text->text[text->cursorPosition]);
    refresh();
    text->cursorPosition++;
  }
  text->cursorPosition = text->length - 1;
  for(;;)
  {
    bool canBreak = false;
    int input = getch();
    switch(input)
    {
      case KEY_UP:
        break;
      case KEY_DOWN:
        break;
      case KEY_LEFT:
        break;
      case KEY_RIGHT:
        break;
      case KEY_BACKSPACE:

        break;
      case KEY_F(1):
        saveFile(&fileNamePointer, &text);
        /*DEBUG only, print it to screen.  going to have to find a nice way of telling the user they've saved though
        
        printw("Savepoint");
        refresh();*/
        break;
      case KEY_F(2):
        canBreak = true;
        break;
      default:
        addch(input);
        text->length++;
        text->text = realloc(text->text, text->length + 1);
        text->text[text->cursorPosition] = input;
        text->cursorPosition++;
        break;
    }
    refresh();
    
    if (canBreak == true)
      break;
  }
  getch();
  endwin();/*Close out NCurses.*/
}

bool checkIfFileExists(char (***fileNamePointer)[256])
{
  FILE *checkIfOpen = fopen(**(fileNamePointer + 0 + 0)[0], "r");
  if (checkIfOpen == NULL) /*File does not exist.*/
  {
    fclose(checkIfOpen);
    return false;
  }
  fclose(checkIfOpen);
  return true;
}

void writeFile(char (**fileNamePointer)[256])/*Writes an empty file.*/
{
  char okayToOverwrite = 'N';
  if (!checkIfFileExists(&fileNamePointer))
    okayToOverwrite = 'Y';
  else
  {
    printf("File Already Exists.  Press \"Y\" to overwrite.");
    okayToOverwrite = getchar();
  }
  
  if (okayToOverwrite == 'Y' || okayToOverwrite == 'y')
  {
    FILE *filePointer = fopen(*(fileNamePointer + 0 + 0)[0], "a");

    if (filePointer == NULL)
    {
      printf("Error opening file: %s!\n", strerror(errno));
      exit(1);
    }
    fprintf(filePointer, "%c", '\0');
    fclose(filePointer);
  }
  else
  {
    printf("Error: Could not overwrite file.!\n");
    exit(1);
  }
}

void initialiseTextFile(textFile *text)
{
  text->length = 0;/*We're going to initialise it with 0 chars*/
  text->cursorPosition = 0;
  text->text = (char *) malloc(sizeof(char) * (text->length));/*Allocate memory space for 0 chars*/
  text->text[text->length] = '\0';/*Null char to initialise it.*/
}

void handleReadWriteOrNothing(short readWriteOrNothingOutput, char (*fileNamePointer)[256])/*Handles what actions are taken when the user selects to read or write a file.*/
{
  textFile text; /*Create a textFile variable*/
  initialiseTextFile(&text);
  switch(readWriteOrNothingOutput)
  {
    case 1:/*No extra Args*/
      printHowToNotice();
      printCopyrightNotice();
      break;
    case 2:/*Read A File*/
      CLEAR();
      getFileContents(&fileNamePointer, &text);
      rewriteFile(&fileNamePointer, &text);
      CLEAR();
      break;
    case 3:/*Write a new File*/
      CLEAR();
      writeFile(&fileNamePointer);
      rewriteFile(&fileNamePointer, &text);
      break;
    default:/*Unknown Command*/
      printf("Please provide your command in the format: \"Aviate <operation> <filename>\".  Exiting.\n");
      break;
  }
}

short readWriteOrNothing(int *argc, char ***argv, char (*fileNamePointer)[256])/*Returns a value corresponding to whether the user wants to read or write a file.*/
{
  short returnVal = 0;
  if (*argc == 1)
  {
    for (int count = 0; count < 256; count++)
      (*fileNamePointer)[count] = '\0';
    returnVal = 1;
  }
  else if (*argc == 3)
  {
    if (strcmp(argv[0][1], "Read\0") == 0 || strcmp(argv[0][1], "read\0") == 0)
      returnVal = 2;
    else if (strcmp(argv[0][1], "Write\0") == 0 || strcmp(argv[0][1], "write\0") == 0)
      returnVal = 3;
    else
      printf("%s is not a known command.  Try Read or Write.\n", argv[0][1]);
    for (int count = 0; count < 256; count++)
      (*fileNamePointer)[count] = argv[0][2][count];
  }
  return returnVal;
}

int main(int argc, char **argv)
{
  char fileName[256];

  printUIIntroduction();
  handleReadWriteOrNothing(readWriteOrNothing(&argc, &argv, &fileName), &fileName);
  return EXIT_SUCCESS;
}