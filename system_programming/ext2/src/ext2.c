/* ext2.c
   This file implements a command-line tool to interact with the ext2 filesystem.
   It allows users to open a disk image, retrieve the inode number for a specified file,
   and print the contents of that file.
 * Author: Chananya Templeman
 * Reviewed by: Elisheva
 * Date: 22/10/2024*/

#define _POSIX_C_SOURCE 200809L  /*strdup*/
#include <stdio.h>  /*printf, fopen etc.*/
#include <stdlib.h>  /*malloc*/
#include <string.h>  /*strncpy*/
#include <fcntl.h>  /*file control options: open(), O_RDONLY*/
#include <unistd.h>  /*read(), lseek() etc.*/
#include <stdint.h>  /*uint32_t etc.*/
#include <errno.h>  /*perror()*/
#include "ext2.h"

/*Magic number for ext2 filesystem*/
#define EXT2_SUPER_MAGIC 0xEF53
/*Root inode number*/
#define EXT2_ROOT_INO 2
/*Maximum length of a filename*/
#define EXT2_NAME_LEN 255

#define EXT2_SUPERBLOCK_OFFSET 1024
#define EXT2_NUM_DIRECT_BLOCKS 12
#define EXT2_LAST_DIRECT_BLOCK_INDEX 11

/*Superblock structure of the ext2 filesystem (as defined in Harvard.edu). 
  All elements will be populated upon reading the superblock from the ext2 filesystem. 
  Note: Many elements are included to reflect the ext2 filesystem structure, but not all are necessary for basic ext2 parsing functionality.*/
struct ext2_super_block 
{
    uint32_t s_inodes_count;  
    uint32_t s_blocks_count;  
    uint32_t s_r_blocks_count;  /*Number of reserved blocks for the superuser*/
    uint32_t s_free_blocks_count;  
    uint32_t s_free_inodes_count;  
    uint32_t s_first_data_block;  /*Block number of the first data block.*/
    uint32_t s_log_block_size;  /*Used to calculate the block size. It's the exponent to which 2 must be raised to get the actual block size.*/
    uint32_t s_log_frag_size;  /*Used to calculate the fragment size (allows a block to be broken down into smaller pieces to save space for small files).*/
    uint32_t s_blocks_per_group;  
    uint32_t s_frags_per_group;
    uint32_t s_inodes_per_group;
    uint32_t s_mtime;  /*Time of the last mount.*/
    uint32_t s_wtime;  /*Time of the last write*/
    uint16_t s_mnt_count;
    uint16_t s_max_mnt_count;
    uint16_t s_magic;  /*Magic number to identify the filesystem (0xEF53 for ext2).*/
    uint16_t s_state;  /*Filesystem state (clean, errors, etc.).*/
    uint16_t s_errors;  /*What to do when errors are detected (continue, panic, etc.).*/
    uint16_t s_minor_rev_level;
    uint32_t s_lastcheck;  /*Time of the last filesystem check.*/
    uint32_t s_checkinterval;  /*Maximum time between filesystem checks.*/
    uint32_t s_creator_os;  /*ID of the OS that created the filesystem.*/
    uint32_t s_rev_level;
    uint16_t s_def_resuid;  /*Default user ID for reserved blocks.*/
    uint16_t s_def_resgid;  /*Default group ID for reserved blocks.*/
    uint32_t s_first_ino;  /*First non-reserved inode (typically inode 11).*/
    uint16_t s_inode_size;
    uint16_t s_block_group_nr;  /*Block group number hosting this superblock.*/
    uint32_t s_feature_compat;  /*Compatible feature set.*/
    uint32_t s_feature_incompat;
    uint32_t s_feature_ro_compat;  /*Read-only compatible feature set.*/
    uint8_t  s_uuid[16];  /*Universally unique identifier for each filesystem instance (16 bytes).*/
    char     s_volume_name[16];  /*A human-readable identifier for the filesystem.*/
    char     s_last_mounted[64];  /*Directory path where the filesystem was last mounted.*/
    uint32_t s_algorithm_usage_bitmap;  /*For compression usage.*/
    uint8_t  s_prealloc_blocks;  /*The number of blocks that the filesystem preallocates upon the creation of a file (8 by default).*/
    uint8_t  s_prealloc_dir_blocks;
    uint16_t s_reserved_gdt_blocks;  /*Number of reserved blocks set aside for the group descriptor table.*/
    uint8_t  s_journal_uuid[16];  /*The next 4 elements are for journalong. And even though ext2 itself does not utilize it, these fields remain in the superblock structure for future compatibility.*/
    uint32_t s_journal_inum;
    uint32_t s_journal_dev;
    uint32_t s_last_orphan;  /*Those that have been deleted or are no longer connected to any directory entry but still have blocks allocated to them.*/
    uint32_t s_hash_seed[4];  /*Used as a seed for the hash function that directories use to store and retrieve directory entries. O(1) time. (16 bytes)*/
    uint8_t  s_def_hash_version;  /*The default version of the hash function.*/
    uint8_t  s_reserved_char_pad;  /*A 1 byte padding that ensures that the next member (s_reserved_word_pad) starts at a 2-byte boundary.*/
    uint16_t s_reserved_word_pad;  /*This 2 byte field ensures that the following field (s_default_mount_opts, a uint32_t) starts at a 4-byte aligned address.*/
    uint32_t s_default_mount_opts;
    uint32_t s_first_meta_bg;  /*Indicates the first block group that holds metadata blocks.*/
    uint32_t s_reserved[190];  /*Array of reserved space intended for future use by the filesystem.*/
};

