/* ext2_test.c
   This file contains test cases designed to ensure that the ext2.c implementation 
   correctly performs tasks related to file management, inode handling, chmode, and other 
   functionalities defined in the ext2 specification.
 * Author: Chananya Templeman
 * Reviewed by: Elisheva
 * Date: 22/10/2024*/

/* ext2_test.c - test file */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ext2.h"

int main(int argc, char *argv[])
{
    const char *disk_name = NULL;
    const char *file_path = NULL;
    const char *new_mode = NULL;
    FS_t* fs = NULL;
    long inode_num = 0;

    /*Checks command line arguments*/
    if (argc != 3 && argc != 4)
    {
        fprintf(stderr, "Usage: %s <disk_image> <file_path> [new_mode]\n", argv[0]);
        fprintf(stderr, "new_mode format: rwxrwxrwx (use - for no permission)\n");
        exit(EXIT_FAILURE);
    }

    disk_name = argv[1];
    file_path = argv[2];

    printf("Opening disk: %s\n", disk_name);
    fs = FSOpen(disk_name);  /*Opens the file system*/

    if (NULL == fs) 
    {
        fprintf(stderr, "Failed to open %s\n", disk_name);
        return EXIT_FAILURE;
    }

    printf("Getting inode for file: %s\n", file_path);
    inode_num = GetInode(fs, file_path);  /*Gets the inode number for the specified file*/

    if (-1 == inode_num) 
    {
        fprintf(stderr, "Failed to find inode for %s\n", file_path);
        FSClose(fs);
        return EXIT_FAILURE;
    }

    printf("Inode number for %s: %ld\n", file_path, inode_num);

    if (argc == 4)  /*The user adds the string of the new mode in the CLI*/
    {
        new_mode = argv[3];
        printf("Changing mode to: %s\n", new_mode);
        
        if (-1 == Chmode(fs, inode_num, (char *)new_mode))
        {
            fprintf(stderr, "Failed to change mode for %s\n", file_path);
            FSClose(fs);
            return EXIT_FAILURE;
        }
        printf("Successfully changed mode\n");
    }
    else
    {
        printf("Printing contents of %s:\n", file_path);
        PrintFile(fs, inode_num);
    }

    FSClose(fs);
    return EXIT_SUCCESS;
}