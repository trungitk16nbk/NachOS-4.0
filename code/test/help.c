/* help.c
 *	Simple program to test whether the systemcall interface works.
 *
 *	In ra console cac thong tin cua nhom va mo ta chuc nang cung nhu 
 *  cach su dung cua chuong trinh sort va chuong trinh ascii
 */

#include "syscall.h"

int main()
{
    PrintString("- Mo ta ve nhom:\n");
    PrintString("\tNhom gom 3 thanh vien la: \n");
    PrintString("\t\t+ Bui Quoc Trung - 20120023\n");
    PrintString("\t\t+ Du Quoc Huy - 20120101\n");
    PrintString("\t\t+ Nguyen Kong Dai - 20120448\n");
    PrintString("- Mo ta ve chuong trinh sort:\n");
    PrintString("\tCho phep nguoi dung nhap vao 1 mang so nguyen va yeu cau sap xep tang hoac giam\n");
    PrintString("\tChuong trinh se thuc hien sap xep va ghi ket qua ra man hinh\n");
    PrintString("- Mo ta ve chuong trinh ascii\n");
    PrintString("\tChuong trinh se in ra man hinh bang ma ascii voi nhung ki tu doc duoc\n\n");

    Halt();
    /* not reached */
}