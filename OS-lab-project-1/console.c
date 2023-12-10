// Console input and output.
// Input is from the keyboard or serial port.
// Output is written to the screen and serial port.

#include "types.h"
#include "defs.h"
#include "param.h"
#include "traps.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "fs.h"
#include "file.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"

#define INPUT_BUF 128
struct {
  char buf[INPUT_BUF];
  uint r;  // Read index
  uint w;  // Write index
  uint e;  // Edit index
  uint idx;
  uint com;
  uint tot_com;
} input;

char com[11][INPUT_BUF];

static void consputc(int);

static int panicked = 0;

static struct {
  struct spinlock lock;
  int locking;
} cons;

static void
printint(int xx, int base, int sign)
{
  static char digits[] = "0123456789abcdef";
  char buf[16];
  int i;
  uint x;

  if(sign && (sign = xx < 0))
    x = -xx;
  else
    x = xx;

  i = 0;
  do{
    buf[i++] = digits[x % base];
  }while((x /= base) != 0);

  if(sign)
    buf[i++] = '-';

  while(--i >= 0)
    consputc(buf[i]);
}
//PAGEBREAK: 50

// Print to the console. only understands %d, %x, %p, %s.
void
cprintf(char *fmt, ...)
{
  int i, c, locking;
  uint *argp;
  char *s;

  locking = cons.locking;
  if(locking)
    acquire(&cons.lock);

  if (fmt == 0)
    panic("null fmt");

  argp = (uint*)(void*)(&fmt + 1);
  for(i = 0; (c = fmt[i] & 0xff) != 0; i++){
    if(c != '%'){
      consputc(c);
      continue;
    }
    c = fmt[++i] & 0xff;
    if(c == 0)
      break;
    switch(c){
    case 'd':
      printint(*argp++, 10, 1);
      break;
    case 'x':
    case 'p':
      printint(*argp++, 16, 0);
      break;
    case 's':
      if((s = (char*)*argp++) == 0)
        s = "(null)";
      for(; *s; s++)
        consputc(*s);
      break;
    case '%':
      consputc('%');
      break;
    default:
      // Print unknown % sequence to draw attention.
      consputc('%');
      consputc(c);
      break;
    }
  }

  if(locking)
    release(&cons.lock);
}

void
panic(char *s)
{
  int i;
  uint pcs[10];

  cli();
  cons.locking = 0;
  // use lapiccpunum so that we can call panic from mycpu()
  cprintf("lapicid %d: panic: ", lapicid());
  cprintf(s);
  cprintf("\n");
  getcallerpcs(&s, pcs);
  for(i=0; i<10; i++)
    cprintf(" %p", pcs[i]);
  panicked = 1; // freeze other CPU
  for(;;)
    ;
}

//PAGEBREAK: 50
#define BACKSPACE 0x100
#define ARROWUP 0xe2
#define ARROWDOWN 0xe3
#define LEFTARROW 75
#define RIGHTARROW 76
#define CRTPORT 0x3d4
static ushort *crt = (ushort*)P2V(0xb8000);  // CGA memory

