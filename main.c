/*
*MyProg, created by James Phillips <james@jamesphillipsuk.com> on 3rd March 2016
*
*MyProg is a terminal application text-editor, designed to be used on UNIX-like systems.
*Copyright (C) 2016 James Phillips.
*
*Created on: 03/04/2016
*Edited on: 03/04/2016
*Edited on: 04/04/2016
*Edited on: 05/04/2016
*Edited on: 06/04/2016
*Edited on: 09/04/2016
*Edited on: 10/04/2016
*/

#include <stdio.h>			/*I/O Library*/
#include <stdlib.h>			/*Standard (File Handling) Library*/
#include <string.h>			/*String Handling Library*/
#include <limits.h>

#define CLEAR() printf("\e[2J\e[H");	/*output keycode to clear UNIX Terminal*/

int copyproc()				/*Displays copyright notice*/
{
	CLEAR();
	printf("MyProg, created by James Phillips <james@jamesphillipsuk.com> on 3rd March 2016\n\nMyProg is a terminal application text-editor, designed to be used on UNIX-like systems.\n\n");
	printf("This program is free software: you can redistribute it and/or modify\nit under the terms of the GNU General Public License as published by\nthe Free Software Foundation, either version 3 of the License, or\n(at your option) any later version.\n\n");
	printf("This program is distributed in the hope that it will be useful,\nbut WITHOUT ANY WARRANTY; without even the implied warranty of\nMERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\nGNU General Public License for more details.\n\n");
	printf("You should have received a copy of the GNU General Public License\nalong with this program.  If not, see <http://www.gnu.org/licenses/>.\n\n");
	printf("Press ENTER to agree to these terms, or CTRL+BACKLASH to quit.");
}

int drawproc1()				/*Draws design for screen one*/
{
	int cnt = 0;

	CLEAR();
	printf("╔══════════════════════════════════════════════════════════════════════════════╗\n");
	printf("║                                    MyProg                                    ║\n");
	printf("╚══════════════════════════════════════════════════════════════════════════════╝\n");
	printf("╔══════════════════════════════════════════════════════════════════════════════╗\n");
	for (cnt = 0;cnt < 19; ++ cnt)	/*Loop: I'm too lazy to draw an 80 char x20 char box*/
	{
		printf("║                                                                              ║\n");
	}
	printf("╚══════════════════════════════════════════════════════════════════════════════╝");
	printf("\033[5;2H Welcome to MyProg: the C-based text-editor designed for UNIX-like systems.\033[6;2H Designed and made by James Phillips.");
	return 0;
}

int drawproc2()
{
	int cnt = 0;

	CLEAR();
	printf("\033[0;0H╔══════════════════════════════════════════════════════════════════════════════╗\n");
	printf("║                                    MyProg                                    ║\n");
	printf("╚══════════════════════════════════════════════════════════════════════════════╝\n");
	printf("╔══════════════════════════════════════════════════════════════════════════════╗\n");
	for (cnt = 0;cnt < 19; ++ cnt)
	{
		printf("                                                                                \n");
	}
	printf("╚══════════════════════════════════════════════════════════════════════════════╝");
	printf("\033[5;0H");
	return 0;
}

int saveproc1(char Src[1024], char Dest[1024])
{
	int cnt = 0;
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
		printf("Error opening file!\n");
		exit(1);
	}

	/* print some text */
	char text[] = "You have saved a blank file.  Content has been created for you.";
	fprintf(f, "%s", text);

	fclose(f);
	/**/
	printf("\033[11;26H Press [ENTER] to continue. ");
	return 0;
}

int saveproc2 (char Dest[2048], char Buffer[])
{
	char ReDest [2048] = "\0";

	strcpy (ReDest, Dest);
	strcat ( ReDest, ".indev");

	FILE *f = fopen(ReDest, "a");
	if (f == NULL)
	{
		printf("Error opening file!\n");
		exit(1);
	}
	fprintf(f, "%s", Buffer);

	fclose(f);
	return 0;
}

int saveproc3 (char Dest[2048])
{
	char ReDest [2048] = "\0";
	char CharCopy;

	strcpy (ReDest, Dest);
	strcat ( ReDest, ".indev");

	FILE *c = fopen (ReDest, "r");
	FILE *f = fopen (Dest, "a");
	if (f == NULL)
	{
		printf("Error opening file! %s\n", Dest);
		exit(1);
	}
	if (c == NULL)
	{
		printf("Error opening file! %s\n", ReDest);
		exit(1);
	}
	while (1==1)
	{
		CharCopy = fgetc (c);
		if (CharCopy == EOF)
		{
			break;	
		}
		else
		{
			putc (CharCopy, f);
		}
	}
	printf ("Success!\n%s\n%s", Dest, ReDest);
	fclose(c);
	fclose(f);
	return 0;
}

int textproc()
{
	unsigned ConsecutiveEnterCount = 0;
	char Buffer [2048] = "\0";
	char Src[1024], Dest[1024];	/*Used to set destination for saving*/

	saveproc1(Src, Dest);
	getchar();
	drawproc2();
	for (;;)
	{
		memset(Buffer, '\0', strlen(Buffer));
		if (fgets(Buffer, sizeof(Buffer), stdin) == NULL)
		{
			break;
		}
		if (Buffer[0] == '\n')
		{
			ConsecutiveEnterCount++;
			if (ConsecutiveEnterCount >= 2)
			{
				break;
			}
		}
		else
		{
			ConsecutiveEnterCount = 0;
			saveproc2(Dest, Buffer);
		}
	}
	saveproc3(Dest);
	drawproc1();
	printf ("\033[10;25HThank you for using MyProg!\n\033[11;27HPress [ENTER] to close.");
	return 0;
}

void main (void)
{
	copyproc();
	getchar();
	drawproc1();
	textproc();
	getchar();
	CLEAR();
}
