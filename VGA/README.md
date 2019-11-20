<h4>Kernel :</h4><br/>
<img src="https://raw.githubusercontent.com/pritamzope/OS/master/VGA/kernel_c/kernel_vga_output.png"/>
<br/>
<b>Reference:</b><a href="https://github.com/pritamzope/OS/blob/master/VGA/kernel_c/Reference/Intel%20OpenSource%20HD%20Graphics.pdf">https://github.com/pritamzope/OS/blob/master/VGA/kernel_c/Reference/Intel%20OpenSource%20HD%20Graphics.pdf</a>

<h4>Asm :</h4><br/>
<b>vga_hello.asm : </b> This source is same as we did in Kernel.c source. It points to VGA addreess 0xB800, with colors 4 bits, and 80*20 resolution font.

<b>color_change.asm : </b> Background colors changes as any key is pressed.

<b>rotate_text.asm : </b> Rotate the text to the right position with changing fore and back color by incrementing VGA index as any key is pressed.


