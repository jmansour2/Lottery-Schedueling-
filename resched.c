/**
 * @file resched.c
 * @provides resched
 *
 * COSC 3250 Assignment 4
 */
/* Embedded XINU, Copyright (C) 2008.  All rights reserved. */

#include <xinu.h>

extern void ctxsw(void *, void *);
/**
 * Reschedule processor to next ready process.
 * Upon entry, currpid gives current process id.  Proctab[currpid].pstate 
 * gives correct NEXT state for current process if other than PRREADY.
 * @return OK when the process is context switched back
 */
syscall resched(void)
{
    pcb *oldproc;               /* pointer to old process entry */
    pcb *newproc;               /* pointer to new process entry */

    oldproc = &proctab[currpid];
	irqmask im;
	im = disable();
    /* place current process at end of ready queue */
    if (PRCURR == oldproc->state)
    {
        oldproc->state = PRREADY;
        enqueue(currpid, readylist);
    }

    /**
     * We recommend you use a helper function for the following:
     * TODO: Get the total number of tickets from all processes that are in current and ready states.
     * Utilize the random() function to pick a random ticket. 
     * Traverse through the process table to identify which proccess has the random ticket value.
     * Remove process from queue.
     * Set currpid to the new process.
*	2 for loops
*	if statment
*	returns the process id for state
*	9.1 OS book	
     */
	int totaltickets =0; 
	int k, j;
	int ticketcounter=0;

	for(j =0; j < NPROC; j++){
		if((PRCURR == proctab[j].state) || (PRREADY == proctab[j].state)){
			totaltickets  += proctab[j].tickets;
		}
	}
	int winner = random(totaltickets);

	for(k = 0; k < NPROC; k++) {
		if((PRCURR == proctab[k].state) || (PRREADY == proctab[k].state)){
                        ticketcounter += proctab[k].tickets;
		}
	
	
	if(ticketcounter > winner){
		currpid = k;
		break;
		}
	}
	remove(currpid);
   	 //currpid = dequeue(readylist);
    
	newproc = &proctab[currpid];
    	newproc->state = PRCURR;    /* mark it currently running    */

#if PREEMPT
    preempt = QUANTUM;
#endif

    ctxsw(&oldproc->stkptr, &newproc->stkptr);
	
	restore(im);
    /* The OLD process returns here when resumed. */
    return OK;
}
