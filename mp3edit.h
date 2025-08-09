/*
File        : mp3edit.h
Author      : G C Phaneendra
Roll No     : 25008_031
Description : Header file for MP3 Tag Editing functionality.

              This file contains the structure definition and function 
              prototypes required to modify ID3v2 tag data in MP3 files.

              Key Components:
               - MP3EDIT : Holds file pointers, tag name, new data content, 
                           and size information required for editing.
               - data[]  : Stores the new text that will replace existing tag content.

              Supported Editing Workflow:
               1. Parse command-line arguments to determine the tag to edit and new content.
               2. Validate the MP3 file extension.
               3. Open the MP3 file and create a temporary output file.
               4. Locate the specified ID3v2 frame (e.g., TIT2 for title).
               5. Replace old tag data with the new provided content.
               6. Save the modified MP3 file.

              Supported Tag Options:
               -t : Title
               -a : Artist
               -A : Album
               -y : Year
               -m : Genre
               -c : Comment

              Notes:
               - Only ".mp3" files are supported for editing.
               - Editing should preserve the rest of the MP3 file data unchanged.
               - This header works in conjunction with mp3view.h and mp3view.c 
                 to provide full tag viewing and editing capabilities.
*/

#ifndef mp3edit_h
#define mp3edit_h
#include "types.h"
#include "mp3view.h"

typedef struct mp3edit
{
    unsigned int size; // integer variable to store the new size of the content;
    char tag[5];  // an array of character to store tag;

    FILE *fptr_output_file;
     /*This is a file pointer to open file and perform file operations */

    char *input_file;
    FILE *fptr_input_file;

    char data[50]; // to store the new content;


}MP3EDIT;

//Function to edit content;
status mp3_edit(MP3EDIT *mp3edit, char *argv[]);

//Function to check the new file extension;
status check_for_newfileextension(MP3EDIT *mp3edit, char *argv[]);

//Function open the files to edit the data;
status open_files(MP3EDIT *mp3edit);

/* Function to edit tag data */
status edit_tag_data(MP3EDIT *mp3edit);

//Function to convert big endian to little endian;
int convert_to_littleEndian(const char *buffer);
#endif

