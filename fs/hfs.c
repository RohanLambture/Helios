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

void hfs_debug_info(void){
	if(!fs_initialized) {
		kprint("HFS: Filesystem not initialized\n");
		return;
	}

	kprint("=== FILESYSTEM DEBUG ===\n");
	kprint("Current dir: ");
	char debug_str[16];
	int_to_ascii(current_dir, debug_str);
	kprint(debug_str);
	kprint("\n");

	kprint("All allocated inodes:\n");
	for(int i = 0; i < HFS_MAX_FILES; i++) {
		if(BIT_TEST(inode_bitmap, i)) {
			int_to_ascii(i, debug_str);
			kprint("Inode ");
			kprint(debug_str);
			kprint(": '");
			kprint(inodes[i].name);
			kprint("' (parent: ");
			int_to_ascii(inodes[i].parent, debug_str);
			kprint(debug_str);
			kprint(", type: ");
			int_to_ascii(inodes[i].type, debug_str);
			kprint(debug_str);
			kprint(")\n");
		}
	}

	kprint("Free inodes: ");
	int_to_ascii(superblock.free_inodes, debug_str);
	kprint(debug_str);
	kprint(" / ");
	int_to_ascii(superblock.total_inodes, debug_str);
	kprint(debug_str);
	kprint("\n");
}

void hfs_init(){
	// // clearing all structures
	// memset((uint8_t*)&superblock,0,sizeof( struct hfs_superblock));
	// memset((uint8_t*)inodes,0,sizeof(inodes));
	// memset((uint8_t*)data_blocks,0,sizeof(data_blocks));
	// memset(block_bitmap,0,HFS_BITMAP_SIZE);
	// memset(inode_bitmap,0,HFS_INODE_BITMAP_SIZE);

	// superblock Initialize
	superblock.magic_number = HFS_MAGIC;
	superblock.total_blocks = HFS_TOTAL_BLOCKS;
	superblock.free_blocks = HFS_TOTAL_BLOCKS - 1; // block 0 for superblock
	superblock.total_inodes = HFS_MAX_FILES;
	superblock.free_inodes = HFS_MAX_FILES - 1;  // inode 0 for root

	BIT_SET(block_bitmap, 0); // mark block 0 used (superblock)

	//root directory
	BIT_SET(inode_bitmap, 0);
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
	for(int i = 1;i < HFS_MAX_FILES; i++){
		if(!BIT_TEST(inode_bitmap, i)) return i;
	}
	return HFS_INVALID_INODE;
}

static uint16_t find_free_block(){
	for(int i = 1;i <HFS_TOTAL_BLOCKS ; i++){
		if(!BIT_TEST(block_bitmap, i)) return i;
	}
	return HFS_INVALID_BLOCK;
}

static uint16_t allocate_inode(){
	uint16_t free_inode = find_free_inode();
	if(free_inode != HFS_INVALID_INODE){
		BIT_SET(inode_bitmap, free_inode);
		superblock.free_inodes--;
		memset((uint8_t*)&inodes[free_inode],0,sizeof(struct hfs_inode));
	}
	return free_inode;
}

static uint16_t allocate_block(){
	uint16_t free_block = find_free_block();
	if(free_block != HFS_INVALID_BLOCK){
		BIT_SET(block_bitmap, free_block);
		superblock.free_blocks--;
		memset(data_blocks[free_block],0,HFS_BLOCK_SIZE);
	}
	return free_block;
}