/*Group descriptor structure*/
struct ext2_group_desc 
{
    uint32_t bg_block_bitmap;  /*Block number of the block bitmap for the block group.*/
    uint32_t bg_inode_bitmap;
    uint32_t bg_inode_table;
    uint16_t bg_free_blocks_count;
    uint16_t bg_free_inodes_count;
    uint16_t bg_used_dirs_count;
    uint16_t bg_pad;  /*Padding for proper alignment of the struct.*/
    uint32_t bg_reserved[3];  /*Reserved fields are included for future use.*/ 
};

/*Inode structure*/
struct ext2_inode 
{
    uint16_t i_mode;  /*File type and access permissions.*/
    uint16_t i_uid;  /*User ID of the file's owner.*/
    uint32_t i_size;  
    uint32_t i_atime;  /*Last access time.*/
    uint32_t i_ctime;  /*Last time the inode's metadata (like permissions) was changed.*/
    uint32_t i_mtime;  /*Last time the file's content was modified.*/
    uint32_t i_dtime;  /*Indicates when the file was deleted,(if not then it's set to 0).*/ 
    uint16_t i_gid;  /*Group ID of the file's group owner.*/
    uint16_t i_links_count;  /*Number of hard links to the file.*/
    uint32_t i_blocks;  /*Number of blocks allocated to the file, (including indirect blocks).*/
    uint32_t i_flags;  /*Additional attributes for the file (like whether it’s a hidden file).*/
    uint32_t i_osd1;  /*OS specific data*/
    uint32_t i_block[15];  /*This array contains the block numbers of the first 12 direct blocks, one single indirect block (13th entry), one double indirect block (14th entry), and one triple indirect block (15th entry).*/
    uint32_t i_generation;  /*An integer that is incremented each time the inode is changed in a way that affects its contents or attributes.*/
    uint32_t i_file_acl;  /*Access control list for the file.*/
    uint32_t i_dir_acl;  /*ACL for directories.*/
    uint32_t i_faddr;  /*Fragment address (unused in ext2). Reserved for future use.*/
    uint8_t  i_osd2[12];  /*Additional operating system-specific data.*/
};

/*Directory entry structure*/
struct ext2_dir_entry 
{
    uint32_t inode;  /*The inode number of the file or directory associated with this directory entry.*/
    uint16_t rec_len;  /*Record length - total length of this directory entry in bytes. Helps the parser know how many bytes to skip to get to the next entry in the directory.*/
    uint8_t  name_len;
    uint8_t  file_type;
    char     name[EXT2_NAME_LEN];
};

/*File system structure*/
struct FS 
{
    int fd;  /*File descriptor for the opened file system, used to perform read and write operations.*/
    struct ext2_super_block sb;  /*The superblock structure*/
    struct ext2_group_desc *gd;  /*Pointer to an array of group descriptor structures*/
    uint32_t block_size;
};

/*Function prototypes*/
static void read_super_block(FS_t *fs);
static void read_group_desc(FS_t *fs);
static struct ext2_inode *read_inode(FS_t *fs, uint32_t inode_num);
static uint32_t search_dir(FS_t *fs, uint32_t inode_num, const char *name);

