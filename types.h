/*
File        : types.h
Author      : G C Phaneendra
Roll No     : 25008A_031
Description : Common type definitions and enumerations used across
              the MP3 Tag Editor project.

              This header defines:
               1. status (E_SUCCESS, E_FAILURE)
                  - Used as return values for indicating success or failure
                    of operations.

               2. OperationType
                  - Enumerates the types of operations the program can perform:
                      view_mp3tags  : View existing ID3v2 tags in the MP3 file
                      edit_mp3tags  : Edit specific ID3v2 tags in the MP3 file
                      Help_menu     : Display usage/help instructions
                      unsupported   : Invalid or unrecognized command

              This file acts as a central point for status and operation
              type constants, ensuring consistent use across multiple
              source files.
*/
#ifndef TYPES_H
#define TYPES_H

typedef enum
{
	E_SUCCESS,
	E_FAILURE
}status;

typedef enum
{
	view_mp3tags,
	edit_mp3tags,
	Help_menu,
	unsupported
} OperationType;

#endif

 