static void
cgaputc(int c)
{
  int pos;
  int f_em = 0;
  // Cursor position: col + 80*row.
  outb(CRTPORT, 14);
  pos = inb(CRTPORT+1) << 8;
  outb(CRTPORT, 15);
  pos |= inb(CRTPORT+1);

  if(c == '\n'){
    pos += 80 - pos%80;
    f_em = 1;
  }
  else if(c == BACKSPACE){
    if(pos > 0 && input.idx > input.w){
      if (input.idx == input.e){
        input.idx--;
        input.e--;
        f_em = 1;
      }else{
        int crt_pos = pos - 1;
        for (int i = input.idx - 1; i < input.e; i++){
          crt[crt_pos] = crt[crt_pos + 1];
          crt_pos++;
          input.buf[i % INPUT_BUF] = input.buf[(i + 1) % INPUT_BUF];
        }
        // crt[crt_pos] = ' ' | 0x0700;
        input.idx--;
        input.e --;
      }
      --pos;
    }
  }
  else if(c == LEFTARROW){
    if (pos > 0) pos --;
  }else if(c == RIGHTARROW){
    pos ++;
  }
  else{
    if (input.e == input.idx){
      f_em = 1;
    }else{
      int crt_tmp = pos + input.e - input.idx;
      for (int i = input.e; i > input.idx; i--){
        crt[crt_tmp] = crt[crt_tmp - 1];
        input.buf[i % INPUT_BUF] = input.buf[(i - 1) % INPUT_BUF];
        crt_tmp --;
      }
    }
    crt[pos++] = (c&0xff) | 0x0700;  // black on white
  }

  if(pos < 0 || pos > 25*80)
    panic("pos under/overflow");

  if((pos/80) >= 24){  // Scroll up.
    memmove(crt, crt+80, sizeof(crt[0])*23*80);
    pos -= 80;
    memset(crt+pos, 0, sizeof(crt[0])*(24*80 - pos));
  }

  outb(CRTPORT, 14);
  outb(CRTPORT+1, pos>>8);
  outb(CRTPORT, 15);
  outb(CRTPORT+1, pos);
  if (f_em == 1) crt[pos] = ' ' | 0x0700;
}

void
consputc(int c)
{
  if(panicked){
    cli();
    for(;;)
      ;
  }
  if(c == BACKSPACE){
    uartputc('\b'); uartputc(' '); uartputc('\b');
  } else
    uartputc(c);
  cgaputc(c);
}



#define C(x)  ((x)-'@')  // Control-x

void 
set_pos(int pos){
  outb(CRTPORT, 14);
  outb(CRTPORT + 1, pos >> 8);
  outb(CRTPORT, 15);
  outb(CRTPORT + 1, pos);
  crt[pos] = ' ' | 0x0700;
}

void 
write_in_pos(int c, int pos){
  crt[pos] = (c&0xff) | 0x0700;
}

int 
get_pos(){
  int pos;
  outb(CRTPORT, 14);
  pos = inb(CRTPORT + 1) << 8;
  outb(CRTPORT, 15);
  pos |= inb(CRTPORT + 1);
  return pos;
}
void clear_line(){
  int pos = get_pos();
  pos += input.e - input.idx;
  set_pos(pos);
  input.idx = input.e;
  while(input.e != input.w &&
            input.buf[(input.e-1) % INPUT_BUF] != '\n'){
        consputc(BACKSPACE);
      }
}

void 
clear_all(){
  int pos = get_pos();
  while (pos){
    crt[pos--] = ' ' | 0x0700;
  }
}

