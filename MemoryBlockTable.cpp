// MemoryBlockTable.cpp : Defines the entry point for the console application.

#include "PCB.h"
#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

void menu();

struct MBT
{
	int freeBlocks;	// # of blocks labeled "true" in the array
	bool boolArray[1024];
};

int main()
{
	/*
		Initialize MBT values. 
	*/
	MBT myMBT;
	myMBT.freeBlocks = 992; // (1024 - 32) blocks free
	for (int i = 0; i < 32; i++) // blocks 0-31 blocks occupied by OS = FALSE
		myMBT.boolArray[i] = false;
	for (int i = 32; i < 1024; i++) // blocks 32-1023 free for process use = TRUE
		myMBT.boolArray[i] = true;

	/*
		Initialize ready queue.
	*/
	readyQueue readyqueue;

	/*
		All these variables are here because Visual Studio screamed at me when I put them inside the switch statements
		which makes sense since you wouldn't want to initialize them every time you go through the loop but this looks ugly :(
	*/
	PCB *temp;
	int myPID;
	int blocksReq;
	int *myPageTable;
	int findPID;
	bool *boolPtr = myMBT.boolArray;
	char exitProgram;
	int x = 1024/16;

	/*
		Menu interactions.
	*/	
	bool exit = false;
	int choice = 0;

	while (!exit)
	{
		menu();
		cin >> choice;

		switch (choice)
		{
			case 1: 
				/*
					Create PCB.
				*/
				cout << "Creating process...\n";
				temp = new PCB;

				/*
					Let user enter unique PID for PCB.
				*/
				cout << "Enter a unique PID for this process.\n";
				cin >> myPID;
				while (readyqueue.pidInList(myPID))
				{
					cout << "\nPID already exists in queue. Enter another: \n";
					cin >> myPID;
				}
				temp->PID = myPID;

				/*
					Random int from 10-250 generated for number of blocks required by process.
				*/
				blocksReq = rand() % 241 + 10;
				cout << "This process will require " << blocksReq << " blocks in the MBT.\n";
				if (blocksReq > myMBT.freeBlocks)
				{
					cout << "There is not enough space in the MBT. Cannot add process to queue.\n";
					break;
				}
				else
				{
					temp->pageTableSize = blocksReq;

					/*
					Create page table that represents the location of each process block in the MBT.
					*/
					myPageTable = new int[blocksReq];
					temp->pageTable = myPageTable;

					/*
					Iterate through MBT to find free (TRUE) blocks, and record index into page table.
					*/

					for (int i = 0; i < blocksReq; i++)
					{
						for (int j = 0; j < 1024; j++)
						{
							if (myMBT.boolArray[j] == true)
							{
								temp->pageTable[i] = j;
								myMBT.boolArray[j] = false; // Set corresponding MBT block to occupied (false).
								break;
							}
						}
					}

					myMBT.freeBlocks -= blocksReq;
					cout << "There are now " << myMBT.freeBlocks << " free blocks left.\n";

					/*
					Push PCB into readyqueue.
					*/
					readyqueue.push(temp);

					/*
					Output page table and MBT for verification.
					*/
					cout << "Printing page table...\n";
					temp->printPageTable();
					cout << "\n";

					/*
					WARNING: HUGE AND UGLY. Also I'm too lazy to put this in another method. Sorry
					*/
					cout << "Printing MBT...\n";
					for (int i = 0; i < x; i++)
					{
						for (int j = 0; j < 1024; j++)
						{
							if (j % x == i)
							{
								if (myMBT.boolArray[j] == true)
								{
									cout << setw(4) << left << j << "| " << "T" << "   ";
								}
								else
								{
									cout << setw(4) << left << j << "| " << "F" << "   ";
								}
							}
						}
						cout << "\n";
					}
				}

				break;

			case 2:
				/*
					Prints the PIDs in the ready queue.
				*/
				if (readyqueue.isEmpty())
					cout << "\nReady queue is empty.\n";
				else
				{
					cout << "\nPrinting PIDS...\n";
					readyqueue.printQueue();
				}
				break;

			case 3:
				/*
					User enters PID of PCB to be deleted. 
					If no PCB with that PID exists, nothing happens.
					If PCB with matching PID found, MBT blocks deallocated,
					and page table and PCB are deleted.
				*/
				cout << "\nEnter the PID of the PCB to be deleted.\n";
				cin >> findPID;
				readyqueue.searchAndDestroy(findPID, &(myMBT.freeBlocks), boolPtr);
				//cout << myMBT.freeBlocks;
				break;

			case 4:
				if (readyqueue.isEmpty())
					cout << "\nReady queue is empty.\n";
				else
				{
					cout << "\nPrinting PIDS...\n";
					readyqueue.printQueue();
				}
				cout << "\nPrinting MBT...\n";
				for (int i = 0; i < x; i++)
				{
					for (int j = 0; j < 1024; j++)
					{
						if (j % x == i)
						{
							if (myMBT.boolArray[j] == true)
							{
								cout << setw(4) << left << j << "| " << "T" << "   ";
							}
							else
							{
								cout << setw(4) << left << j << "| " << "F" << "   ";
							}
						}
					}
					cout << "\n";
				}

				/*
					If ready queue is not empty, user must confirm that they want to exit program.
				*/
				if (!readyqueue.isEmpty())
				{
					cout << "\nThe queue is not empty. Are you sure you want to exit? (Y/N)\n";
					cin >> exitProgram;
					if (exitProgram == 'Y' || exitProgram == 'y')
					{
						cout << "\nDeleting all dynamically allocated memory...\n";
						readyqueue.deleteQueue(&(myMBT.freeBlocks), boolPtr);

						//readyqueue.printQueue();

						system("pause");
						exit = true;
						break;
					}
					else
					{
						break;
					}
				}
				system("pause");
				exit = true;
				break;
			default:
				cout << "\nInvalid input. Enter again\n";
				break;
		}
	}
    return 0;
}

void menu()
{
	cout << "\n--------------------------------------\n";
	cout << "1. Initiate a process\n";
	cout << "2. Print all processes in ready queue\n";
	cout << "3. Terminate process with certain PID\n";
	cout << "4. Exit\n";
	cout << "--------------------------------------\n";
}