/*Reads the superblock from the filesystem*/
static void read_super_block(FS_t *fs)
{
    lseek(fs->fd, EXT2_SUPERBLOCK_OFFSET, SEEK_SET);  /*Moves the file pointer to the location of the superblock (offset 1024 bytes)*/ 
    if (sizeof(struct ext2_super_block) != read(fs->fd, &fs->sb, sizeof(struct ext2_super_block)))  /*Tries to read exactly sizeof(struct ext2_super_block) bytes. If read() returns any other number (either more, less, or error (-1)), an error is triggered.*/
    {
        perror("Error reading superblock");
        exit(1);
    }
    if (fs->sb.s_magic != EXT2_SUPER_MAGIC) 
    {
        fprintf(stderr, "Not a valid ext2 filesystem\n");
        exit(1);
    }
    fs->block_size = EXT2_SUPERBLOCK_OFFSET << fs->sb.s_log_block_size;  /*Example: if s_log_block_size is 2, then 1024 << 2 = 1024 * 2^2 = 4096 (4 kb block size).*/
    printf("Block size: %u\n", fs->block_size);
}

/*Reads the group descriptors from the filesystem*/
static void read_group_desc(FS_t *fs)
{
    uint32_t num_groups = 0;
    uint32_t gd_size = 0;  /*Size of group descriptor array*/

    num_groups = (fs->sb.s_blocks_count + fs->sb.s_blocks_per_group - 1) / fs->sb.s_blocks_per_group;  /*Calculates the number of block groups in the filesystem. Example: (21+10)/10=3*/
    gd_size = sizeof(struct ext2_group_desc) * num_groups;
    fs->gd = malloc(gd_size);
    if (!fs->gd) 
    {
        perror("Failed to allocate memory for group descriptors");
        exit(1);
    }
    lseek(fs->fd, fs->block_size, SEEK_SET);  /*Moves the pointer to the start of block 2 to read from the group descriptor table.*/
    if (read(fs->fd, fs->gd, gd_size) != gd_size)  /*Reads the group descriptor table from the filesystem into the allocated memory (fs->gd)*/
    {
        perror("Error reading group descriptors");
        exit(1);
    }
}

/*Opens the filesystem*/
FS_t* FSOpen(const char* disk_name)
{
    FS_t* fs;

    fs = (FS_t*)malloc(sizeof(FS_t));  /*Memory allocation for the FS_t struct*/
    if (fs == NULL) 
    {
        perror("Failed to allocate memory for FS structure");
        return NULL;
    }

    fs->fd = open(disk_name, O_RDWR);  /*Opens the disk specified by disk_name (/dev/ram0 in my case) in read-write mode (RDWR - added write option for chmode function), and returns a file descriptor which is stored in fs->fd.*/
    if (fs->fd == -1) 
    {
        perror("Failed to open disk");
        free(fs);
        return NULL;
    }

    read_super_block(fs);  /*The read_super_block function is called to read the superblock of the filesystem into the FS_t structure.*/
    read_group_desc(fs);

    return fs;
}

/*Closes the filesystem and frees the group descriptor and the FS_t struct allocated memory*/
int FSClose(FS_t* file_system)
{
    if (file_system == NULL)
        return -1;

    close(file_system->fd);
    free(file_system->gd);
    free(file_system);
    return 0;
}

/*Finds the exact location of the specified inode number in the filesystem using byte offsets, reads its data, and populates the ext2_inode struct. Returns a pointer to the struct (instead of void) since there can be multiple inodes.*/
static struct ext2_inode *read_inode(FS_t *fs, uint32_t inode_num)
{
    uint32_t group = 0;
    uint32_t index = 0;
    uint32_t block = 0;
    uint32_t offset = 0;
    struct ext2_inode *inode = NULL;

    group = (inode_num - 1) / fs->sb.s_inodes_per_group;  /*Calculates which group the given inode_num belongs to.*/
    index = (inode_num - 1) % fs->sb.s_inodes_per_group;  /*Calculates the index of the inode within its group's inode table.*/
    block = fs->gd[group].bg_inode_table + (index * fs->sb.s_inode_size) / fs->block_size;  /*Calculates the block number where the inode data is stored by adding the starting block of the group's inode table to the offset in blocks for the specific inode.*/

    offset = (index * fs->sb.s_inode_size) % fs->block_size;  /*Calculates the byte offset within the block where the inode is located.*/

    inode = malloc(sizeof(struct ext2_inode));
    if (!inode) 
    {
        perror("Failed to allocate memory for inode");
        return NULL;
    }

    lseek(fs->fd, block * fs->block_size + offset, SEEK_SET);  /*Moves the file pointer to the exact location of the inode on disk*/
    if (read(fs->fd, inode, sizeof(struct ext2_inode)) != sizeof(struct ext2_inode))  /*Reads the inode data from the disk into the allocated inode structure.*/
    {
        perror("Error reading inode");
        free(inode);
        return NULL;
    }

    return inode;
}

