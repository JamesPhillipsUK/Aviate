/**
 * Aviate, created by James Phillips <james@jamesphillipsuk.com> on 3rd April 2016.  
 * Aviate is a terminal-based text-editor, designed for *Nix systems.  
 * 
 * Copyright (C) 2016 James Phillips, Released under the GNU GPL v3.0 or later.
 * 
 * This program is free software: you can redistribute it and/or modify it under the terms of the 
 * GNU General Public License as published by the Free Software Foundation, either version 3 of the License, 
 * or (at your option) any later version.  This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.  
 * You should have received a copy of the GNU General Public License along with this program.  
 * If not, see <http://www.gnu.org/licenses/>.
 *
 * Created on: 03/04/2016
 * Edited on: 03/04/2016
 * Edited on: 04/04/2016
 * Edited on: 05/04/2016
 * Edited on: 06/04/2016
 * Edited on: 09/04/2016
 * Edited on: 10/04/2016
 * Edited on: 07/06/2016
 * Edited on: 08/06/2016
 * Edited on: 11/06/2016
 * Edited on: 12/06/2016
 * Edited on: 13/06/2016
 * Edited on: 20/09/2016
 * Edited on: 21/09/2016
 * Edited on: 23/09/2016
 * Edited on: 26/09/2016
 * Edited on: 29/09/2016
 * Edited on: 01/10/2016
 * Edited on: 02/10/2016
 * Edited on: 03/10/2016 - 6 months in!
 * Edited on: 12/03/2017
**/

#include <stdio.h>			/*I/O Library*/
#include <stdlib.h>			/*Standard (File Handling) Library*/
#include <string.h>			/*String Handling Library*/
#include <errno.h>			/*Error Handling Library*/

#define CLEAR() printf("\e[2J\e[H");	/*output keycode to clear UNIX Terminal*/
#define CLEARLN() printf("\e[2K\r");	/*Output keycode to clear one line in UNIX Terminal*/

int copyproc()				/*Displays copyright notice*/
{
	CLEAR();
	printf("╔══════════════════════════════════════════════════════════════════════════════╗\n");
	printf("║                                    Aviate                                    ║\n");
	printf("╚══════════════════════════════════════════════════════════════════════════════╝\n");
	printf("╔══════════════════════════════════════════════════════════════════════════════╗\n");
	printf("Aviate, created by James Phillips <james@jamesphillipsuk.com> on 3rd April 2016\n\nAviate is a terminal-based text-editor, designed for *Nix systems.\n\n");
	printf("This program is free software: you can redistribute it and/or modify\nit under the terms of the GNU General Public License as published by\nthe Free Software Foundation, either version 3 of the License, or\n(at your option) any later version.\n\n");
	printf("This program is distributed in the hope that it will be useful,\nbut WITHOUT ANY WARRANTY; without even the implied warranty of\nMERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\nGNU General Public License for more details.\n\n");
	printf("You should have received a copy of the GNU General Public License\nalong with this program.  If not, see <http://www.gnu.org/licenses/>.\n\n");
	printf("Press ENTER to agree to these terms, or CTRL+BACKLASH to quit.");
	printf("\033[24;0H╚══════════════════════════════════════════════════════════════════════════════╝");
	return 0;
}

int drawintroproc()				/*Draws design for the introductory screen*/
{
	int Cnt = 0; /*Counter variable for for.. loop below.*/

	CLEAR();
	printf("╔══════════════════════════════════════════════════════════════════════════════╗\n");
	printf("║                                    Aviate                                    ║\n");
	printf("╚══════════════════════════════════════════════════════════════════════════════╝\n");
	printf("╔══════════════════════════════════════════════════════════════════════════════╗\n");
	for (Cnt = 0;Cnt < 19; ++ Cnt)	/*Loop: I'm too lazy to draw an 80 char x20 char box*/
	{
		printf("║                                                                              ║\n");
	}
	printf("╚══════════════════════════════════════════════════════════════════════════════╝");
	printf("\033[5;2H Welcome to Aviate Public Alpha 2.2!\033[6;2H Aviate: the C-based text-editor designed for UNIX-like systems.\033[7;2H Designed and made by James Phillips.");
	return 0;
}

