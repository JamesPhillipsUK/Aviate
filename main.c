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

#include <stdio.h>/* I/O Library. */
#include <stdlib.h>/* Standard (File Handling) Library. */
#include <stdbool.h>/* Standard Boolean Library. */
#include <string.h>/* String Handling Library. */
#include <errno.h>/* Error Handling Library. */
#include <ncurses.h>/* NCurses (text editing interface) Library. */

#define CLEAR() printf("\e[2J\e[H");/* Output keycode to clear UNIX Terminal */
#define CLEARLN() printf("\e[2K\r");/* Output keycode to clear one line in UNIX Terminal */
#define ALT_KEY_BACKSPACE 127/* The alternative keycode for Backspace. */
#define SCRWIDTH COLS
#define SCRHEIGHT LINES - 1

/** textFile will be a structured variable type used for storing text files to edit. **/
typedef struct textFile
{
  int length;/* The length of *text. */
  int cursorPosition;/* A cursor position within the textFile. */
  char *text;/* Pointer to a memory location containing chars we've stored.  Used as a dynamic array. */
} __attribute__((packed)) textFile;

/** Prints the UI to the screen.  Title bar at the top, then top and bottom boundaries of the content area. **/
void printUIFrame()
{
  printf("╔══════════════════════════════════════════════════════════════════════════════╗\n");
  printf("║                                    Aviate                                    ║\n");/* The title bar. */
  printf("╚══════════════════════════════════════════════════════════════════════════════╝\n");
  printf("╔══════════════════════════════════════════════════════════════════════════════╗\n");
  printf("\033[24;0H╚══════════════════════════════════════════════════════════════════════════════╝");/* The content area. */
  printf("\033[5;0H");/* Move the cursor to the first space in the content area. */
}

/** This function prints an intro to Aviate to the screen. **/
void printUIIntroduction()
{
  CLEAR();
  printUIFrame();/* Print our satndard UI. */
  printf("\033[5;26H Welcome to Aviate Beta 1.1!\033[6;9H Aviate: the C-based text-editor designed for UNIX-like systems.\033[7;30H - by James Phillips.\033[8;19H Donate to me at: paypal.me/JamesPhillipsUK\033[23;27H Press [ENTER] to continue.");/* Give the user the chance to move on. */
  getchar();
}

/** This function prints the instructions on how to use Aviate to the screen. **/
void printHowToNotice()
{
  CLEAR();
  printUIFrame();/* Print our standard UI. */
  printf("\033[5;26H Welcome to Aviate Beta 1.0!\033[6;2H To use Aviate, follow these simple instructions, and you'll be on your way:\n 1| To start writing a new file, use the command: \"Aviate Write x.y\" in your\n     Bash terminal.\n 2| To read or write to a pre-existing file, use: \"Aviate Read x.y\".\n 3| Once Aviate has started, to cut your current line of text, use: \"[CTRL]+K\".\n     To paste, use: \"[CTRL]+U\".\n 4| To save your work, use: \"[F1]\".  To Exit, use \"[F2]\".\033[23;27H Press [ENTER] to continue.");/* Give the user the chance to move on. */
  getchar();
}

/** This function prints the copyright/left notice to the screen. **/
void printCopyrightNotice()
{
  CLEAR();
  printUIFrame();/* Print our standard UI. */
  printf("\033[5;2H Copyleft Notice: \n");/* Title it, so the user understands what the're reading. */
  printf("Aviate, created by James Phillips <james@jamesphillipsuk.com> on 3rd April 2016\n\n  Aviate is a terminal-based text-editor, designed for *Nix systems.\n\n  This program is free software: you can redistribute it and/or modify\n it under the terms of the GNU General Public License as published by\n the Free Software Foundation, either version 3 of the License, or\n (at your option) any later version.\n\n  This program is distributed in the hope that it will be useful,\n but WITHOUT ANY WARRANTY; without even the implied warranty of\n MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n GNU General Public License for more details.\n\n  You should have received a copy of the GNU General Public License\n along with this program.  If not, see <http://www.gnu.org/licenses/>.\033[23;25H Press [ENTER] to Exit.");/* Give the user the chance to exit. */
  getchar();
}