/*Searches for a file in a directory*/
static uint32_t search_dir(FS_t *fs, uint32_t inode_num, const char *name)
{
    struct ext2_inode *inode = NULL;  /*Pointer to an inode structure for the specified directory.*/
    uint8_t *block = NULL;  /*Pointer to memory allocated for a block of directory entries.*/
    struct ext2_dir_entry *entry = NULL;  /*Pointer to a directory entry structure.*/
    uint32_t offset = 0;  /*Keeps track of the current position in the directory block.*/
    char file_name[EXT2_NAME_LEN+1];  /*Array to store the name of the found file/directory.*/
    uint32_t inode_number = 0;  /*Will hold the inode number of the found entry (initialized to 0).*/

    inode = read_inode(fs, inode_num);  /*Calls the read_inode function so it can determine where the directory's entries are stored on disk, in order to search for the specified filename.*/
    if (!inode) 
        return 0;  /*The directory's inode could not be found*/

    block = malloc(fs->block_size);
    if (!block) 
    {
        perror("Failed to allocate memory for block");
        free(inode);
        return 0;
    }

    lseek(fs->fd, inode->i_block[0] * fs->block_size, SEEK_SET);  /*Moves the file pointer to the start of the first block of the directory by multiplying the block number by the block size.*/

    if (fs->block_size != read(fs->fd, block, fs->block_size))  /*Reads the contents of the directory block into "block".*/
    {
        perror("Error reading directory block");
        free(inode);
        free(block);
        return 0;
    }

    offset = 0;
    while (offset < inode->i_size)  /*Iterates through the directory entries within a specific directory block*/
    {
        entry = (struct ext2_dir_entry *)(block + offset);  /*Calculates the address of the current directory entry based on the offset and casts it to a pointer of type struct ext2_dir_entry in order to be able to access the members of that struct.*/ 
        memcpy(file_name, entry->name, entry->name_len);  /*Copies the name of the file/directory from the entry structure to the file_name array. We copy the name here so that we can manually add a null terminating character, which isn't guaranteed if we just use entry->name for strcmp.*/
        file_name[entry->name_len] = '\0';  /*Adds a null terminator at the end of the copied string*/

        printf("Found entry: %s (inode: %u)\n", file_name, entry->inode);

        if (strcmp(file_name, name) == 0)  /*Compares the current file_name with the "name parameter" (the name being searched for).*/
        {
            inode_number = entry->inode;
            break;
        }

        offset += entry->rec_len;  /*Increments the offset by the length of the current directory entry in order to move to the next entry in the block.*/
    }

    free(inode);
    free(block);
    return inode_number;
}

/*Gets the inode number for a file based on its path*/
long GetInode(FS_t* file_system, const char* file_path)
{
    const char *mount_point = "/mnt/ramdisk";
    const char *relative_path = NULL;  /*The portion of file_path after the mount point.*/
    char *path = NULL;  /*A copy of relative_path that will be tokenized (split) to traverse the directories.*/
    char *token = NULL;  /*Each directory or file component (separated by /) from the path.*/
    uint32_t inode_num = 0;  /*The inode number of the current directory or file being searched.*/

    if (strncmp(file_path, mount_point, strlen(mount_point)) != 0)  /*Checks if the file_path starts with the mount point*/
    {
        fprintf(stderr, "Error: The path must start with the mount point: %s\n", mount_point);
        return -1;
    }

    relative_path = file_path + strlen(mount_point);  /*Points to the part of file_path after the mount_point.*/

    if (*relative_path == '/')  /*Skips the leading '/' if it exists*/
        relative_path++;

    path = strdup(relative_path);  /*Now, relative_path is the actual path we want to search*/

    token = strtok(path, "/");  /*Points to the first component of the path (like a directory or file name), and replaces the '/' with a null terminator.*/
    inode_num = EXT2_ROOT_INO;  /*The search starts from the root directory*/

    while (token != NULL) 
    {
        printf("Searching for: %s\n", token);
        inode_num = search_dir(file_system, inode_num, token);  /*Retrieves the inode number associated with the directory or file represented by token.*/

        if (inode_num == 0) 
        {
            printf("Failed to find: %s\n", token);
            free(path);
            return -1; /*Returns if the current token was not found*/
        }

        token = strtok(NULL, "/"); /*Moves to the next token in the path. The parameter is NULL because the previous call to strtok replaced '/' with a null terminator.*/
    }

    free(path);
    return inode_num; /*Returns the inode number of the final token*/
}

