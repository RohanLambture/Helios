// screen dimensions
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

// VGA memory address
#define VGA_MEMORY_ADDRESS 0xb8000

// Colors
#define WHITE_ON_BLACK 0x0f
#define RED_ON_WHITE 0xf4
#define GREEN_ON_BLACK 0x0a

// VGA data ports
#define VGA_CTRL_PORT 0x3d4
#define VGA_DATA_PORT 0x3d5

void clear_screen(int col,int row);
void printk_at(char *message,int col,int row,int color);
void printk(char *message);