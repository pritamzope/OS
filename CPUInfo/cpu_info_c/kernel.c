#include "kernel.h"

//index for video buffer array
uint32 vga_index;
//counter to store new lines
static uint32 next_line_index = 1;
//fore & back color values
uint8 g_fore_color = WHITE, g_back_color = BLUE;
//digit ascii code for printing integers
int digit_ascii_codes[10] = {0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39};

/*
16 bit video buffer elements(register ax)
8 bits(ah) higher : 
  lower 4 bits - forec olor
  higher 4 bits - back color

8 bits(al) lower :
  8 bits : ASCII character to print
*/
uint16 vga_entry(unsigned char ch, uint8 fore_color, uint8 back_color) 
{
  uint16 ax = 0;
  uint8 ah = 0, al = 0;

  ah = back_color;
  ah <<= 4;
  ah |= fore_color;
  ax = ah;
  ax <<= 8;
  al = ch;
  ax |= al;

  return ax;
}

//clear video buffer array
void clear_vga_buffer(uint16 **buffer, uint8 fore_color, uint8 back_color)
{
  uint32 i;
  for(i = 0; i < BUFSIZE; i++){
    (*buffer)[i] = vga_entry(NULL, fore_color, back_color);
  }
  next_line_index = 1;
  vga_index = 0;
}

//initialize vga buffer
void init_vga(uint8 fore_color, uint8 back_color)
{
  vga_buffer = (uint16*)VGA_ADDRESS;
  clear_vga_buffer(&vga_buffer, fore_color, back_color);
  g_fore_color = fore_color;
  g_back_color = back_color;
}

/*
increase vga_index by width of row(80)
*/
void print_new_line()
{
  if(next_line_index >= 55){
    next_line_index = 0;
    clear_vga_buffer(&vga_buffer, g_fore_color, g_back_color);
  }
  vga_index = 80*next_line_index;
  next_line_index++;
}

//assign ascii character to video buffer
void print_char(char ch)
{
  if(ch == '\n'){
    print_new_line();
  }else if(ch == '\t'){
    //2 spaces for horizontal tab(9)
    vga_buffer[vga_index] = vga_entry(9, g_back_color, g_back_color);
    vga_index++;
    vga_buffer[vga_index] = vga_entry(9, g_back_color, g_back_color);
    vga_index++;
  }else{
    vga_buffer[vga_index] = vga_entry(ch, g_fore_color, g_back_color);
    vga_index++;
  }
}


uint32 strlen(const char* str)
{
  uint32 length = 0;
  while(str[length])
    length++;
  return length;
}

uint32 digit_count(int num)
{
  uint32 count = 0;
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
  int dgcount = digit_count(num);
  int index = dgcount - 1;
  char x;
  if(num == 0 && dgcount == 1){
    number[0] = '0';
    number[1] = '\0';
  }else{
    while(num != 0){
      x = num % 10;
      number[index] = x + '0';
      index--;
      num = num / 10;
    }
    number[dgcount] = '\0';
  }
}

//print string by calling print_char
void print_string(char *str)
{
  uint32 index = 0;
  while(str[index]){
    print_char(str[index]);
    index++;
  }
}

//print int by converting it into string
//& then printing string
void print_int(int num)
{
  char str_num[digit_count(num)+1];
  itoa(num, str_num);
  print_string(str_num);
}

void print_binary(uint32 num)
{
  char bin_arr[32];
  uint32 index = 31;
  uint32 i;
  while (num > 0){
    if(num & 1){
      bin_arr[index] = '1';
    }else{
      bin_arr[index] = '0';
    }
    index--;
    num >>= 1;
  }

  for(i = 0; i < 32; ++i){
   if(i <= index)
      print_char('0');
   else
     print_char(bin_arr[i]);
  }
}