/*Helper function to convert permission string to mode bits*/
static int parse_mode(const char *mode_str)
{
    int mode = 0;
    
    /*Parse user permissions using bitwise operations*/
    if (mode_str[0] == 'r') mode |= 0400;
    if (mode_str[1] == 'w') mode |= 0200;
    if (mode_str[2] == 'x') mode |= 0100;
    
    /*Parse group permissions*/
    if (mode_str[3] == 'r') mode |= 0040;
    if (mode_str[4] == 'w') mode |= 0020;
    if (mode_str[5] == 'x') mode |= 0010;
    
    /*Parse others permissions*/
    if (mode_str[6] == 'r') mode |= 0004;
    if (mode_str[7] == 'w') mode |= 0002;
    if (mode_str[8] == 'x') mode |= 0001;
    
    return mode;
}

int Chmode(FS_t* file_system, long inode_num, char *new_mode)
{
    struct ext2_inode *inode = NULL;
    int mode_bits = 0;

    /*Input validation*/
    if (!file_system || !new_mode || strlen(new_mode) != 9)
    {
        fprintf(stderr, "Input validation failed: file_system = %p, new_mode = %s\n", (void*)file_system, new_mode);
        return -1;
    }

    /*Read current inode - this function already handles the location calculations*/
    inode = read_inode(file_system, inode_num);
    if (!inode)
    {
        fprintf(stderr, "Error reading inode: inode_num = %ld\n", inode_num);
        return -1;
    }

    /*Parse new mode and preserve file type bits*/
    mode_bits = parse_mode(new_mode);
    if (mode_bits == -1)
    {
        fprintf(stderr, "Error parsing mode: new_mode = %s\n", new_mode);
        free(inode);
        return -1;
    }

    inode->i_mode = (inode->i_mode & 0xF000) | mode_bits;

    /*Write the modified inode back to disk at the same location it was read from.
       The lseek function uses SEEK_CUR to calculate the offset relative to the current file position,
       moving backwards by the size of struct ext2_inode. This is used instead of SEEK_SET,
       which would calculate offsets from the beginning of the file.*/
    if (-1 == lseek(file_system->fd, -sizeof(struct ext2_inode), SEEK_CUR))
    {
        perror("Error seeking to inode location");
        free(inode);
        return -1;
    }

    /*Write updated inode back to disk*/
    if (sizeof(struct ext2_inode) != write(file_system->fd, inode, sizeof(struct ext2_inode)))
    {
        perror("Error writing inode back to disk");
        free(inode);
        return -1;
    }

    free(inode);
    return 0;
}


/*Prints the contents of a file based on the provided inode number.*/
void PrintFile(FS_t* file_system, long inode_num)
{
    struct ext2_inode *inode = NULL;
    uint8_t *buffer = NULL;
    int i = 0;
    ssize_t bytes_read = 0;
    int bytes_to_print = 0;

    inode = read_inode(file_system, inode_num);  /*Allocates memory for an ext2_inode structure. Reads the inode data from the file system into this struct, and returns a pointer to it, which now "inode" also points to.*/
    if (!inode) 
    {
        printf("Failed to read inode %ld\n", inode_num);
        return;
    }

    buffer = malloc(file_system->block_size);  /*A buffer is allocated to hold the contents of a block from the file. The buffer allows us to handle larger chunks of data at once, rather than making multiple, smaller reads directly from the file descriptor.*/
    if (!buffer) 
    {
        perror("Failed to allocate memory for file buffer");
        free(inode);
        return;
    }

    printf("File contents:\n");

    for (i = 0; i < EXT2_NUM_DIRECT_BLOCKS && inode->i_block[i]; ++i)  /*The loop iterates up to 12 times (the maximum number of direct blocks for an inode in ext2) as long as there are valid block pointers in inode->i_block[i].*/
    {
        lseek(file_system->fd, inode->i_block[i] * file_system->block_size, SEEK_SET);  /*Sets the file pointer to the position of the current block of the file.*/
        bytes_read = read(file_system->fd, buffer, file_system->block_size);  /*Reads the contents of the block into the buffer.*/
        if (bytes_read == -1) 
        {
            perror("Error reading file block");
            break;
        }

        /*Determines how many bytes to print from the current block. 
          If the current block is the last direct block (either it's index 11 or the next block is NULL), 
          it calculates the number of remaining bytes in the file to print. 
          Otherwise, it prints the entire block.*/
        bytes_to_print = (i == EXT2_LAST_DIRECT_BLOCK_INDEX || !inode->i_block[i+1]) 
                         ? inode->i_size % file_system->block_size 
                         : file_system->block_size;

        write(STDOUT_FILENO, buffer, bytes_to_print);  /*A system call used to output the contents of the buffer to the standard output (the terminal). It bypasses a buffering stage and is thus faster than printf.*/ 
    }

    free(inode);
    free(buffer);
}