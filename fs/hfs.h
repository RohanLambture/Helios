#ifndef HFS_H
#define HFS_H

#include "../include/types.h"

#define HFS_BLOCK_SIZE		512
#define HFS_MAX_FILENAME	32
#define HFS_MAX_FILES		64
#define HFS_MAX_FILE_SIZE	(16 * HFS_BLOCK_SIZE)  // 8KB
#define HFS_TOTAL_BLOCKS	128
#define HFS_DIRECT_BLOCKS	16  // how many block pointers each inode has
#define HFS_BITMAP_SIZE		((HFS_TOTAL_BLOCKS + 7) / 8) // to track data blocks' availability
#define HFS_INODE_BITMAP_SIZE	((HFS_MAX_FILES + 7 ) / 8)   // to track inodes' availability

#define HFS_MAGIC		0x48465321  // "HFS!"
#define HFS_TYPE_FILE		1
#define HFS_TYPE_DIR		2

#define HFS_SUCCESS		0
#define HFS_ERROR		-1
#define HFS_INVALID_INODE	0xFFFF
#define HFS_INVALID_BLOCK	0xFFFF


struct hfs_superblock {
	uint32_t magic_number;  // to indentify fs type
	uint32_t total_blocks;
	uint32_t total_inodes;
	uint32_t free_blocks;
	uint32_t free_inodes;
	uint8_t reserved[496];  // Padding to block size
};

struct hfs_inode{
	uint8_t type;  // file or directory
	char name[HFS_MAX_FILENAME];  // name of file/directory
	uint32_t size;  // size of file
	uint16_t parent; // parent directory
	uint16_t direct_block[HFS_DIRECT_BLOCKS];
	uint8_t reserved[32];
};


void hfs_init(void);
int hfs_create_file(const char *filename);
int hfs_create_dir(const char *dirname);
int hfs_write_file(const char *filename,const char *data,uint32_t size);
int hfs_read_file(const char *filename,char *buffer,uint32_t buffer_size,uint32_t *bytes_read);
int hfs_delete(const char *name);
void hfs_list_dir(void);
int hfs_change_dir(const char *dirname);


#endif
