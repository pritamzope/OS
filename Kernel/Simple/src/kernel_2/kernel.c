#include"kernel.h"

static int Y_INDEX = 1;

static UINT16 VGA_DefaultEntry(unsigned char to_print) {
	return (UINT16) to_print | (UINT16)WHITE_COLOR << 8;
}


void Clear_VGA_Buffer(UINT16 **buffer)
{
  for(int i=0;i<BUFSIZE;i++){
    (*buffer)[i] = '\0';
  }
  Y_INDEX = 1;
  VGA_INDEX = 0;
}

void InitTerminal()
{
  TERMINAL_BUFFER = (UINT16*) VGA_ADDRESS;
  Clear_VGA_Buffer(&TERMINAL_BUFFER);
}


int digitCount(int num)
{
  int count = 0;
  if(num == 0)
    return 1;
  while(num > 0){
    count++;
    num = num/10;
  }
  return count;
}


void itoa(int num, char *number)
{
  int digit_count = digitCount(num);
  int index = digit_count - 1;
  char x;
  if(num == 0 && digit_count == 1){
    number[0] = '0';
    number[1] = '\0';
  }else{
    while(num != 0){
      x = num % 10;
      number[index] = x + '0';
      index--;
      num = num / 10;
    }
    number[digit_count] = '\0';
  }
}


void printNewLine()
{
  if(Y_INDEX >= 55){
    Y_INDEX = 0;
    Clear_VGA_Buffer(&TERMINAL_BUFFER);
  }
  VGA_INDEX = 80*Y_INDEX;
  Y_INDEX++;
}


void printString(char *str)
{
  int index = 0;
  while(str[index]){
    TERMINAL_BUFFER[VGA_INDEX] = VGA_DefaultEntry(str[index]);
    index++;
    VGA_INDEX++;
  }
}

void printInt(int num)
{
  char str_num[digitCount(num)+1];
  itoa(num, str_num);
  printString(str_num);
}


void KERNEL_MAIN()
{
  TERMINAL_BUFFER = (UINT16*) VGA_ADDRESS;
  printString("Hello World!");
  printNewLine();
  printInt(1234567890);
  printNewLine();
  printString("GoodBye World!");
}