static void free_inode(uint16_t inode_num){
	if(inode_num >= HFS_MAX_FILES || !BIT_TEST(inode_bitmap, inode_num)) return ;
	BIT_CLR(inode_bitmap, inode_num);
	superblock.free_inodes++;

	for(int i = 0; i< HFS_DIRECT_BLOCKS; i++){
		// if(inodes[inode_num].direct_block[i] == 0) continue;
		// block_bitmap[inodes[inode_num].direct_block[i]] = 0;
		// if(inodes[inode_num].direct_block[i] == 0) continue;
		// BIT_CLR(block_bitmap, inodes[inode_num].direct_block[i]);
		// superblock.free_blocks++;
		uint16_t b = inodes[inode_num].direct_block[i];
		if(b == 0) continue;
		BIT_CLR(block_bitmap, b);
		superblock.free_blocks++;
		inodes[inode_num].direct_block[i] = 0;
		// FIXME : recursive ?
	}
	memset((uint8_t*)&inodes[inode_num],0,sizeof(struct hfs_inode));
}
static uint16_t find_file_in_dir(uint16_t dir_inode, const char* name) {
	if(dir_inode >= HFS_MAX_FILES || inodes[dir_inode].type != HFS_TYPE_DIR) {
		return HFS_INVALID_INODE;
	}
	// Search through all inodes for children of this directory
	for(uint16_t i = 0; i < HFS_MAX_FILES; i++) {
		if(BIT_TEST(inode_bitmap, i) && inodes[i].parent == dir_inode) {
			if(strcmp(inodes[i].name, (char*)name) == 0) {
				return i;
			}
		}
	}

	return HFS_INVALID_INODE;
}

int hfs_create_file(const char* filename) {
	if(!fs_initialized) {
		kprint("HFS: Filesystem not initialized\n");
		return HFS_ERROR;
	}

	if(strlen(filename) >= HFS_MAX_FILENAME) {
		kprint("HFS: Filename too long\n");
		return HFS_ERROR;
	}

	// Check if file already exists
	if(find_file_in_dir(current_dir, filename) != HFS_INVALID_INODE) {
		kprint("HFS: File already exists\n");
		return HFS_ERROR;
	}

	// Allocate new inode
	uint16_t inode_num = allocate_inode();
	if(inode_num == HFS_INVALID_INODE) {
		kprint("HFS: No free inodes\n");
		return HFS_ERROR;
	}

	// Initialize inode
	inodes[inode_num].type = HFS_TYPE_FILE;
	inodes[inode_num].size = 0;
	inodes[inode_num].parent = current_dir;

	// Copy filename
	int i;
	for(i = 0; i < strlen(filename) && i < HFS_MAX_FILENAME - 1; i++) {
		inodes[inode_num].name[i] = filename[i];
	}
	inodes[inode_num].name[i] = '\0';

	kprint("HFS: File created: ");
	kprint(filename);
	kprint("\n");

	return HFS_SUCCESS;
}

// Create directory
int hfs_create_dir(const char* dirname) {
	if(!fs_initialized) {
		kprint("HFS: Filesystem not initialized\n");
		return HFS_ERROR;
	}

	if(strlen(dirname) >= HFS_MAX_FILENAME) {
		kprint("HFS: Directory name too long\n");
		return HFS_ERROR;
	}

	// Check if directory already exists
	if(find_file_in_dir(current_dir, dirname) != HFS_INVALID_INODE) {
		kprint("HFS: Directory already exists\n");
		return HFS_ERROR;
	}

	// Allocate new inode
	uint16_t inode_num = allocate_inode();
	if(inode_num == HFS_INVALID_INODE) {
		kprint("HFS: No free inodes\n");
		return HFS_ERROR;
	}

	// Initialize inode
	inodes[inode_num].type = HFS_TYPE_DIR;
	inodes[inode_num].size = 0;
	inodes[inode_num].parent = current_dir;

	// Copy dirname
	int i;
	for(i = 0; i < strlen(dirname) && i < HFS_MAX_FILENAME - 1; i++) {
		inodes[inode_num].name[i] = dirname[i];
	}
	inodes[inode_num].name[i] = '\0';

	kprint("HFS: Directory created: ");
	kprint(dirname);
	kprint("\n");

	return HFS_SUCCESS;
}

