/*
File        : mp3view.c
Author      : G C Phaneendra
Roll No     : 25008_031
Description : Implementation file for MP3 Tag Viewing functionality.

              This file contains the definitions of functions required to:
               - Parse command-line arguments to determine operation type
               - Display help menu for viewing and editing MP3 metadata
               - Validate MP3 file extension
               - Open MP3 files for reading
               - Verify the presence of an ID3 tag and its version
               - Read tag frames (Title, Artist, Album, Year, Genre, Comment)
               - Convert frame size from big endian to little endian
               - Display retrieved tag data in a formatted way

              Workflow for Viewing:
               1. User provides the "-v" option with an MP3 filename.
               2. Program checks the file extension and opens the file.
               3. ID3 tag presence and version are validated.
               4. Tag frames are read and stored in the MP3VIEW structure.
               5. Collected tag information is displayed to the user.

              Supported ID3v2 Tags:
               - TIT2 : Title
               - TPE1 : Artist
               - TALB : Album
               - TYER : Year
               - TCON : Genre
               - COMM : Comment

              Notes:
               - Only ".mp3" files are supported.
               - Tag sizes are converted from big endian format as per ID3v2 spec.
               - This module is designed to work with the main MP3 Viewer/Editor.
*/

#include "types.h"
#include <stdio.h>
#include <string.h>
#include "mp3view.h"

char *tags[6] = {"TIT2", "TPE1", "TALB", "TYER", "TCON", "COMM"};

/*Function to check for command line arguments*/
OperationType check_Operation_Type(int argc, char *argv[])
{
      if((argc >= 2) && argv[1] != NULL)
      {
	    if(strcmp(argv[1], "-v") == 0)
         {
	       return view_mp3tags;
         }
         else if(strcmp(argv[1], "-e") == 0)
         {
	       return edit_mp3tags;
         }
         else if(strcmp(argv[1], "--help") == 0)
         {
	        return Help_menu;
          }
          else
         {
	      return unsupported;
         }

      }
      else
      {
           return  unsupported;

      }
}

//Function to display help Info
void Help_Info()
{
	printf("----------------------------------------HELP MENU-----------------------------------------------\n");
	printf("1. -v -> to view mp3 file contents\n");
	printf("2. -e -> to edit mp3 file contents\n");
	printf("2.1. -t -> to edit song title\n");
	printf("2.2. -a -> to edit artist name\n");
	printf("2.3. -A -> to edit album name\n");
	printf("2.4. -y -> to edit year\n");
	printf("2.5. -m -> to edit content\n");
	printf("2.1. -c -> to edit comment\n");
      printf("-------------------------------------------------------------------------------------------------\n");
}

// Function to check for the file extension;
status check_for_extension(char *argv[], MP3VIEW *mp3view)
{
      char *temp = strrchr(argv[2], '.');
      if(temp != NULL && strcmp(temp, ".mp3") == 0)
      {
           mp3view -> sample_mp3_fname = argv[2]; 
           return E_SUCCESS;
      }
      else
      {
           return E_FAILURE;
      }
           
}

status Mp3View(MP3VIEW *mp3view)
{
      if(open_mp3file(mp3view) != E_SUCCESS)
      {
            return E_FAILURE;
      }

      if(check_for_ID3(mp3view) != E_SUCCESS)
      {
            return E_FAILURE;
      }

      if(check_for_version(mp3view) != E_SUCCESS)
      {
            return E_FAILURE;
      }

      if(read_tag_info(mp3view) != E_SUCCESS)
      {
              return E_FAILURE;
      }
      display_mp3tags(mp3view);
      return E_SUCCESS;
}

status open_mp3file(MP3VIEW *mp3view)
{
      mp3view -> fptr_sample_mp3 = fopen(mp3view -> sample_mp3_fname, "r");

      if(mp3view -> fptr_sample_mp3 == NULL)
      {
              perror("Error: Unable to open The file");
              return E_FAILURE;
      }
      else
      {
            printf("The file was opened successfully\n");
            return E_SUCCESS;
      }

}