/** This prints the content of the file called by the user to the screen. **/
void getFileContents(char (*fileNamePointer)[256], textFile *text)
{
  FILE *filePointer = fopen(*(fileNamePointer + 0), "r");/* Read the file. */
  char c = '\0';
  if (filePointer == NULL)/* If it can't access, or find the file: */
  {
    printf("Error opening file: %s!\n", strerror(errno));
    exit(EXIT_FAILURE);/* Throw it. */
  }
  while (c != EOF)/* Read the file to the textFile. */
  {
    c = fgetc(filePointer);/* Get a char from the file. */
    text->text = realloc(text->text, text->length + 1);/* Ask for some heap space for another char. */
    text->text[text->length] = c;/* Add the char to our textFile variable. */
    text->length++;/* Increment the textFile's length. */
  }
  fclose(filePointer);/* Save the computer, close the file. */
}

/** This saves any given textFile to a file and saves the file. **/
void saveFile(char (*fileNamePointer)[256], textFile *text)
{
  FILE *filePointer = fopen(*(fileNamePointer + 0), "w");/* Open the file with write permission. */
  if (filePointer == NULL)
  {
    printf("Error opening file: %s!\n", strerror(errno));
    exit(EXIT_FAILURE);/* If the user can't edit the file (usually a permissions problem), throw that at them. */
  }
  text->cursorPosition = 0;/* Point to the start of the file. */
  while (text->length - 1 >= text->cursorPosition) /* Read through one char at a time. */
  {
    if (text->text[text->cursorPosition] != '\0')
      fprintf(filePointer, "%c", text->text[text->cursorPosition]);
    text->cursorPosition++;
  }
  fclose(filePointer);
}

/** Used to add chars from rewriteFile() to a given textFile variable. **/
void addToTextFileStruct(char charToAdd, textFile *text)
{
  text->length++;/* Add a char to it's length. */
  text->text = realloc(text->text, text->length + 1);/* Allocate the heap memory space needed for the char. */
  text->text[text->cursorPosition] = charToAdd;/* Add the char to the textFile. */
  text->cursorPosition++;/* Move the cursor position along one char. */
}

