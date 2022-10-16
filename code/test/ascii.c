/* ascii.c
 *	Simple program to test whether the systemcall interface works.
 *
 *	Just do a add syscall that: print the num and its ascii code (Printable ASCII characters)
 *
 */

#include "syscall.h"

int main()
{
    int i;
    // ky tu co ma doc duoc la tu 32 den 126
    for (i = 32; i < 127; i++)
    {
        PrintNum(i); // in so
        PrintChar((char)(32)); // in khoang cach
        PrintChar((char)(i)); // in ma ascii
        PrintChar('\n'); // in xuong dong
    }

    Halt();
    /* not reached */
}