// Write to file
int hfs_write_file(const char* filename, const char* data, uint32_t size) {
	if(!fs_initialized) {
		kprint("HFS: Filesystem not initialized\n");
		return HFS_ERROR;
	}

	// Find file
	uint16_t inode_num = find_file_in_dir(current_dir, filename);
	if(inode_num == HFS_INVALID_INODE) {
		kprint("HFS: File not found\n");
		return HFS_ERROR;
	}

	if(inodes[inode_num].type != HFS_TYPE_FILE) {
		kprint("HFS: Not a file\n");
		return HFS_ERROR;
	}

	if(size > HFS_MAX_FILE_SIZE) {
		kprint("HFS: File too large\n");
		return HFS_ERROR;
	}

	// Calculate blocks needed
	uint32_t blocks_needed = (size + HFS_BLOCK_SIZE - 1) / HFS_BLOCK_SIZE;
	if(blocks_needed > HFS_DIRECT_BLOCKS) {
		kprint("HFS: File too large for direct blocks\n");
		return HFS_ERROR;
	}

	// Free existing blocks
	for(int i = 0; i < HFS_DIRECT_BLOCKS; i++) {
		if(inodes[inode_num].direct_block[i] != 0) {
		block_bitmap[inodes[inode_num].direct_block[i]] = 0;
		superblock.free_blocks++;
		inodes[inode_num].direct_block[i] = 0;
		}
	}

	// Allocate new blocks
	for(uint32_t i = 0; i < blocks_needed; i++) {
		uint16_t block_num = allocate_block();
		if(block_num == HFS_INVALID_BLOCK) {
			kprint("HFS: No free blocks\n");
			return HFS_ERROR;
		}
		inodes[inode_num].direct_block[i] = block_num;
	}

	// Write data to blocks
	uint32_t remaining = size;
	uint32_t offset = 0;

	for(uint32_t i = 0; i < blocks_needed && remaining > 0; i++) {
		uint32_t to_write = (remaining > HFS_BLOCK_SIZE) ? HFS_BLOCK_SIZE : remaining;
		uint16_t block_num = inodes[inode_num].direct_block[i];

		for(uint32_t j = 0; j < to_write; j++) {
			data_blocks[block_num][j] = data[offset + j];
		}

		offset += to_write;
		remaining -= to_write;
	}

	inodes[inode_num].size = size;

	kprint("HFS: Data written to file: ");
	kprint(filename);
	kprint("\n");

	return HFS_SUCCESS;
}

// Read from file
int hfs_read_file(const char* filename, char* buffer, uint32_t buffer_size, uint32_t* bytes_read) {
	if(!fs_initialized) {
		kprint("HFS: Filesystem not initialized\n");
		return HFS_ERROR;
	}

	// Find file
	uint16_t inode_num = find_file_in_dir(current_dir, filename);
	if(inode_num == HFS_INVALID_INODE) {
		kprint("HFS: File not found\n");
		return HFS_ERROR;
	}

	if(inodes[inode_num].type != HFS_TYPE_FILE) {
		kprint("HFS: Not a file\n");
		return HFS_ERROR;
	}

	uint32_t file_size = inodes[inode_num].size;
	uint32_t to_read = (file_size > buffer_size) ? buffer_size : file_size;

	uint32_t remaining = to_read;
	uint32_t offset = 0;
	uint32_t block_index = 0;

	while(remaining > 0 && block_index < HFS_DIRECT_BLOCKS) {
		uint16_t block_num = inodes[inode_num].direct_block[block_index];
		if(block_num == 0) break;

		uint32_t from_block = (remaining > HFS_BLOCK_SIZE) ? HFS_BLOCK_SIZE : remaining;

		for(uint32_t i = 0; i < from_block; i++) {
			buffer[offset + i] = data_blocks[block_num][i];
		}

		offset += from_block;
		remaining -= from_block;
		block_index++;
	}

	*bytes_read = offset;
	return HFS_SUCCESS;
}

