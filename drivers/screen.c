#include "screen.h"
#include "ports.h"
#include "../kernel/util.h"

/* Private functions declarations */
int printk_char(char c,int col,int row,int color);
int get_cursor_offset();
void set_cursor_offset(int offset);
int get_offset(int col, int row);
int get_offset_row(int offset);
int get_offset_col(int offset);
void error();

/* Public functions */
void printk_at(char *message,int col,int row , int color){
	if(!message)return;
	if(!color){
		color = WHITE_ON_BLACK;
	}
	int index = 0;
	int offset = get_offset(col,row);
	while(message[index]!='\0'){
		char *vga = (char*)VGA_MEMORY_ADDRESS;
		vga[offset] = message[index++];
		vga[offset+1] = color;
		offset+=2;
	}
	// while(message[index] !='\0'){
	// 	offset = printk_char(message[index++],col,row,color);
	// 	row = get_offset_row(offset);
	// 	col = get_offset_col(offset);
	// }
}

void printk(char *message){
	printk_at(message,0,0,WHITE_ON_BLACK);
}

/* Clears the screen from current position */
void clear_screen(int col,int row){
	int offset = get_offset(col,row);
	char *vga = (char*)VGA_MEMORY_ADDRESS;
	while(offset < SCREEN_HEIGHT * SCREEN_WIDTH * 2){
		vga[offset]= ' ';
		vga[offset+1]= WHITE_ON_BLACK;
		offset+=2;
	}
	set_cursor_offset(get_offset(col, row));
}

/* Private functions implementation */

int printk_char(char c,int col,int row,int color){
	// if(col >= SCREEN_WIDTH || row >= SCREEN_HEIGHT){
	// 	// error();
	// }
	int offset = get_offset(col,row);
	// if(!color){
	// 	color = WHITE_ON_BLACK;
	// }
	// if(offset < 0 ){
	// 	offset = get_cursor_offset();
	// }
	// else if(offset >= 2 * SCREEN_HEIGHT * SCREEN_WIDTH){
	// 	// error();
	// 	// for(int i=1;i<SCREEN_HEIGHT;i++){
	// 	// 	memcpy((char*)VGA_MEMORY_ADDRESS+get_offset(0,i-1),(char*)VGA_MEMORY_ADDRESS+get_offset(0,i),SCREEN_WIDTH * 2);
	// 	// }
	// 	// Blank last line
	// 	// clear_screen(0, SCREEN_HEIGHT-1);
	// }
	if(c == '\n'){
		error();
		row++;
		col = 0;
		offset = get_offset(col,row);
	}else{
		char *vga = (char*)VGA_MEMORY_ADDRESS;
		vga[offset] = c;
		vga[offset+1] = color;
		offset+=2;
	}
	
	set_cursor_offset(offset);
	return offset;
}


int get_cursor_offset(){
	write_byte_to_port(VGA_CTRL_PORT, 14);
	int position = read_byte_from_port(VGA_DATA_PORT);
	position = position << 8;
	write_byte_to_port(VGA_CTRL_PORT, 15);
	position +=read_byte_from_port(VGA_DATA_PORT);
	return position * 2;
}

void set_cursor_offset(int offset){
	offset /=2;
	
	write_byte_to_port(VGA_CTRL_PORT, 14);
	write_byte_to_port(VGA_DATA_PORT, offset >> 8);
	write_byte_to_port(VGA_CTRL_PORT, 15);
	write_byte_to_port(VGA_DATA_PORT,offset & 0xff);
}

int get_offset(int col,int row){
	return 2 * (row * SCREEN_WIDTH + col);
}

int get_offset_row(int offset){
	return offset / ( SCREEN_WIDTH * 2 );
}

int get_offset_col(int offset){
	return (offset % (SCREEN_WIDTH * 2))/2;
}
int error_pos = 2 * SCREEN_WIDTH * SCREEN_HEIGHT ;
void error(){
	char *vga = (char*)VGA_MEMORY_ADDRESS;
	vga[error_pos-2]= 'X';
	vga[error_pos-1]= RED_ON_WHITE;
	// error_pos -=2;
}

