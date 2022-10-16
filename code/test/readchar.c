/* readchar.c
 *	
 *	Thuc hien system call ReadChar doc 1 ky tu tu console
 *	
 */

#include "syscall.h"

int
main()
{
  char result;
  
  result = ReadChar();
  Halt();
  /* not reached */
}