status check_for_ID3(MP3VIEW *mp3view)
{
      
      char temp_buffer[4];
      if(fread(temp_buffer, sizeof(char), 3, mp3view -> fptr_sample_mp3) == 3)
      {
            temp_buffer[3] = '\0';
            //printf("%s\n",temp_buffer);
            if(strcmp(temp_buffer, "ID3") == 0)
            {
                  return E_SUCCESS;
            }
            else
            {
                  printf("ID3 was not found in the file\n");
                  return E_FAILURE;
            }
      } 
      else
      {
             printf("fread function failed to read the data from a file stream\n");
             return E_FAILURE;
      }

}

//Function to check for version;
status check_for_version(MP3VIEW *mp3view)
{
      char version[2];
      //tempary buffer to store the version;
      //if fread function was successfull then it will returns number of elements successfully read from the file;
      if(fread(version, sizeof(char), 2, mp3view ->fptr_sample_mp3) == 2)
      {
            
             if((unsigned char)version[0] == 3 && (unsigned char)version[1] == 0)
             {
                  return E_SUCCESS;
             }
             else
             {
                  printf("the correct version was not found\n");
                  return E_FAILURE;

             }
      }
      else
      {
            printf("fread function failed to read the data from a file stream\n");
             return E_FAILURE;
      }
}
//Function to read all tags and titles related to tags;
status read_tag_info(MP3VIEW *mp3view)
{

   fseek(mp3view -> fptr_sample_mp3, 5, SEEK_CUR);
   char size_buffer[4];
   char *data;
   char tag[5];
   int i;
for(i = 0; i < 6; i++)
{
      if(fread(tag, 1, 4, mp3view -> fptr_sample_mp3) != 4)
      {
            return E_FAILURE;
      }
      tag[4] = '\0';
      if(strcmp(tags[i], tag) == 0)
      {
            strcpy(mp3view -> mp3viewinfo[i].tags, tag);
           
      }
      else
      {
            printf("The tag not found");
            break;
      }

      if(fread(size_buffer, 1, 4, mp3view -> fptr_sample_mp3) != 4)
      {
            return E_FAILURE;
      }
      mp3view -> mp3viewinfo[i].size = bigendian_to_littleendian(size_buffer);
      fseek(mp3view -> fptr_sample_mp3, 2, SEEK_CUR);
      unsigned char encoding;
      fread(&encoding, 1, 1, mp3view -> fptr_sample_mp3);
       // actual text size is equal to frame size - 1 byte encoding;
       size_t text_data = mp3view -> mp3viewinfo[i].size - 1;

      if(fread(mp3view -> mp3viewinfo[i].data, text_data, 1, mp3view -> fptr_sample_mp3) != 1)
      {
            return E_FAILURE;
      }
      mp3view -> mp3viewinfo[i].data[text_data] = '\0';
     
}
      return E_SUCCESS;

}


//Function to convert big endian to little endian;
unsigned int bigendian_to_littleendian(const unsigned char *ptr)
{
    return (ptr[0] & 0x7F) << 21 |
           (ptr[1] & 0x7F) << 14 |
           (ptr[2] & 0x7F) << 7  |
           (ptr[3] & 0x7F);
}

void display_mp3tags(MP3VIEW *mp3view)
{
  printf("------------------------------------------SELECTED VIEW DETAILS-----------------------------------\n");
  
  printf("----------------------------------------------------------------------------------\n");
  printf("==================MP3 TAG READER AND EDITOR FOR ID3V2===========================\n");
  printf("----------------------------------------------------------------------------------\n");
  printf("TITLE          :            %s\n",  mp3view -> mp3viewinfo[0].data);
  printf("ARTIST         :            %s\n",  mp3view -> mp3viewinfo[1].data);
  printf("ALBUM          :            %s\n",  mp3view -> mp3viewinfo[2].data);
  printf("YEAR           :            %s\n",  mp3view -> mp3viewinfo[3].data);
  printf("MUSIC          :            %s\n",  mp3view -> mp3viewinfo[4].data);
  printf("COMMENT        :            %s\n",  mp3view -> mp3viewinfo[5].data);
  printf("----------------------------------------------------------------------------------\n");
}