/** This allows the user to edit text in a pre-existing file.  It can be one that has already been read, or one that has been written. **/
void rewriteFile(char (*fileNamePointer)[256], textFile *text)
{
  initscr();/* Initialise NCurses. */
  cbreak();/* Enable Character-at-a-time input. */
  noecho();/* Don't auto-output to the screen.  I'll handle that. */
  WINDOW *textEdit = newwin(SCRHEIGHT - 2, SCRWIDTH, 0,0);
  keypad(textEdit, TRUE);/* Take special key inputs as well. */
  text->cursorPosition = 0;
  if (text->length != 0)
  {
    while (text->length > text->cursorPosition)/* Read through whatever we've already got in the textFile. */
    {
      waddch(textEdit, text->text[text->cursorPosition]);/* Put the char on the screen. */
      wrefresh(textEdit);
      text->cursorPosition++;
    }
    text->cursorPosition = text->length - 1;
  }
  for(;;)/* We'll process text indefinitely, or until this loop is broken out of. */
  {
    bool canBreak = false;/* Used to process text indefinitely, until this value is true. */
    int input = wgetch(textEdit);
    int y = getcury(textEdit); /* Current cursor y co-ordinate. */
    int x = getcurx(textEdit); /* Current cursor x co-ordinate. */
    switch(input)
    {
      case KEY_UP:/* If the user presses the Up Arrow, move the cursor up respectively. */
        if (y > 0)
        {
          wmove(textEdit, y - 1, x);
          text->cursorPosition -= SCRWIDTH;
          wrefresh(textEdit);
        }
        break;
      case KEY_DOWN:/* If the user presses the Down Arrow, move the cursor down respectively. */
        if (text->cursorPosition < text->length - SCRWIDTH && mvinch(y + 1, x) != '\0' && !(y >= SCRHEIGHT - 2))
        {
          wmove(textEdit, y + 1, x);
          text->cursorPosition += SCRWIDTH;
          wrefresh(textEdit);
        }
        break;
      case KEY_LEFT:/* If the user presses the Left Arrow, move the cursor left respectively. */
        if (x > 0)
          x--;
        else
        {
          y--;
          x = SCRWIDTH - 1;
        }
        wmove(textEdit, y, x);
        text->cursorPosition--;
        wrefresh(textEdit);
        break;
      case KEY_RIGHT:/* If the user presses the Right Arrow, move the cursor right respectively. */
        if (text->cursorPosition < text->length && mvinch(y, x + 1) != '\0' && !(x >= SCRWIDTH - 1))
        {
          wmove(textEdit, y, x + 1);
          text->cursorPosition++;
          wrefresh(textEdit);
        }
        else if(x >= SCRWIDTH - 1)/* If we're at the end of the line, move onto the next one. */
        {
          wmove(textEdit, y + 1, 0);
          text->cursorPosition++;
          wrefresh(textEdit);
        }
        break;
      case KEY_BACKSPACE:/* If the user presses [BACKSPACE], go back one space on both the screen and the textFile, and delete the char they've backspaced over. */
      case ALT_KEY_BACKSPACE:
        if(x > 0)
        {
          wmove(textEdit, y, x - 1);
          wdelch(textEdit);
          text->cursorPosition--;
          text->text[text->cursorPosition] = '\0';
          wrefresh(textEdit);
        }
        break;
      case KEY_F(1):/* If the user presses [F1] it will save the current textFile to a given filename. */
        saveFile(fileNamePointer, text);
        break;
      case KEY_F(2):/* If the user presses [F2], they will exit.  It sets canBreak to true. */
        canBreak = true;
        break;
      case KEY_ENTER:/* If the user presses [ENTER]: */
        for (int remainingChars = SCRWIDTH - 1 - x; remainingChars > 0; remainingChars--)/* Pad out any remaining space on the screen with null chars.  Same in the textFile.  We ignore them in the save function, so it doesn't make the filesize huge. */
        {
          waddch(textEdit, '\0');
          addToTextFileStruct('\0', text);
        }
        waddch(textEdit, '\n');
        addToTextFileStruct('\n', text);/* add a line break to the screen and textFile. */
        break;
      default:
        waddch(textEdit, input);/* Add the char to the screen. */
        addToTextFileStruct(input, text);/* Add the char to the textFile. */
        break;
    }
    wrefresh(textEdit);
    if (canBreak == true)/* Only if the user's told us they want to exit, we exit. */
      break;
  }
  delwin(textEdit);
  endwin();/* Close out NCurses because we've stopped using it. */
}

/** This checks if a given file exists, and returns true if so. **/
bool checkIfFileExists(char (*fileNamePointer)[256])
{
  FILE *checkIfOpen = fopen(*(fileNamePointer + 0), "r");
  if (checkIfOpen == NULL) /* File does not exist.  Return false. */
    return false;
  fclose(checkIfOpen);/* File exists.  Return true. */
  return true;
}

