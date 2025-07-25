/* asmhead.nas */
struct BOOTINFO { /* 0x0ff0-0x0fff */
	char cyls; /* 启动区读磁盘读到此为止 */
	char leds; /* 启动时键盘的LED的状态 */
	char vmode; /* 显卡模式为多少位彩色 */
	char reserve;
	short scrnx, scrny; /* 画面分辨率 */
	char *vram;
};
#define ADR_BOOTINFO	0x00000ff0
#define ADR_DISKIMG		0x00100000

/* naskfunc.nas */
void io_hlt(void);
void io_cli(void);
void io_sti(void);
void io_stihlt(void);
int io_in8(int port);
void io_out8(int port, int data);
int io_load_eflags(void);
void io_store_eflags(int eflags);
void load_gdtr(int limit, int addr);
void load_idtr(int limit, int addr);
int load_cr0(void);
void store_cr0(int cr0);
void load_tr(int tr);
void store_cr3(int cr3);
unsigned int read_cr3(void);
void flush_tlb(void);
void asm_inthandler0c(void);
void asm_inthandler0d(void);
void asm_inthandler20(void);
void asm_inthandler21(void);
void asm_inthandler2c(void);
unsigned int memtest_sub(unsigned int start, unsigned int end);
void farjmp(int eip, int cs);
void farcall(int eip, int cs);
void asm_hrb_api(void);
void start_app(int eip, int cs, int esp, int ds, int *tss_esp0);
void asm_end_app(void);

/* fifo.c */
struct FIFO32 {
	int *buf;
	int p, q, size, free, flags;
	struct TASK *task;
};
void fifo32_init(struct FIFO32 *fifo, int size, int *buf, struct TASK *task);
int fifo32_put(struct FIFO32 *fifo, int data);
int fifo32_get(struct FIFO32 *fifo);
int fifo32_status(struct FIFO32 *fifo);

/* graphic.c */
void init_palette(void);
void set_palette(int start, int end, unsigned char *rgb);
void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1);
void init_screen8(char *vram, int x, int y);
void putfont8(char *vram, int xsize, int x, int y, char c, char *font);
void putfonts8_asc(char *vram, int xsize, int x, int y, char c, unsigned char *s);
void init_mouse_cursor8(char *mouse, char bc);
void putblock8_8(char *vram, int vxsize, int pxsize,
	int pysize, int px0, int py0, char *buf, int bxsize);
#define COL8_000000		0
#define COL8_FF0000		1
#define COL8_00FF00		2
#define COL8_FFFF00		3
#define COL8_0000FF		4
#define COL8_FF00FF		5
#define COL8_00FFFF		6
#define COL8_FFFFFF		7
#define COL8_C6C6C6		8
#define COL8_840000		9
#define COL8_008400		10
#define COL8_848400		11
#define COL8_000084		12
#define COL8_840084		13
#define COL8_008484		14
#define COL8_848484		15

/* dsctbl.c */
struct SEGMENT_DESCRIPTOR {
	short limit_low, base_low;
	char base_mid, access_right;
	char limit_high, base_high;
};
struct GATE_DESCRIPTOR {
	short offset_low, selector;
	char dw_count, access_right;
	short offset_high;
};
void init_gdtidt(void);
void set_segmdesc(struct SEGMENT_DESCRIPTOR *sd, unsigned int limit, int base, int ar);
void set_gatedesc(struct GATE_DESCRIPTOR *gd, int offset, int selector, int ar);
#define ADR_IDT			0x0026f800
#define LIMIT_IDT		0x000007ff
#define ADR_GDT			0x00270000
#define LIMIT_GDT		0x0000ffff
#define ADR_BOTPAK		0x00280000
#define LIMIT_BOTPAK	0x0007ffff
#define AR_DATA32_RW	0x4092
#define AR_CODE32_ER	0x409a
#define AR_LDT			0x0082
#define AR_TSS32		0x0089
#define AR_INTGATE32	0x008e

/* int.c */
void init_pic(void);
#define PIC0_ICW1		0x0020
#define PIC0_OCW2		0x0020
#define PIC0_IMR		0x0021
#define PIC0_ICW2		0x0021
#define PIC0_ICW3		0x0021
#define PIC0_ICW4		0x0021
#define PIC1_ICW1		0x00a0
#define PIC1_OCW2		0x00a0
#define PIC1_IMR		0x00a1
#define PIC1_ICW2		0x00a1
#define PIC1_ICW3		0x00a1
#define PIC1_ICW4		0x00a1