// Delete file or directory
int hfs_delete(const char* name) {
	if(!fs_initialized) {
		kprint("HFS: Filesystem not initialized\n");
		return HFS_ERROR;
	}

	// Find file/directory
	uint16_t inode_num = find_file_in_dir(current_dir, name);
	if(inode_num == HFS_INVALID_INODE) {
		kprint("HFS: File/Directory not found\n");
		return HFS_ERROR;
	}

	// Don't allow deleting root directory
	if(inode_num == 0) {
		kprint("HFS: Cannot delete root directory\n");
		return HFS_ERROR;
	}

	// If it's a directory, check if it's empty
	if(inodes[inode_num].type == HFS_TYPE_DIR) {
		for(uint16_t i = 0; i < HFS_MAX_FILES; i++) {
			if( BIT_TEST(inode_bitmap, i) && inodes[i].parent == inode_num) {
				kprint("HFS: Directory not empty\n");
				return HFS_ERROR;
			}
		}
	}

	// Free the inode
	free_inode(inode_num);

	kprint("HFS: Deleted: ");
	kprint(name);
	kprint("\n");

	return HFS_SUCCESS;
}

void hfs_list_dir() {
	if(!fs_initialized) {
		kprint("HFS: Filesystem not initialized\n");
		return;
	}

	kprint("Directory listing:\n");

	// Show current directory info
	kprint("Current directory: ");
	if(current_dir == 0) {
		kprint("/");
	} else {
		kprint(inodes[current_dir].name);
	}
	kprint("\n");

	// List all files/directories in current directory
	int count = 0;
	for(uint16_t i = 0; i < HFS_MAX_FILES; i++) {
		// if(inode_bitmap[i] && inodes[i].parent == current_dir) {
		if(BIT_TEST(inode_bitmap, i) && inodes[i].parent == current_dir){
			if(inodes[i].type == HFS_TYPE_DIR) {
				kprint("[DIR]  ");
			} else {
				kprint("[FILE] ");
			}
			kprint(inodes[i].name);
			if(inodes[i].type == HFS_TYPE_FILE) {
				kprint(" (");
				char size_str[16];
				int_to_ascii(inodes[i].size, size_str);
				kprint(size_str);
				kprint(" bytes)");
			}
			kprint("\n");
			count++;
		}
	}

	if(count == 0) {
		kprint("Directory is empty\n");
	}

	// Show filesystem stats
	char stats[32];
	int_to_ascii(superblock.free_inodes, stats);
	kprint("Free inodes: ");
	kprint(stats);
	kprint(" / ");
	int_to_ascii(superblock.total_inodes, stats);
	kprint(stats);
	kprint("\n");

	int_to_ascii(superblock.free_blocks, stats);
	kprint("Free blocks: ");
	kprint(stats);
	kprint(" / ");
	int_to_ascii(superblock.total_blocks, stats);
	kprint(stats);
	kprint("\n");
}

int hfs_change_dir(const char* dirname) {
	if(!fs_initialized) {
		kprint("HFS: Filesystem not initialized\n");
		return HFS_ERROR;
	}

	// Handle ".." (parent directory)
	if(strcmp((char*)dirname, "..") == 0) {
		if(current_dir != 0) {
		current_dir = inodes[current_dir].parent;
		kprint("HFS: Changed to parent directory\n");
		} else {
		kprint("HFS: Already at root directory\n");
		}
		return HFS_SUCCESS;
	}

	// Handle "/" (root directory)
	if(strcmp((char*)dirname, "/") == 0) {
		current_dir = 0;
		kprint("HFS: Changed to root directory\n");
		return HFS_SUCCESS;
	}

	// Find the directory
	uint16_t inode_num = find_file_in_dir(current_dir, dirname);
	if(inode_num == HFS_INVALID_INODE) {
		kprint("HFS: Directory not found\n");
		return HFS_ERROR;
	}

	if(inodes[inode_num].type != HFS_TYPE_DIR) {
		kprint("HFS: Not a directory\n");
		return HFS_ERROR;
	}

	current_dir = inode_num;
	kprint("HFS: Changed directory to: ");
	kprint(dirname);
	kprint("\n");

	return HFS_SUCCESS;
}
