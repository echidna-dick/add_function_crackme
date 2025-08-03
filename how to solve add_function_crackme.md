first compile the code using gcc 'gcc -g cmd_args.c -o cmd_args'
run it using 'gdb ./cmd_args'

in GDB:
make a breakpoint 'b main'
run it 'r'

lets disassble it 'disas main'
you probely will see something like this:

   0x000000000040050e <+0>:	push   %rbp
   0x000000000040050f <+1>:	mov    %rsp,%rbp
   0x0000000000400512 <+4>:	sub    $0x10,%rsp
=> 0x0000000000400516 <+8>:	mov    %edi,-0x4(%rbp)         // our 'argc' in C, moved to a local variable.
   0x0000000000400519 <+11>:	mov    %rsi,-0x10(%rbp)        // our 'argv' in C, moved to a local variable.
   0x000000000040051d <+15>:	cmpl   $0x1,-0x4(%rbp)         // compare 1 to 'argc'.
   0x0000000000400521 <+19>:	jne    0x400539 <main+43>      // jump if not equal to -> <main+43> if argc != 1.
   0x0000000000400523 <+21>:	mov    $0x40061e,%edi
   0x0000000000400528 <+26>:	call   0x400410 <puts@plt>
   0x000000000040052d <+31>:	mov    $0x0,%eax
   0x0000000000400532 <+36>:	jmp    0x40054a <main+60>
   0x0000000000400534 <+38>:	mov    $0x0,%eax
   0x0000000000400539 <+43>:	mov    -0x10(%rbp),%rax        // now we are here, getting our 'argv' from stack
   0x000000000040053d <+47>:	add    $0x8,%rax               // add 8 bytes to get the first argument
   0x0000000000400541 <+51>:	mov    (%rax),%rax             // get the argument itself
   0x0000000000400544 <+54>:	mov    %rax,%rdi
   0x0000000000400547 <+57>:	call   0x400400 <puts@plt>
   0x000000000040054c <+62>:	mov    $0x0,%eax
   0x0000000000400551 <+67>:	leave
   0x0000000000400552 <+68>:	ret

                                  ASM description

first we have a new main function with argc and argv as arguments.
Our argc is passed in %edi and argv in %rsi.
You can see it in mov %edi,-0x4(%rbp) for argc and mov %rsi,-0x10(%rbp) for argv.

Now lets cover the if statement:
cmpl $0x1,-0x4(%rbp) checks if our argc is equal to 1. If it is, jne will not jump.
If argc is more than 1, we jump to 0x400539 <main+43>.

The if statement is checking for command-line arguments. If we just run ./cmd_args we have only one argument, which is the program name itself, so argc is 1.

The else part of the code starts at <main+43>:
mov -0x10(%rbp),%rax moves argv into %rax.
add $0x8,%rax moves the pointer to the first actual argument because argv[0] is the program name, and char* is 8 bytes on a 64-bit system.
Then mov (%rax),%rax gets the content of the argument, and puts prints it.

                                    HOW TO CRACK

We will try to make the program print our argument even if we don't provide one.
The key is to bypass the if statement. The program checks if argc is 1. If it is, it prints "No arguments provided.". We want to trick it into thinking argc is not 

Set a breakpoint on the compare instruction: b *0x40051d

Run the program without any arguments: r

The program will stop. Check the argc value in %edi: info reg edi

Change argc to a value greater than 1, so the jne instruction will jump. For example, change it to 2.

set $edi = 2

Continue the program: c

What will happen? Will it crash or will it print something? You have to find out >:)