/* keyboard.c */
void inthandler21(int *esp);
void wait_KBC_sendready(void);
void init_keyboard(struct FIFO32 *fifo, int data0);
#define PORT_KEYDAT		0x0060
#define PORT_KEYCMD		0x0064

/* mouse.c */
struct MOUSE_DEC {
	unsigned char buf[3], phase;
	int x, y, btn;
};
void inthandler2c(int *esp);
void enable_mouse(struct FIFO32 *fifo, int data0, struct MOUSE_DEC *mdec);
int mouse_decode(struct MOUSE_DEC *mdec, unsigned char dat);

/* memory.c */
<<<<<<< HEAD
// paging
#define CR0_PG				0x80000000	// paging enable
#define KERNEL_PAGE_DIR  	0x00400000  // PDT phy addr
#define KERNEL_PAGE_TBL   	0x00401000  // PT phy addr
#define KERNEL_VIRT_ADDR  	0xc0000000

// paging flags
#define PG_P              0x00000001  // Present
#define PG_RW             0x00000002  // Read/Write
#define PG_US             0x00000004  // User/Supervisor
#define PG_A              0x00000020  // Accessed
#define PG_D              0x00000040  // Dirty
#define PG_PS             0x00000080  // Page Size (4MB)

#define KERNEL_V2P(x) ((x) - 0xc0000000)
#define KERNEL_P2V(x) ((x) + 0xc0000000)

=======
>>>>>>> cd96f5c5205cd987613df74b40457a5ca0d64373
#define MEMMAN_FREES		4090	/* 32KB */
#define MEMMAN_ADDR			0x003c0000
// //
// memory.c 新增内容
#define DISK_SIZE 1474560  // 1.44MB，模拟软盘大小
#define FAT12_BOOT_SECTOR_SIZE 512
#define FAT12_FAT_SIZE 2880   // FAT12每个FAT表大小
#define FAT12_ROOT_DIR_SIZE 512 * 4  // 根目录大小
// 之前为了图省事只初始化了一个FAT表，但是计算的时候用的两个，导致数据区起始位置计算错误
// data_start = FAT12_DATA_START / 512 会比实际大 FAT12_FAT_SIZE / 512 扇区（即一个 FAT 表的扇区数）
// 因此我们先尝试单 FAT 表的简化版本 - 但是在内存组织上还是按照标准的 FAT12 格式进行
// #define FAT12_DATA_START (FAT12_BOOT_SECTOR_SIZE + FAT12_FAT_SIZE + FAT12_ROOT_DIR_SIZE)
#define FAT12_DATA_START 16896
#define MAX_CLUSTERS ((DISK_SIZE - FAT12_DATA_START) / 512)
//  //


struct FREEINFO {	/* ������� */
	unsigned int addr, size;
};
struct MEMMAN {		/* �������Ǘ� */
	int frees, maxfrees, lostsize, losts;
	struct FREEINFO free[MEMMAN_FREES];
};
unsigned int memtest(unsigned int start, unsigned int end);
void memman_init(struct MEMMAN *man);
unsigned int memman_total(struct MEMMAN *man);
unsigned int memman_alloc(struct MEMMAN *man, unsigned int size);
int memman_free(struct MEMMAN *man, unsigned int addr, unsigned int size);
unsigned int memman_alloc_4k(struct MEMMAN *man, unsigned int size);
int memman_free_4k(struct MEMMAN *man, unsigned int addr, unsigned int size);
void init_paging();

/* sheet.c */
#define MAX_SHEETS		256
struct SHEET {
	unsigned char *buf;
	int bxsize, bysize, vx0, vy0, col_inv, height, flags;
	struct SHTCTL *ctl;
	struct TASK *task;
};
struct SHTCTL {
	unsigned char *vram, *map;
	int xsize, ysize, top;
	struct SHEET *sheets[MAX_SHEETS];
	struct SHEET sheets0[MAX_SHEETS];
};
struct SHTCTL *shtctl_init(struct MEMMAN *memman, unsigned char *vram, int xsize, int ysize);
struct SHEET *sheet_alloc(struct SHTCTL *ctl);
void sheet_setbuf(struct SHEET *sht, unsigned char *buf, int xsize, int ysize, int col_inv);
void sheet_updown(struct SHEET *sht, int height);
void sheet_refresh(struct SHEET *sht, int bx0, int by0, int bx1, int by1);
void sheet_slide(struct SHEET *sht, int vx0, int vy0);
void sheet_free(struct SHEET *sht);