int drawmainproc() /*Draws the main page of the program*/
{
	int Cnt = 0;

	CLEAR();
	printf("\033[0;0H╔══════════════════════════════════════════════════════════════════════════════╗\n"); /*Draw box as in drawintroproc*/
	printf("║                                    Aviate                                    ║\n");
	printf("╚══════════════════════════════════════════════════════════════════════════════╝\n");
	printf("╔══════════════════════════════════════════════════════════════════════════════╗\n");
	for (Cnt = 0;Cnt < 19; ++ Cnt)
	{
		printf("                                                                                \n");
	}
	printf("╚══════════════════════════════════════════════════════════════════════════════╝");
	printf("\033[5;0H");
	return 0;
}

int saveinitproc(char Src[1024], char Dest[1024]) /*Initial save, This creates the file to reserve it for later*/
{
	const char Pwd[] = "./\0";	/*Holds "./", UNIX present working directory*/
	char Dir[] = "\0";		/*Will hold to user specified sub-directory and file name*/

	printf("\033[7;2H Please enter the name of your file");
	printf("\033[8;2H %s:", Pwd);
	scanf("%s", Dir);
	printf("\033[9;2H Saved to: %s%s", Pwd, Dir);

	/*Code below saves file*/
	strcpy(Src,  Dir);
	strcpy(Dest, Pwd);
	strcat(Dest, Src);

	FILE *f = fopen(Dest, "w");
	if (f == NULL)
	{
		printf("Error opening file: %s!\n", strerror(errno));
		exit(1); /*Kill with exit code one if file cannot be created.*/
	}
	fprintf(f,"");
	fclose(f);
	printf("\033[11;26H Press [ENTER] to continue. ");
	return 0;
}

int saveindevproc (char Dest[2048], char Buffer[]) /*Saves current buffer content to a .indev file*/
{
	char ReDest [2048] = "\0";

	strcpy (ReDest, Dest);
	strcat ( ReDest, ".indev");
	FILE *f = fopen(ReDest, "a");
	
	if (f == NULL)
	{
		printf("Error opening file: %s!\n", strerror(errno));
		exit(1);
	}
	fprintf(f, "%s", Buffer);

	fclose(f);
	return 0;
}

int savefinalproc (char Dest[2048]) /*Saves the final file when the editing is complete*/
{
	char ReDest [2048] = "\0";
	char CharCopy;
	int Remover;
	strcpy (ReDest, Dest);
	strcat ( ReDest, ".indev");
	FILE *c = fopen (ReDest, "r");
	FILE *f = fopen (Dest, "a");

	if (f == NULL)
	{
		printf("Error opening file: %s!\n", strerror(errno));
		exit(1);
	}
	if (c == NULL)
	{
		printf("Error opening file: %s!\n", strerror(errno));
		exit(1);
	}
	while (1==1) /*infinitely copy content from the .indev file to the final file*/
	{
		CharCopy = fgetc (c);
		if (CharCopy == EOF)
		{
			break;
		}
		else
		{
			fputc (CharCopy, f);
		}
	}
	fclose(c);
	fclose(f);
	
	Remover = remove(ReDest); /*Remove the .indev file, no longer needed for saving*/
	if(Remover == 0) 
	{
		printf(".indev file successfully removed");
	}
	else 
	{
		printf("Error: unable to remove .indev file");
	}

	printf ("Success!\n%s\n%s", Dest, ReDest);
	return 0;
}

