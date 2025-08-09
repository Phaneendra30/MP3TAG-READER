/*
File        : mp3edit.c
Author      : G C Phaneendra
Roll No     : 25008_031
Description : Implementation file for MP3 Tag Editing functionality.

              This file contains the function definitions required to
              modify ID3v2 metadata tags in MP3 files. It works by scanning
              the MP3's tag frames, locating the desired frame, replacing its
              data with user-provided content, and writing the modified data
              to a new file.

              Features:
               - Parse command-line arguments to identify the tag to edit
               - Validate new MP3 file extension before editing
               - Open input and output MP3 files in binary mode
               - Locate and update the selected ID3v2 frame (e.g., TIT2, TPE1)
               - Preserve all other frames and file content without changes
               - Replace the original file with the updated one after editing

              Supported Tag Edit Options:
               -t : Title    (TIT2)
               -a : Artist   (TPE1)
               -A : Album    (TALB)
               -y : Year     (TYER)
               -m : Genre    (TCON)
               -c : Comment  (COMM)

              Editing Workflow:
               1. User runs the program with -e and a tag option;
 */
#include <stdio.h>
#include "types.h"
#include "mp3edit.h"
#include <string.h>
#include "mp3view.h"
#include <stdlib.h>

char *edit_tag[6] = {"-t", "-a", "-A", "-y", "-m", "-c"};

//Function to edit content;
status mp3_edit(MP3EDIT *mp3edit, char *argv[])
{
    unsigned short int i;
    unsigned short int found = 0;
    for(i = 0; i < 6; i++)
    {
          if(strcmp(argv[2], edit_tag[i]) == 0)
         {
              found = 1;
               switch (argv[2][1])
              {
                      case 't': strcpy(mp3edit -> tag, "TIT2"); break;
                      case 'a': strcpy(mp3edit -> tag, "TPE1"); break;
                      case 'A': strcpy(mp3edit -> tag, "TALB"); break;
                      case 'y': strcpy(mp3edit -> tag, "TYER"); break;
                      case 'm': strcpy(mp3edit -> tag, "TCON"); break;
                      case 'c': strcpy(mp3edit -> tag, "COMM"); break;
                     default:
                      printf("Error: Unsupported tag option '%s'\n", argv[2]);
                     return E_FAILURE;
               }
          
              if(argv[3] == NULL)
              {
               printf("Error missing new content for editing\n");
               return E_FAILURE;
              }
              size_t len = strlen(argv[3]);
              strcpy(mp3edit->data, argv[3]);
              mp3edit->data[len] = '\0';
              mp3edit -> size = len;
               if(check_for_newfileextension(mp3edit, argv) != E_SUCCESS)
               {
                return E_FAILURE;
               }
               return E_SUCCESS;
          }
     }

    if(!found)
    {
      printf("we'r here to help you every step of the way\n");
           printf("-t -> to edit song title\n");
           printf("-a to edit artist name\n");
           printf("-A to edit album name\n");
           printf("-y to edit year\n");
           printf("-m to edit content\n");
           printf("-c to edit comment\n");
           return E_FAILURE;
    }
}

//Function to check the new file extension;
 status check_for_newfileextension(MP3EDIT *mp3edit, char *argv[])
 {
     if(argv[4] == NULL)
     {
        printf("Error: missing output file name\n");
        return E_FAILURE;
     }
      char *temp = strrchr(argv[4], '.');
      if(temp != NULL && strcmp(temp, ".mp3") == 0)
      {
           mp3edit -> input_file = argv[4]; 
           return E_SUCCESS;
      }
      else
      {
           printf("Nem file extension should be .mp3\n");
           return E_FAILURE;
      }

  }


  /* Function to open input and output files */
    status open_files(MP3EDIT *mp3edit)
   {
      mp3edit -> fptr_input_file = fopen(mp3edit -> input_file, "rb");

      if(mp3edit -> fptr_input_file  == NULL)
      {
              perror("Error: Unable to open The input file");
              return E_FAILURE;
      }
      else
      {
            printf("The input file was opened successfully\n");
      }

      mp3edit -> fptr_output_file =  fopen("output.mp3", "wb");
      if(mp3edit -> fptr_output_file   == NULL)
      {
         perror("Error opening output file");
         return E_FAILURE;
      }
      else
      {
          printf("The output file was opened successfully\n");
      }
     return E_SUCCESS;
      
  }



