
/*
File        : mp3view.h
Author      : G C Phaneendra
Roll No     : 25008_031
Description : Header file for MP3 Tag Viewing functionality.

              This file contains the structure definitions, constants, 
              and function prototypes required to read and display 
              ID3v2 tags from an MP3 file.

              Key Components:
               - MP3VIEWINFO : Holds individual tag name, size, and value.
               - MP3VIEW     : Holds the MP3 filename, file pointer, and 
                               an array of MP3VIEWINFO structures for all tags.
               - Constants   : MAX_LEN (tag name length), MAX_TAGS (number of tags).

              Supported Operations:
               - Checking command-line operation type
               - Displaying help information
               - Validating MP3 file extension
               - Opening MP3 file and checking ID3 header
               - Determining ID3 version
               - Reading all tag data and converting endian format
               - Displaying all retrieved tags to the user

              Notes:
               - Only files with the ".mp3" extension are supported.
               - This header is designed for use with the main MP3
                 Viewer/Editor program.
*/
#ifndef mp3view_h
#define mp3view_h
#include "types.h"
#define MAX_LEN 5
#define MAX_TAGS 6

typedef struct ViewInfo
{
	char tags[MAX_LEN]; //  an array of character to store different tags; 
	unsigned int size;           // integer variable to store the size of the tag data;
	char data[100]; // character array to store information about tags;
}MP3VIEWINFO;

typedef struct mp3view
{
	char *sample_mp3_fname; //this is a character pointer to store the base address of sample file name; 
	FILE *fptr_sample_mp3;   // this is a file pointer to open the file and perform file operations
    MP3VIEWINFO mp3viewinfo[MAX_TAGS];
}MP3VIEW;

// Function to check operationtype 
OperationType check_Operation_Type(int argc, char *argv[]);

//Function to display HELP MENU
void Help_Info();

//Function to check file Extension;
status check_for_extension(char *argv[], MP3VIEW *mp3view);

//Function for mp3view;
status Mp3View(MP3VIEW *mp3view);

//Function to open mp3  file;
status open_mp3file(MP3VIEW *mp3view);

//Function to check for ID3;
status check_for_ID3(MP3VIEW *mp3view);

//Function to check for version;
status check_for_version(MP3VIEW *mp3view);

//Function to read all tags and titles related to tags;
status read_tag_info(MP3VIEW *mp3view);

//Function to convert big endiann to little endian;
unsigned int bigendian_to_littleendian(const unsigned char *ptr);

//Function to display mp3 view tags
void display_mp3tags(MP3VIEW *mp3view);













#endif
