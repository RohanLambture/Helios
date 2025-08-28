#include "hfs.h"
#include "../libc/string.h"
#include "../include/types.h"
#include "../libc/mem.h"
#include "../drivers/screen.h"

// File-system
static struct hfs_superblock superblock;
static struct hfs_inode inodes[HFS_MAX_FILES];
static uint8_t data_blocks[HFS_TOTAL_BLOCKS][HFS_BLOCK_SIZE];
static uint8_t block_bitmap[HFS_BITMAP_SIZE];
static uint8_t inode_bitmap[HFS_INODE_BITMAP_SIZE];
static uint8_t fs_initialized = 0;
static uint16_t current_dir = 0;

void hfs_init(){
	// clearing all structures
	memset((uint8_t*)&superblock,0,sizeof( struct hfs_superblock));
	memset((uint8_t*)inodes,0,sizeof(inodes));
	memset((uint8_t*)data_blocks,0,sizeof(data_blocks));
	memset(block_bitmap,0,HFS_BITMAP_SIZE);
	memset(inode_bitmap,0,HFS_INODE_BITMAP_SIZE);


	// superblock Initialize
	superblock.magic_number = HFS_MAGIC;
	superblock.total_blocks = HFS_TOTAL_BLOCKS;
	superblock.free_blocks = HFS_TOTAL_BLOCKS - 1; // block 0 for superblock
	superblock.total_inodes = HFS_MAX_FILES;
	superblock.free_inodes = HFS_MAX_FILES - 1;  // inode 0 for root

	block_bitmap[0] = 1; // marking as used (superblock)

	//root directory
	inode_bitmap[0] = 1;
	inodes[0].type = HFS_TYPE_DIR;
	inodes[0].size = 0;
	inodes[0].parent = 0; // parent of root is root

	for(int i=0;i<HFS_MAX_FILENAME;i++){
		inodes[0].name[i] = '\0';
	}
	inodes[0].name[0] = '/';

	current_dir = 0;
	fs_initialized = 1;

	kprint("HFS: File-system activiated\n");
}

static uint16_t find_free_inode(){
	for(int i = 0;i < HFS_MAX_FILES; i++){
		if(inode_bitmap[i] == 0) return i;
	}
	return HFS_INVALID_INODE;
}

static uint16_t find_free_block(){
	for(int i = 1;i <HFS_TOTAL_BLOCKS ; i++){
		if(block_bitmap[i] == 0) return i;
	}
	return HFS_INVALID_BLOCK;
}

static uint16_t allocate_node(){
	uint16_t free_inode = find_free_inode();
	if(free_inode != HFS_INVALID_INODE){
		inode_bitmap[free_inode] = 1;
		superblock.free_inodes--;
		memset((uint8_t*)&inodes[free_inode],0,sizeof(struct hfs_inode));
	}
	return free_inode;
}

static uint16_t allocate_block(){
	uint16_t free_block = find_free_block();
	if(free_block != HFS_INVALID_BLOCK){
		block_bitmap[free_block] = 1;
		superblock.free_blocks--;
		memset(data_blocks[free_block],0,HFS_BLOCK_SIZE);
	}
	return free_block;
}

static void free_inode(uint16_t inode_num){
	if(inode_num >= HFS_MAX_FILES) return ;
	// TODO : Complete this
}

// TODO : implement this methods
static uint16_t find_file_in_dir(uint16_t dir_inode,const char *name);
int hfs_create_file(const char *filename);
int hfs_create_dir(const char *dirname);
int hfs_write_file(const char *filename,const char *data,uint32_t size);
int hfs_read_file(const char *filename, char *buffer, uint32_t buffer_size, uint32_t *bytes_read);
int hfs_delete(const char *name);
void hfs_list_dir();
int hfs_change_dir(const char *dirname);
