// exception.cc
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "main.h"
#include "syscall.h"
#include "ksyscall.h"
//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2.
//
// If you are handling a system call, don't forget to increment the pc
// before returning. (Or else you'll loop making the same system call forever!)
//
//	"which" is the kind of exception.  The list of possible exceptions
//	is in machine.h.
//----------------------------------------------------------------------

void ExceptionHandler(ExceptionType which)
{
	int type = kernel->machine->ReadRegister(2);

	DEBUG(dbgSys, "Received Exception " << which << " type: " << type << "\n");

	switch (which)
	{
	case NoException:
		return;
	
	case SyscallException:
		DEBUG(dbgSys, "Begin SyscallException\n");

		switch (type)
		{
		case SC_Halt:
			DEBUG(dbgSys, "Shutdown, initiated by user program.\n");

			SysHalt();

			ASSERTNOTREACHED();
			break;

		case SC_Add:
			DEBUG(dbgSys, "Add " << kernel->machine->ReadRegister(4) << " + " << kernel->machine->ReadRegister(5) << "\n");

			/* Process SysAdd Systemcall*/
			int result;
			result = SysAdd(/* int op1 */ (int)kernel->machine->ReadRegister(4),
							/* int op2 */ (int)kernel->machine->ReadRegister(5));

			DEBUG(dbgSys, "Add returning with " << result << "\n");
			/* Prepare Result */
			kernel->machine->WriteRegister(2, (int)result);

			IncreasePC();

			return;

			ASSERTNOTREACHED();

			break;
		

		// System call ReadNum. Doc so nguyen tu console
		case SC_ReadNum:
			DEBUG(dbgSys, "Begin ReadNum" << "\n");
			char ch;
			char* buffer;
			int size;
			size = 0;
			buffer = new char[MAX_BUFFER + 1];
			int number;
			number = 0; // so nguyen tra ve
			bool check;
			check = true;

			// doc 1 chuoi ki tu vao
			// enter de ngat
			while (true) 
			{
				ch = SysReadChar();
				if ( (ch == '\012') || (ch == '\001') )
				{
					break;
				}
				buffer[size] = ch;
				size++;	
			}

			// Cac truong hop so:
				// -123
				// 123
				// 123.000
				// > maxint
				// < minint
			bool isNegative;	// kiem tra so am
			isNegative = false; 
			int beginNum;	// vi tri bat dau so
			beginNum = 0;
			int endNum;		// vi tri ket thuc so
			endNum = 0;
			bool haveDot;	// check dau cham .
			haveDot = false;

			// kiem tra so am, tim vi tri bat dau va ket thuc so
			if (buffer[0] == '-') {
				isNegative = true; // so am
				beginNum = 1;
				endNum = 1;
			}

			// bat dau kiem tra tung ki tu
			// neu khong dung dang so nguyen -> bao loi -> tra ket qua thanh 0 -> tang thanh ghi PC -> ket thuc
			for (int i = beginNum; i < size; i++) 
			{
				if (buffer[i] == '.') // 123.0000 van la so
				{
					if (haveDot == true)	// da co 1 dau . (123.000.000 khong phai so nguyen)
					{
						//  khong phai so
						DEBUG(dbgSys, "The integer number is not valid" << "\n");
						kernel->machine->WriteRegister(2, 0);
						IncreasePC();
						delete buffer;
						return;
					}
					haveDot = true;
					// kiem tra co phai so nguyen khong (123.999 khong phai so nguyen)
					for (int j = i + 1; j < size; j++)
					{
						if (buffer[j] != '0')
						{
							// khong phai so
							DEBUG(dbgSys, "The integer number is not valid" << "\n");
							kernel->machine->WriteRegister(2, 0);
							IncreasePC();
							delete buffer;
							return;
						}
					}
					// thoa dang 123.000
					endNum = i - 1;		// dung truoc dau .
					break;
				}
				// kiem tra cac ki tu khac
				else if (buffer[i] < '0' || buffer[i] > '9')
				{
					// khong phai dang so 0 -> 9
					DEBUG(dbgSys, "The integer number is not valid" << "\n");
					kernel->machine->WriteRegister(2, 0);
					IncreasePC();
					delete buffer;
					return;
				}
				endNum = i;
			}

			// Chac chan la so -> su dung atoi
			number = atoi(buffer);

			DEBUG(dbgSys, "Number is: " << number << "\n");

			// tra ket qua ve thanh ghi so 2
			kernel->machine->WriteRegister(2, (int)number);
			// tang gia tri thanh ghi PC (tranh vong lap)
			IncreasePC();
			delete buffer;
			return;

			ASSERTNOTREACHED();
			break;
			// mot so truong hop dac biet
			// number > max_int -> number = max_int
			// number < min_int -> number = min_int

		// System call ReadNum. Ghi so nguyen ra console
		case SC_PrintNum:
			// nhan number tu thanh ghi so 4
			number = kernel->machine->ReadRegister(4);
			DEBUG(dbgSys, "Number: " << number << "\n");

			int numberTemp;

			isNegative = false;

			// kiem tra number co am khong
			if (number < 0) 
			{
				SysPrintChar('-');
				isNegative = true;
			}

			// tach tung ki tu (int -> char[])
			buffer = new char[MAX_BUFFER + 1];
			size = 0;

			// truong hop number chi la 0
			if (number == 0) 
			{
				size = 1;
				buffer[0] = (char)(0 + 48);
			}

			// neu khac 0 -> tach tung ki tu
			// 123 -> buffer{3, 2, 1}
			while (number != 0) 
			{
				numberTemp = (isNegative)?((number % 10) * -1):(number % 10);
				buffer[size] = (char)(numberTemp + 48);
				number = number / 10;
				size++;
			}

			// bat dau ghi ra console
			for (int i = size - 1; i >= 0; i--)
			{
				SysPrintChar(buffer[i]);
			}

			delete buffer;

			DEBUG(dbgSys, "\n");
			IncreasePC();

			return;

			ASSERTNOTREACHED();
			break;

		//System call ReadChar. Doc 1 ki tu tu console
		case SC_ReadChar:
		{
			DEBUG(dbgSys, "Read a character from console\n");
			char res;
			res = SysReadChar();
			// ghi ket qua vao thanh ghi so 2
			kernel->machine->WriteRegister(2, (int)res);
			DEBUG(dbgSys, "The character is " << res << "\n");

			IncreasePC();
			return;
			ASSERTNOTREACHED();
			break;
		}

		//System call PrintChar. Ghi ki tu ra console
		case SC_PrintChar:
		{
			DEBUG(dbgSys, "Print a character to console\n");
			int val;
			
			// doc ki tu tu thanh ghi 4
			val = kernel->machine->ReadRegister(4);
			ch = char(val);
			SysPrintChar(ch);

			IncreasePC();
			return;
			ASSERTNOTREACHED();
			break;
		}

		// System call RandomNum. Lay ra 1 so ngau nhien interger
		case SC_RandomNum:
		{
			DEBUG(dbgSys, "Random a number\n");

			int num;
			// su dung ham random
			num = rand();

			// ghi so nguyen vao thanh ghi 2
			kernel->machine->WriteRegister(2, num);
			DEBUG(dbgSys, "The number have just been random is " << num << "\n");

			IncreasePC();
			return;
			ASSERTNOTREACHED();
			break;
		}

		//System call ReadString. Doc 1 chuoi ki tu tu console
		case SC_ReadString:
			int virtAddr;	// dia chi de doc
			int length; 	// do dai toi da
			virtAddr = kernel->machine->ReadRegister(4);
			length = kernel->machine->ReadRegister(5);

			// chuoi duoc luu o vung nho thuoc Userspace
			// chuyen du lieu tu Userspace -> Kernelspace
			buffer = User2System(virtAddr, length);

			size = 0; // do dai that
			// bat dau doc chuoi tu console
			while (true) 
			{
				ch = SysReadChar();
				if ( (ch == '\012') || (ch == '\001') )
				{
					break;
				}
				buffer[size] = ch;
				size++;	
			}
			
			DEBUG(dbgSys, "String: " << buffer);

			// chuyen chuoi doc duoc ve lai Userspace
			System2User(virtAddr, size, buffer);
			delete buffer;

			IncreasePC();

			return;

			ASSERTNOTREACHED();
			break;
		
		// System call PrintString. ghi chuoi ki tu ra console console
		case SC_PrintString:
			// doc dia chi cua chuoi tu thanh ghi 4
			virtAddr = kernel->machine->ReadRegister(4);
			// chuyen chuoi tu vung nho thuoc Userspace -> Kernelspace
			buffer = User2System(virtAddr, 255);
			
			// tim do dai chuoi
			size = 1;
			while (buffer[size-1] != '\0')
				size++;

			DEBUG(dbgSys, "Length buffer " << size);

			// ghi chuoi ra console
			for (int i = 0; i < size-1; i++) 
			{
				SysPrintChar(buffer[i]);
			}

			SysPrintChar('\n');
			delete buffer;
			IncreasePC();

			return;

			ASSERTNOTREACHED();
			break;

		default:
			cerr << "Unexpected system call " << type << "\n";
			break;
		}
		break;

/*---------------------------------------end code------------------------------------------------*/

	case PageFaultException:
		cerr << "No valid translation found "
			 << "\n";
		SysHalt();
		return;
		break;

	case ReadOnlyException:
		cerr << "Write attempted to page marked \"read-only\" "
			 << "\n";
		SysHalt();
		return;
		break;

	case BusErrorException:
		cerr << "Translation resulted in an invalid physical address "
			 << "\n";
		SysHalt();
		return;
		break;

	case AddressErrorException:
		cerr << "Unaligned reference or one that was beyond the end of the address space "
			 << "\n";
		SysHalt();
		return;
		break;

	case OverflowException:
		cerr << "Integer overflow in add or sub "
			 << "\n";
		SysHalt();
		return;
		break;

	case IllegalInstrException:
		cerr << "Unimplemented or reserved instr "
			 << "\n";
		SysHalt();
		return;
		break;

	case NumExceptionTypes:
		cerr << "Number Exception Types "
			 << "\n";
		SysHalt();
		return;
		break;

	default:
		cerr << "Unexpected user mode exception" << (int)which << "\n";
		break;
	}
	ASSERTNOTREACHED();
}
