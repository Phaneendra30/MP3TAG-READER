/*
Name        : G C Phaneendra
Roll No     : 25008_031
Description : Main driver program for the MP3 Tag Viewer and Editor.

              This program allows the user to either view or edit ID3v2 tags
              of MP3 audio files from the command line. It supports reading
              metadata such as Title, Artist, Album, Year, Genre, and Composer,
              as well as modifying these fields with new values.

              Functionalities:
               - View existing MP3 tag information (-v option)
               - Edit specific tags using tag options (-e option)
               - Display help information (--help option)

              Supported tag edit options:
               -t : Title
               -a : Artist
               -A : Album
               -y : Year
               -m : Genre
               -c : Composer

              Usage:
                To view : ./a.out -v <mp3filename>
                To edit : ./a.out -e -t/-a/-A/-y/-m/-c "new_text" <mp3filename>
                Help    : ./a.out --help

              Notes:
               - Only files with the ".mp3" extension are supported.
               - Editing requires a valid MP3 file and proper tag option.
               - Invalid arguments or missing parameters will display usage help.
*/
#include <stdio.h>
#include "mp3view.h"
#include "mp3edit.h"
#include "types.h"

int main(int argc, char *argv[])
{
    MP3VIEW mp3view = {0};
    MP3EDIT mp3edit = {0};

    OperationType operation = check_Operation_Type(argc, argv);

    if(operation == unsupported)
    {
        printf("ERROR: ./a.out : INVALID ARGUMENTS\n");
        printf("USAGE : \n");
        printf("To view : ./a.out -v mp3filename\n");
        printf("To edit : ./a.out -e -t/-a/-A/-m/-y/-c change_text mp3filename\n");
        printf("Help    : ./a.out --help\n");
    }
    else if(operation == Help_menu)
    {
        Help_Info();
    }
    else if(operation == view_mp3tags)
    {
        if(argc < 3)
        {
            printf("Error: Missing mp3 filename\n");
            return 1;
        }
        if(check_for_extension(argv, &mp3view) == E_SUCCESS)
        {
            Mp3View(&mp3view);
        }
        else
        {
            printf("Error: mp3 file extension should be .mp3\n");
        }
    }
    else if(operation == edit_mp3tags)
    {
	      if(argc < 3)
          {
             printf("Error: missing tag option Use -t, -a, -A, -y, -m, -c\n");
             return 1;

          }
		    
             if(mp3_edit(&mp3edit, argv) != E_SUCCESS)// rename function for clarity
             return 1;
             if(open_files(&mp3edit) != E_SUCCESS)
             return 1;

             edit_tag_data(&mp3edit);
    }
    else
    {
        printf("Invalid operation type\n");
        Help_Info();
    }

    return 0;
}