void cpuid(uint32 value, uint32 *eax, uint32 *ebx, unsigned int *ecx, uint32 *edx)
{
  uint32 eaxres, ebxres, ecxres, edxres;
  asm("xorl\t%eax, %eax");
  asm("xorl\t%ebx, %ebx");
  asm("xorl\t%ecx, %ecx");
  asm("xorl\t%edx, %edx");
  asm("movl\t%0, %%eax":"=m"(value));
  asm("cpuid");
  asm("movl\t%%eax, %0" : "=m" (eaxres));
  asm("movl\t%%ebx, %0" : "=m" (ebxres));
  asm("movl\t%%ecx, %0" : "=m" (ecxres));
  asm("movl\t%%edx, %0" : "=m" (edxres));
  *eax = eaxres;
  *ebx = ebxres;
  *ecx = ecxres;
  *edx = edxres;    
}

void print_eax(uint32 eax)
{
  uint32 step_id, model, family_id, proc_type, ext_mod_id, ext_fam_id;
  step_id = model = family_id = proc_type = ext_mod_id = ext_fam_id = eax;
  
  step_id &= (2 << 3) - 1;	//bits 0-3
  model >>= 4;	//bits 4-7
  model &= (2 << 3) - 1;
  family_id >>= 8;	//bits 8-11
  family_id &= (2 << 3) - 1;
  proc_type >>= 12;	//bits 12-13
  proc_type &= (2 << 1) - 1;
  ext_mod_id >>= 16;	//bits 16-19
  ext_mod_id &= (2 << 3) - 1;
  ext_fam_id >>= 20;	//bits 20-27
  ext_fam_id &= (2 << 7) - 1;

  print_string("\nEAX :-");
  print_string("\n\tStepping ID: ");
  print_int(step_id);
  print_string("\n\tModel: ");
  print_int(model);
  print_string("\n\tFamily ID: ");
  print_int(family_id);
  print_string("\n\tProcessor Type: ");
  print_int(proc_type);
  print_string("\n\tExtended Model ID: ");
  print_int(ext_mod_id);
  print_string("\n\tExtended Family ID: ");
  print_int(ext_fam_id);
}

void print_ebx(uint32 ebx)
{
  uint32 brand_index, cache_line_size, max_addr_id, init_apic_id;
  brand_index = cache_line_size = max_addr_id = init_apic_id = 0;
  char *bytes = (char*)&ebx;

  brand_index = bytes[0];	//bits 0-7
  cache_line_size = bytes[1];	//bits 8-15
  max_addr_id = bytes[2];	//bits 16-23
  init_apic_id = bytes[3];	//bits 24-31

  print_string("\nEBX :-");
  print_string("\n\tBrand Index: ");
  print_int(brand_index);
  print_string("\n\tCache Line Size: ");
  print_int(cache_line_size);
  print_string("\n\tMax Addressable ID for Logical Processors: ");
  print_int(max_addr_id);
  print_string("\n\tInitial APIC ID: ");
  print_int(init_apic_id);
}

void print_edx(uint32 edx)
{
  print_string("\nEDX :-");
  print_string("\n\tbit-31 [ ");
  print_binary(edx);
  print_string(" ] bit-0");
  print_string("\n\tBit 0 : FPU-x87 FPU on Chip");
  print_string("\n\tBit 1 : VME-Virtual-8086 Mode Enhancement");
  print_string("\n\tBit 2 : DE-Debugging Extensions");
  print_string("\n\tBit 3 : PSE-Page Size Extensions");
  print_string("\n\tBit 4 : TSC-Time Stamp Counter");
  print_string("\n\tBit 5 : MSR-RDMSR and WRMSR Support");
  print_string("\n\tBit 6 : PAE-Physical Address Extensions");
}

void cpuid_test()
{
  uint32 eax, ebx, ecx, edx;

  print_string("CPUID EAX = 01H");

  //see cpuid instruction in Intel Manual for more information
  cpuid(0x01, &eax, &ebx, &ecx, &edx);

  print_eax(eax);
  print_ebx(ebx);
  print_edx(edx);
}

void kernel_entry()
{
  //first init vga with fore & back colors
  init_vga(WHITE, BLUE);

  cpuid_test();
}


