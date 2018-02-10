#include "PCB.h"
#include <iostream>

readyQueue::readyQueue()
{
	head = NULL;
	tail = NULL;
}

/*
	PCB nodes will only ever be added to the end of a ready queue.
	No need to worry about insertion in the middle of a ready queue.
*/

void readyQueue::push(PCB *x)
{
	PCB *temp = x;
	temp->next = NULL;

	if (head == NULL)
	{
		head = temp;
		tail = temp;
		temp = NULL;
	}
	else
	{
		tail->next = temp;
		tail = temp;
		tail->next = NULL;
	}

}

/*
	User inputs PID of PCB to be removed from readyqueue.
	If no PCB has matching PID, nothing occurs.
	I'm not sure how this works but it does and that's that.
*/

void readyQueue::searchAndDestroy(int searchPID, int *fBlocks, bool *arr)
{
	PCB *temp = head;
	PCB *prev = head;

	while (temp != NULL)
	{
		if (temp->PID == searchPID)
		{
			cout << "\nProcess found. Deallocating memory blocks...\n";
			*fBlocks += temp->pageTableSize;
			
			for (int i = 0; i < temp->pageTableSize; i++)
			{
				arr[temp->pageTable[i]] = true;
			}

			delete temp->pageTable;

			if (temp == head)
			{
				head = head->next;
			}
			else
			{
				prev->next = temp->next;
			}
			delete temp;			
			break;
		}
		prev = temp;
		temp = temp->next;
	}

	if (temp == NULL) // PID not found
		cout << "\nNo such PCB with that PID exists.\n";
}

/*
	Clears ready queue and reallocates all memory back to MBT.
	This is not actually working. Why??? I am using the code from above which works...
*/

void readyQueue::deleteQueue(int *fBlocks, bool *arr)
{
	/*
	PCB *temp = head;
	PCB *curr = head;
	while (temp != NULL)
	{
		*fBlocks += temp->pageTableSize;

		for (int i = 0; i < temp->pageTableSize; i++)
		{
			arr[temp->pageTable[i]] = true;
		}

		delete temp->pageTable;

		//curr = temp->next;
		//delete temp;
		//temp = curr;

		curr = temp;
		temp = temp->next;
		//delete curr;
	}
	*/
	
	PCB *current = head;
	PCB *temp;

	while (current != NULL)
	{
		*fBlocks += current->pageTableSize;

		for (int i = 0; i < current->pageTableSize; i++)
		{
			arr[current->pageTable[i]] = true;
		}

		delete current->pageTable;

		temp = current;
		current = current->next;
		//delete temp; //<--- causes explosion apparently

	}

}

/*
	Prints PIDs of PCBs currently in the readyqueue.
*/
void readyQueue::printQueue()
{
	PCB *temp = head;
	while (temp != NULL)
	{
		cout << temp->PID << "\t";
		temp = temp->next;
	}
}


/*
	Checks to see if the ready queue is empty.
*/
bool readyQueue::isEmpty()
{
	return (head == NULL);
}