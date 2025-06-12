#include "../drivers/screen.h"

int main() {
	printk("hello world\nNext line");
	printk_at("hello",80,30, 0x0f);
	return 0;
}