/* timer.c */
#define MAX_TIMER		500
struct TIMER {
	// struct TIMER *next; // Removed
	unsigned int timeout, interval; // Added interval
	char flags, flags2;
	int heap_index; // Added heap_index
	struct FIFO32 *fifo;
	int data;
};
struct TIMERCTL {
	unsigned int count;
	// unsigned int next; // Removed
	// struct TIMER *t0; // Removed
	struct TIMER *heap[MAX_TIMER]; // Added heap array
	int heap_size;                 // Added heap size
	struct TIMER timers0[MAX_TIMER];
};
extern struct TIMERCTL timerctl;
void init_pit(void);
struct TIMER *timer_alloc(void);
void timer_free(struct TIMER *timer);
void timer_init(struct TIMER *timer, struct FIFO32 *fifo, int data);
// void timer_settime(struct TIMER *timer, unsigned int timeout); // Original
void timer_settime(struct TIMER *timer, unsigned int timeout, unsigned int interval); // Modified prototype
void inthandler20(int *esp);
int timer_cancel(struct TIMER *timer);
void timer_cancelall(struct FIFO32 *fifo);

/* mtask.c */
#define MAX_TASKS 1000	/*最大任务数量*/
#define TASK_GDT0 3			/*定义从GDT的几号开始分配给TSS */
#define MAX_TASKS_LV	100
#define MAX_TASKLEVELS	10
struct TSS32 {
	int backlink, esp0, ss0, esp1, ss1, esp2, ss2, cr3;
	int eip, eflags, eax, ecx, edx, ebx, esp, ebp, esi, edi;
	int es, cs, ss, ds, fs, gs;
	int ldtr, iomap;
};
struct TASK {
	int sel, flags;		/* sel用来存放GDT的编号*/
	int level, priority; /* 优先级 */
	struct FIFO32 fifo;
	struct TSS32 tss;
	struct SEGMENT_DESCRIPTOR ldt[2];
	struct CONSOLE *cons;
	int ds_base, cons_stack;
	struct FILEHANDLE *fhandle;
	int *fat;
	char *cmdline;
	unsigned char langmode, langbyte1;
	// struct FILEINFO *current_dir;       ///////// 新增字段，记录当前所在目录
	// int dir_level;                // 当前目录层级
    // struct FILEINFO* dir_stack[16]; // 目录栈（最大支持16层嵌套）
};
struct TASKLEVEL {
	int running; /*正在运行的任务数量*/
	int now; /*这个变量用来记录当前正在运行的是哪个任务*/
	struct TASK *tasks[MAX_TASKS_LV];
};
struct TASKCTL {
	int now_lv; /*现在活动中的LEVEL */
	char lv_change; /*在下次任务切换时是否需要改变LEVEL */
	struct TASKLEVEL level[MAX_TASKLEVELS];
	struct TASK tasks0[MAX_TASKS];
};
extern struct TASKCTL *taskctl;
extern struct TIMER *task_timer;
struct TASK *task_now(void);
struct TASK *task_init(struct MEMMAN *memman);
struct TASK *task_alloc(void);
void task_run(struct TASK *task, int level, int priority);
void task_switch(void);
void task_sleep(struct TASK *task);

/* window.c */
void make_window8(unsigned char *buf, int xsize, int ysize, char *title, char act);
void putfonts8_asc_sht(struct SHEET *sht, int x, int y, int c, int b, char *s, int l);
void make_textbox8(struct SHEET *sht, int x0, int y0, int sx, int sy, int c);
void make_wtitle8(unsigned char *buf, int xsize, char *title, char act);
void change_wtitle8(struct SHEET *sht, char act);