void
consoleintr(int (*getc)(void))
{
  int c, doprocdump = 0;

  acquire(&cons.lock);
  while((c = getc()) >= 0){
    switch(c){
    case C('P'):  // Process listing.
      // procdump() locks cons.lock indirectly; invoke later
      doprocdump = 1;
      break;
    case C('U'):  // Kill line.
      clear_line();
      break;
    case C('H'): case '\x7f':  // Backspace
      if(input.e != input.w){
        consputc(BACKSPACE);
      }
      break;
    case C('L'):
      clear_line();
      clear_all();
      write_in_pos('$', 0);
      set_pos(2);
      break;
    case C('B'):
      if (input.idx > input.w){
        input.idx --;
        cgaputc(LEFTARROW);
      }
      break;
    case C('F'):
      if (input.idx < input.e){
        input.idx ++;
        cgaputc(RIGHTARROW);
      }
      break;
    case ARROWUP:
      if (input.com > 0){
        input.com--;
        memset(com[input.com + 1], '\0', sizeof(com[input.com + 1]));
        for (int i = input.w; i < input.e; i++){
          com[input.com + 1][i - (input.w)] = input.buf[i % INPUT_BUF];
        }
        clear_line();
        input.e = input.w;
        input.idx = input.e;
        int pnt = 0;
        while (com[input.com][pnt] != '\0' && com[input.com][pnt] != '\n' && com[input.com][pnt] != C('D')){
          input.buf[(pnt + input.w) % INPUT_BUF] = com[input.com][pnt];
          consputc(com[input.com][pnt]);
          pnt++;
          input.e ++;
          input.idx++;
        }
      }
      break;
    case ARROWDOWN:
      if (input.com < input.tot_com){
        input.com ++;
        memset(com[input.com - 1], '\0', sizeof(com[input.com - 1]));
        for (int i = input.w; i < input.e; i++){
          com[input.com - 1][i - (input.w)] = input.buf[i % INPUT_BUF];
        }
        clear_line();
        input.e = input.w;
        input.idx = input.e;
        int pnt = 0;
        while (com[input.com][pnt] != '\0' && com[input.com][pnt] != '\n' && com[input.com][pnt] != C('D')){
          input.buf[(pnt + input.w) % INPUT_BUF] = com[input.com][pnt];
          consputc(com[input.com][pnt]);
          pnt++;
          input.e ++;
          input.idx++;
        }
      }
      break;
    default:
      if(c != 0 && input.e-input.r < INPUT_BUF){
        c = (c == '\r') ? '\n' : c;
        consputc(c);
        if (c == '\n' || c == C('D')) input.buf[input.e % INPUT_BUF] = c;
        else input.buf[input.idx % INPUT_BUF] = c;
        input.e ++;
        input.idx ++;
        if(c == '\n' || c == C('D') || input.e == input.r+INPUT_BUF){
          memset(com[input.tot_com], '\0', sizeof(com[input.tot_com]));
          for (int i = input.w; i < input.e; i++){
            com[input.tot_com][i - (input.w)] = input.buf[i % INPUT_BUF];
          }
          input.tot_com = input.tot_com + 1;
          if (input.tot_com > 10) input.tot_com = 10;
          if (input.tot_com == 10){
            for (int i = 0; i < 10; i++){
              for (int j = 0; j < INPUT_BUF; j++){
                com[i][j] = com[i + 1][j];
              }
            }
          }
          input.com = input.tot_com;
          input.w = input.e;
          input.idx = input.e;
          wakeup(&input.r);
        }
      }
      break;
    }
  }
  release(&cons.lock);
  if(doprocdump) {
    procdump();  // now call procdump() wo. cons.lock held
  }
}

int
consoleread(struct inode *ip, char *dst, int n)
{
  uint target;
  int c;

  iunlock(ip);
  target = n;
  acquire(&cons.lock);
  while(n > 0){
    while(input.r == input.w){
      if(myproc()->killed){
        release(&cons.lock);
        ilock(ip);
        return -1;
      }
      sleep(&input.r, &cons.lock);
    }
    c = input.buf[input.r++ % INPUT_BUF];
    if(c == C('D')){  // EOF
      if(n < target){
        // Save ^D for next time, to make sure
        // caller gets a 0-byte result.
        input.r--;
      }
      break;
    }
    *dst++ = c;
    --n;
    if(c == '\n')
      break;
  }
  release(&cons.lock);
  ilock(ip);

  return target - n;
}

int
consolewrite(struct inode *ip, char *buf, int n)
{
  int i;

  iunlock(ip);
  acquire(&cons.lock);
  for(i = 0; i < n; i++){
    consputc(buf[i] & 0xff);
  }
  release(&cons.lock);
  ilock(ip);

  return n;
}

void
consoleinit(void)
{
  initlock(&cons.lock, "console");

  devsw[CONSOLE].write = consolewrite;
  devsw[CONSOLE].read = consoleread;
  cons.locking = 1;

  ioapicenable(IRQ_KBD, 0);
}