/** Writes an empty file for us to edit. **/
void writeFile(char (*fileNamePointer)[256])
{
  char okayToOverwrite = 'N';/* Defaults to not deleting a file in case a user accidentally puts in a filename that exists already. */
  if (!checkIfFileExists(fileNamePointer))/* If the file doesn't exist, it's okay to write it. */
    okayToOverwrite = 'Y';
  else
  {
    printf("File Already Exists.  Press \"Y\" to overwrite: ");
    okayToOverwrite = getchar();/* If the file exists, find out whether the user really wants it overwritten. */
  }
  
  if (okayToOverwrite == 'Y' || okayToOverwrite == 'y')/* If the user explicitly tells us to overwrite the existing file, or there isn't an existing file. */
  {
    FILE *filePointer = fopen(*(fileNamePointer + 0), "a");/* Open the file with full privileges. */

    if (filePointer == NULL)
    {
      printf("Error opening file: %s!\n", strerror(errno));/* If there's an error throw it back to the user.  Usually they haven't got their permissions right. */
      exit(EXIT_FAILURE);
    }
    fprintf(filePointer, "%c", '\0');/* Initialise it with just a null char. */
    fclose(filePointer);
  }
  else
  {
    printf("Error: Could not overwrite file.!\n");/* If the user didn't explicitly tell us to overwrite, we'll throw that at them.  Better safe than sorry. */
    exit(EXIT_FAILURE);
  }
}

/** This sets initial values to a given textFile.  It also allocates any heap memory needed to do so. **/
void initialiseTextFile(textFile *text)
{
  text->length = 0;/* We initialise it with 0 chars. */
  text->cursorPosition = 0;/* Put the cursor at the start of the file. */
  text->text = (char *) malloc(sizeof(char) * (text->length));/* Allocate memory space for 0 chars. */
  text->text[text->length] = '\0';/* Use a null char to initialise it. */
}

/** Handles what actions are taken when the user selects to read or write a file. **/
void handleReadWriteOrNothing(short readWriteOrNothingOutput, char (*fileNamePointer)[256])
{
  textFile text; /* Create a textFile variable using our textFile struct from above. */
  initialiseTextFile(&text);/* Initialise its contents and allocate the heap memory. */
  switch(readWriteOrNothingOutput)
  {
    case 1:/* No extra args.  If the user hasn't said what they want Aviate to do, tell them what it can do. */
      printHowToNotice();
      printCopyrightNotice();
      break;
    case 2:/* Read a file for reading or editing. */
      CLEAR();
      getFileContents(fileNamePointer, &text);/* Move the file contents to our textFile. */
      text.length--;
      rewriteFile(fileNamePointer, &text);/* Edit the textFile. */
      CLEAR();
      break;
    case 3:/* Write a new file. */
      CLEAR();
      writeFile(fileNamePointer);/* Reserve the file for use. */
      rewriteFile(fileNamePointer, &text);/*Edit the textFile. */
      break;
    default:/* Unknown command. */
      printf("Please provide your command in the format: \"Aviate <operation> <filename>\".  Exiting.\n");
      break;
  }
}

/** Returns a short integer value corresponding to whether the user wants to read or write a file. **/
short readWriteOrNothing(int *argc, char ***argv, char (*fileNamePointer)[256])
{
  short returnVal = 0;
  if (*argc == 1)/* If the user hasn't passed any arguments, fill the filename with null characters. */
  {
    for (int count = 0; count < 256; count++)
      (*fileNamePointer)[count] = '\0';
    returnVal = 1;
  }
  else if (*argc == 3)/* If the user has passed the correct number of arguments (2 + the program call), parse them. */
  {
    if (strcmp(argv[0][1], "Read\0") == 0 || strcmp(argv[0][1], "read\0") == 0)
      returnVal = 2;
    else if (strcmp(argv[0][1], "Write\0") == 0 || strcmp(argv[0][1], "write\0") == 0)
      returnVal = 3;
    else
      printf("%s is not a known command.  Try Read or Write.\n", argv[0][1]);/* Deal with GIGO case. */
    for (int count = 0; count < 256; count++)
      (*fileNamePointer)[count] = argv[0][2][count];/* Assign the given filename to our fileName array in main(). */
  }
  return returnVal;
}

int main(int argc, char **argv)
{
  char fileName[256];/* Stores the filename. */
  printUIIntroduction();/* Introduce the program to the user. */
  handleReadWriteOrNothing(readWriteOrNothing(&argc, &argv, &fileName), &fileName);/* See what arguments the user's passed and handle them. */
  return EXIT_SUCCESS;
}