/* console.c */
struct CONSOLE {
	struct SHEET *sht;
	int cur_x, cur_y, cur_c;
	struct TIMER *timer;
};
struct FILEHANDLE {
	char *buf;
	int size;
	int pos;
};
void console_task(struct SHEET *sheet, int memtotal);
void cons_putchar(struct CONSOLE *cons, int chr, char move);
void cons_newline(struct CONSOLE *cons);
void cons_putstr0(struct CONSOLE *cons, char *s);
void cons_putstr1(struct CONSOLE *cons, char *s, int l);
void cons_runcmd(char *cmdline, struct CONSOLE *cons, int *fat, int memtotal);
void cmd_mem(struct CONSOLE *cons, int memtotal);
void cmd_cls(struct CONSOLE *cons);
void cmd_dir(struct CONSOLE *cons);
void cmd_exit(struct CONSOLE *cons, int *fat);
void cmd_start(struct CONSOLE *cons, char *cmdline, int memtotal);
void cmd_ncst(struct CONSOLE *cons, char *cmdline, int memtotal);
void cmd_langmode(struct CONSOLE *cons, char *cmdline);
int cmd_app(struct CONSOLE *cons, int *fat, char *cmdline);
int *hrb_api(int edi, int esi, int ebp, int esp, int ebx, int edx, int ecx, int eax);
int *inthandler0d(int *esp);
int *inthandler0c(int *esp);
void hrb_api_linewin(struct SHEET *sht, int x0, int y0, int x1, int y1, int col);

/* file.c */
struct FILEINFO {
	unsigned char name[8], ext[3], type; 	// 对应DIR_Name[11]（8+3）和DIR_Attr
	char reserve[10];						// 对应保留字段
	unsigned short time, date, clustno;		// 对应DIR_WrtTime、DIR_WrtDate、DIR_FstClus
	unsigned int size;						// 对应DIR_FileSize
};
void file_readfat(int *fat, unsigned char *img);
void file_loadfile(int clustno, int size, char *buf, int *fat, char *img);
struct FILEINFO *file_search(char *name, struct FILEINFO *finfo, int max);
char *file_loadfile2(int clustno, int *psize, int *fat);
struct FILEINFO* create_directory_in_memory(struct FILEINFO* parent_dir, const char* name);

/* tek.c */
int tek_getsize(unsigned char *p);
int tek_decomp(unsigned char *p, char *q, int size);

/* bootpack.c */
struct TASK *open_constask(struct SHEET *sht, unsigned int memtotal);
struct SHEET *open_console(struct SHTCTL *shtctl, unsigned int memtotal);

// 内存磁盘结构
struct MEM_DISK {
    unsigned char* data;
    int* fat;
    struct FILEINFO* root_dir;
    int initialized;
	struct FILEINFO *current_dir;       // 当前所在目录
    int dir_level;                      // 当前目录层级
    struct FILEINFO* dir_stack[16];     // 目录栈（最大支持16层嵌套）
};

extern struct MEM_DISK mem_disk;

void init_memory_disk(struct MEMMAN* memman);
void init_boot_sector(unsigned char* boot_sector);
void init_fat_table(int* fat);
void init_root_dir(struct FILEINFO* root_dir);
struct MEM_DISK* get_memory_disk();
void file_readfat_from_memory(int* fat);
void file_loadfile_from_memory(int clustno, int size, char* buf, int* fat);
int file_writefile_to_memory(int clustno, int size, char* buf, int* fat);
int alloc_cluster(int* fat);
struct FILEINFO* create_file_in_memory(struct FILEINFO* parent_dir, const char* name, int size);
struct FILEINFO* find_free_file_info(struct FILEINFO* root_dir, int* fat);
void parse_short_name(const char* long_name, char* short_name);
struct FILEINFO* find_file_info(struct FILEINFO* root_dir, const char* name);
void cmd_mkdir(struct CONSOLE* cons, char* dirname);
void cmd_touch(struct CONSOLE* cons, char* filename);
void cmd_cp(struct CONSOLE* cons, char* filename);
void cmd_paste(struct CONSOLE* cons);
void cmd_rm(struct CONSOLE* cons, char* filename);
void cmd_df(struct CONSOLE* cons);
void cmd_cat(struct CONSOLE* cons, char* filename, int* fat);
void cmd_write(struct CONSOLE* cons, char* filename, char* content, int* fat);
void cmd_rmdir(struct CONSOLE* cons, char* dirname);
void cmd_lsm(struct CONSOLE *cons);
void cmd_cd(struct CONSOLE* cons, char* dirname);
void cmd_sd(struct CONSOLE* cons);
int is_directory_empty(struct FILEINFO* dir_info, int* fat);
void trim_trailing_spaces(char *str);
void cmd_sp(struct CONSOLE* cons);
// 