/* Function to edit tag data */
status edit_tag_data(MP3EDIT *mp3edit)
{
    // Copy ID3v2 header
    rewind(mp3edit -> fptr_input_file);
    char header[10];
    fread(header, 1, 10,  mp3edit -> fptr_input_file);
    fwrite(header, 1, 10,  mp3edit -> fptr_output_file);
    int edited = 0;
    while (1)
{
    char frame_id[5] = {0};
    unsigned char size_buf[4], flags[2];
    unsigned char encoding;
    
    // Frame ID
    if (fread(frame_id, 1, 4, mp3edit->fptr_input_file) != 4 || frame_id[0] == 0x00)
        break;
    
    // Frame size
    if (fread(size_buf, 1, 4, mp3edit->fptr_input_file) != 4)
        break;
    
    int frame_size = convert_to_littleEndian((char *)size_buf);
    
    // Frame flags
    if (fread(flags, 1, 2, mp3edit->fptr_input_file) != 2)
        break;
    
    // Encoding byte
    if (fread(&encoding, 1, 1, mp3edit->fptr_input_file) != 1)
        break;

    // Remaining frame data
    char *data = malloc(frame_size - 1);
    if (!data) return E_FAILURE;
    if (fread(data, 1, frame_size - 1, mp3edit->fptr_input_file) != frame_size - 1)
    {
        free(data);
        return E_FAILURE;
    }

    if (strncmp(frame_id, mp3edit->tag, 4) == 0)
    {
        int new_size = strlen(mp3edit->data) + 1;
        unsigned char new_size_buf[4] = {
            (new_size >> 24) & 0xFF,
            (new_size >> 16) & 0xFF,
            (new_size >> 8) & 0xFF,
            new_size & 0xFF
        };
        fwrite(frame_id, 1, 4, mp3edit->fptr_output_file);
        fwrite(new_size_buf, 1, 4, mp3edit->fptr_output_file);
        fwrite(flags, 1, 2, mp3edit->fptr_output_file);
        fwrite(&encoding, 1, 1, mp3edit->fptr_output_file);
        fwrite(mp3edit->data, 1, new_size - 1, mp3edit->fptr_output_file);
        edited = 1;
    }
    else
    {
        // Copy original frame exactly
        fwrite(frame_id, 1, 4, mp3edit->fptr_output_file);
        fwrite(size_buf, 1, 4, mp3edit->fptr_output_file);
        fwrite(flags, 1, 2, mp3edit->fptr_output_file);
        fwrite(&encoding, 1, 1, mp3edit->fptr_output_file);
        fwrite(data, 1, frame_size - 1, mp3edit->fptr_output_file);
    }

    free(data);
}


    // copy remaining data to output file;
    char buffer[4096];
    size_t bytes;
    while ((bytes = fread(buffer, 1, sizeof(buffer), mp3edit->fptr_input_file)) > 0)
    {
       fwrite(buffer, 1, bytes, mp3edit->fptr_output_file);
    }

    fclose(mp3edit -> fptr_input_file);
    fclose(mp3edit -> fptr_output_file);

    if (edited)
    {
        remove(mp3edit -> input_file);
        rename("output.mp3", mp3edit -> input_file);
        
     printf("✅ Tag '%s' updated successfully.\n", mp3edit -> tag);
    }
    else
    {
        remove("output.mp3");
        printf("⚠  Frame '%s' not found. No changes made.\n", mp3edit -> tag);
    }

    return  E_SUCCESS;
}

          
        

         
    

    
    


//Function to convert big endian to little endian;
int convert_to_littleEndian(const char *buffer)
{
    return ((unsigned char)buffer[0] << 24) |
           ((unsigned char)buffer[1] << 16) |
           ((unsigned char)buffer[2] << 8) |
           (unsigned char)buffer[3];
}
