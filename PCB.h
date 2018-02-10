#pragma once
#ifndef PCB_H
#define PCB_H

#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

struct PCB
{
	int PID;
	int *pageTable;
	int pageTableSize;
	PCB *next;

	PCB() : PID(0)
	{
	}

	void printPageTable()
	{
		int x = pageTableSize / 6;
		for (int i = 0; i < x; i++)
		{
			for (int j = 0; j < pageTableSize; j++)
			{
				if (j % x == i)
				{
					cout << setw(4) << left << j << "| " << *(pageTable + j) << "\t";
				}
			}
			cout << "\n";
		}
	}

};

class readyQueue
{
	private:
		PCB *head, *tail;
	public:
		readyQueue();
		void push(PCB *x);
		void printQueue();
		void searchAndDestroy(int searchPID, int *fBlocks, bool *arr);
		void deleteQueue(int *fBlocks, bool *arr);
		bool isEmpty();
		bool pidInList(int x);
};

#endif