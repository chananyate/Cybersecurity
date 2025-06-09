#ifndef EXT2_PARSING_H
#define EXT2_PARSING_H

typedef struct FS FS_t;

/* find mounting point of FS */
FS_t* FSOpen(const char* disk_name);

/* destroy file descriptor */
int FSClose(FS_t* file_system);

/* find and return inode number */
long GetInode(FS_t* file_system, const char* file_path);

/* perfrom read action from file pointed to by the inode */
void PrintFile(FS_t* file_system, long inode_num);







#endif 			/* 	EXT2_PARSING_H	*/