int testupproc(char Buffer[], char Dest[2048],long int LineNo) /*Procedure to test for arrow key press*/
{
	int Cnt = 0;	/*Counter Variable for loop that reads through text input buffer*/
	int Cnt1 = 0;	/*Counter variable for sub-procedures*/
	char ReDest [2048] = "\0";	/*Overflow Buffer for file names*/
	char ReReDest [2048] = "\0";
	char CharRead;	/*Used in case A: to read characters within a file*/
	long int FileLineNo = 0;	/*Used in case A: for checking the line number of a file*/
	
	for (Cnt = 0; Cnt < 2049; Cnt ++)
	{
		if (Buffer[Cnt] == '\033' && Buffer[Cnt+1] == '[')
 		{
			switch(Buffer[Cnt+2])
			{
				case 'A':
				case 'D':
					/*Up Arrow Press*/
					Buffer[Cnt] = '\0'; /*Clear the current line*/
					Buffer[Cnt+1] = '\0';
					Buffer[Cnt+2] = '\0';					
					CLEARLN();
					printf("%s", Buffer); /*Printout the corrected buffer*/
					printf("\033[1A"); /*Go up to active line then clear it*/
					CLEARLN();
					printf("\033[1A"); /*Go up to previous line then clear it*/
					CLEARLN();
					
					strcpy (ReDest, Dest);
					strcat (ReDest, ".indev");
					FILE *f = fopen(ReDest, "r");
					strcpy (ReReDest, ReDest);
					strcat (ReReDest, ".tmp");
					FILE *c = fopen(ReReDest, "w");
					if (f == NULL || c == NULL)
					{
						printf("Error opening file: %s!\n", strerror(errno));
						exit(1);
					}
					while (FileLineNo < (LineNo - 1)) /*copy content from the .indev file to temporary file*/
					{
						CharRead = fgetc (f);
						if (CharRead == '\n')
						{
							FileLineNo = FileLineNo + 1;
						}
						else if (CharRead == EOF)
						{
							break; /*Break on end of file.*/
						}
						fputc (CharRead, c);
					}
					
					fclose(c);
					fclose(f);
					
					FILE *x = fopen(ReDest, "w+");
					FILE *y = fopen(ReReDest, "r");
					
					while (1==1) /*copy new content from temporary file to the .indev file*/
					{
						CharRead = fgetc (y);
						if (CharRead == EOF)
						{
							break; /*Emergency Break on end of file.  This shouldn't happen.*/
						}
						else
						{
							fputc (CharRead, x);
						}
					}
					
					fclose(y);
					fclose(x);
					
					for (Cnt1 = 0; Cnt1 < 2049; Cnt1++ ) /*Clear the buffer*/
					{
						Buffer[Cnt1] = '\0';
					}
					break;
				/*case 'D':
					printf("\033[1D");
					break;*/
				case 'C':
					/*Move to next line if on previous*/
					
					break;
				case 'B':
					/*Move forward one space if already back*/
					
					break;
				default:
					break;/*Break block if not an arrow key*/
			}
		testupproc(Buffer, Dest, LineNo); /*Revert to the start of the procedure*/
		}
	}
	return 0;
}

int textproc() /*Main input procedure for adding text to a file*/
{
	unsigned ConsecutiveEnterCount = 0;
	char Buffer [2048] = "\0";
	char Src[1024], Dest[1024];	/*Used to set destination for saving*/
	long int LineNo = 0;

	saveinitproc(Src, Dest);
	getchar();
	drawmainproc();
	for (;;)
	{
		
		memset(Buffer, '\0', strlen(Buffer));
		
		if (fgets(Buffer, sizeof(Buffer), stdin) == NULL) /*Fgets reads string stream into buffer*/
		{
			break;
		}
		if (Buffer[0] == '\n')
		{
			ConsecutiveEnterCount++;
			if (ConsecutiveEnterCount >= 1)
			{
				break;/*If enter is pressed twice, break the statement*/
			}
		}
		else
		{
			testupproc(Buffer, Dest, LineNo);
			LineNo = LineNo + 1;
			ConsecutiveEnterCount = 0; 
			saveindevproc(Dest, Buffer);
		}
	}
	savefinalproc(Dest); /*Save the file*/
	drawmainproc();
	printf ("\033[10;25HThank you for using MyProg!\n\033[11;27HPress [ENTER] to close.");/*Closing message*/
	return 0;
}

int main (void)
{
	copyproc();
	getchar();
	drawintroproc();
	textproc();
	getchar();
	CLEAR();
	return 0;
}
