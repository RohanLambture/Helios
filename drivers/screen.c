#include "screen.h"
#include "ports.h"

/* Private functions declarations */
int printk_char(char c,int col,int row,int color);
int get_cursor_offset();
void set_cursor_offset(int offset);
int get_offset(int col, int row);
int get_offset_row(int offset);
int get_offset_col(int offset);

/* Public functions */
void printk_at(char *message,int col,int row , int color){
	if(!message)return;
	if(!color){
		color = WHITE_ON_BLACK;
	}
	int index = 0;
	int offset;
	while(message[index] !='\0'){
		offset = printk_char(message[index++],col,row,color);
		row = get_offset_row(offset);
		col = get_offset_col(offset);
	}
	set_cursor_offset(get_offset(col, row));
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
}

/* Private functions implementation */

int printk_char(char c,int col,int row,int color){
	if(col >= SCREEN_WIDTH || row >= SCREEN_HEIGHT){
		char *vga = (char*)VGA_MEMORY_ADDRESS;
		vga[2*SCREEN_HEIGHT*SCREEN_WIDTH-2]= 'X';
		vga[2*SCREEN_HEIGHT*SCREEN_WIDTH-1]= RED_ON_WHITE;
	}
	int offset = get_offset(col,row);
	if(!color){
		color = WHITE_ON_BLACK;
	}
	if(offset < 0 ){
		int position = get_cursor_offset();
	}
	// TODO:do something for offset >= SCREEN_HEIGHT * SCREEN_WIDTH * 2
	if(c == '\n'){
		row++;
		col = 0;
		offset = get_offset(col,row);
	}else{
		char *vga = (char*)VGA_MEMORY_ADDRESS;
		vga[offset] = c;
		vga[offset+1] = color;
		offset+=2;
	}
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