/* sort.c
 *    Allows the user to input an array of n integers where n is the number entered by 
      the user (n <=100)
 *    
 *    Use bubble sort algorithm to sort the above array in ascending or descending order depending on 
      the user's choice
 * 
 *    Test program to sort a large number of integers.
 *
 *    Intention is to stress virtual memory system.
 *
 *    Ideally, we could read the unsorted array off of the file system,
 *	and store the result back to the file system!
 */

#include "syscall.h"

int a[1024]; /* size of physical memory; with code, we'll run out of space!*/
// char st[4];

// so sanh lon hon
int asc(int x, int y)
{
    return x > y;
}

// so sanh nho hon
int desc(int x, int y)
{
    return x < y;
}

// in mang 
void PrintArray(int arr[], int size)
{
    int k;
    PrintString("Mang sau khi duoc sap xep: ");
    for (k = 0; k < size; ++k)
    {
        PrintNum(a[k]);
        PrintChar(' ');
    }
}

// sap xep noi bot dung con tro ham
void Sort(int arr[], int size, int (*comparisionFunc)(int, int))
{
    int i, j, temp;
    for (i = 0; i < size - 1; ++i)
    {
        for (j = i + 1; j < size; ++j)
            if (comparisionFunc(arr[i], arr[j]) == 1) // con tro ham so sanh
            {
                // hoan doi 2 phan tu
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
    }
}

int main()
{
    int n, i, check;
    // luu y: nhap so khong co khoang trang 
    PrintString("Nhap so luong phan tu mang: ");
    n = ReadNum();

    // nhap moi so tren 1 hang va khong co khoang trang
    PrintString("Nhap cac phan tu  mang:");
    for (i = 0; i < n; ++i)
    {
        a[i] = ReadNum();
    }

    PrintString("Nhap lua chon sap xep cua ban (1: tang, 2: giam): ");
    check = ReadNum();
    
    while (check != 1 && check != 2){
        PrintString("Vui long nhap lai lua chon sap xep cua ban (1: tang, 2: giam): ");
        check = ReadNum();
    }
    if (check == 1)
    {
        Sort(a, n, asc);
        PrintArray(a, n);
    }
    else if (/* condition */ check == 2)
    {
        /* code */
        Sort(a, n, desc);
        PrintArray(a, n);
    }
    // else
    // {
    //     PrintString("Errol!");
    // }

    PrintChar('\n');
    Exit(a[0]); /* and then we're done -- should be 0! */
}