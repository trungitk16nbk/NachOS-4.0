/* readnum.c
 *	
 *	Thuc hien syscall ReadNum doc mot so nguyen tu console
 *	
 */

#include "syscall.h"

int
main()
{
  int num = ReadNum();
  Halt();
  /* not reached */
}