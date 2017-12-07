/* Alexander Minser
** ECE 209 Fall 2017
** 11/26/2017
** schedule.c
** Run my code in EOS I suppose? Ths was tested on Ubuntu Linux so whichever
** environment is closest to that please.
*/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "schedule.h"

/* compares two times, returning 1 if t1 < t2, else 0
** PARAMETERS:
** Time t1 ---> first time to be compared
** Time t2 ---> second time to be compared
** RETURN: integer indicating true/false
*/
int lessThanTime(Time t1, Time t2) {
    if (t1.hr == t2.hr){
        return (t1.min < t2.min ? 1 : 0);
    }
    return (t1.hr < t2.hr ? 1 : 0);
}

/* compares two times, returning 1 if t1 <= t2, else 0
** PARAMETERS:
** Time t1 ---> first time to be compared
** Time t2 ---> second time to be compared
** RETURN: integer indicating true/false
*/
int lessThanEqualTime(Time t1, Time t2) {
    if (t1.hr == t2.hr){
        return (t1.min <= t2.min ? 1 : 0);
    }
    return (t1.hr <= t2.hr ? 1 : 0);
}

/* compares two times, returning 1 if t1 == t2, else 0
** PARAMETERS:
** Time t1 ---> first time to be compared
** Time t2 ---> second time to be compared
** RETURN: integer indicating true/false
*/
int equalTime(Time t1, Time t2) {
    if ((t1.hr == t2.hr) && (t1.min == t2.min))
        return 1;
    return 0;
}

/* inserts a node into an iNode* list */
void insertNode(Schedule s, struct iNode* list, struct iNode* new, int useBusy) {
    new->next = NULL;
    struct iNode temp;
    if (list == NULL){ /* list empty */
        if (useBusy)
            s->busy = new;
        else
            s->idle = new;
    } else if (lessThanEqualTime(new->interval.end, list->interval.start)){
        /* first item in list */
        temp.interval = list->interval;
        list->interval = new->interval;
        new->interval = temp.interval;
        new->next = list->next;
        list->next = new;
    } else { /* step through until find correct place */
        struct iNode* curr = list;
        while (curr->next != NULL && lessThanEqualTime(curr->next->interval.start, new->interval.start)){
            curr = curr->next;
        }
        new->next = curr->next;
        curr->next = new;
    }
}

/* checks to see if room is open during time requested for reservation */
int notOpen(Schedule s, Time start, Time end){
    if (lessThanTime(start, s->start))
        return 1;
    else if (lessThanTime(s->end, end))
        return 1;
    else
        return 0;
}

/* allocates new iNode* and assigns times to it*/
struct iNode* buildInterval(Time start, Time end) {
    struct iNode* new = (struct iNode*) malloc(sizeof(struct iNode));
    new->interval.start = start;
    new->interval.end = end;
    return new;
}

/* removes time slot from busy list by finding specific reserved time
** and removing from list
** PARAMETERS:
** struct iNode* list ---> busy list to remove node from
** Time start ---> start of busy time to be removed
** RETURN: node with correct reservation removed or NULL if not found
*/
struct iNode* clearBusy(Schedule s, struct iNode* list, Time start, const char* name) {
    struct iNode* curr = list;
    struct iNode* prev = NULL;
    while(curr != NULL){
        if (equalTime(curr->interval.start, start) && !strcmp(curr->interval.owner, name)) {
            if (prev == NULL){ /* if item is first in list */
                s->busy = curr->next;
            } else{
                prev->next = curr->next;
            }
            strcpy(curr->interval.owner, ""); /* clear owner name */
            return curr;
        }
        prev = curr;
        curr = curr->next;
    }
    return NULL; /* means function failed to find specified time slot */
}

/* removes a time slot from idle list by adjusting existing nodes
** for time slot being reserved
** PARAMETERS:
** struct iNode* list ---> idle linked list to edit
** Time start ---> start of busy time to be removed
** Time end ---> end of busy time to be removed
** RETURN: node with appropriate times removed from list or NULL if not found
*/
struct iNode* clearIdle(Schedule s, struct iNode* list, Time start, Time end) {
    struct iNode* curr = list;
    struct iNode* prev = NULL;
    struct iNode* new;
    struct iNode* temp;
    Time* currStart;
    Time* currEnd;
    while(curr != NULL){
        currStart = &(curr->interval.start);
        currEnd = &(curr->interval.end);
        if (equalTime(*currStart, start) && equalTime(*currEnd, end)) {
            if (prev == NULL)
                /* if item is first in list */
                s->idle = curr->next;
             else
                prev->next = curr->next;
            return curr;
        } else if (equalTime(*currStart, start)){
            *currStart = end;
            return buildInterval(start, end);
        } else if (equalTime(*currEnd, end)){
            *currEnd = start;
            return buildInterval(start, end);
        } else if (lessThanTime(*currStart, start) && lessThanTime(end, *currEnd)){
            new = buildInterval(end, *currEnd);
            *currEnd = start;
            new->next = curr->next;
            curr->next = new;
            temp = buildInterval(start, end);
            return temp;
        }
        prev = curr;
        curr = curr->next;
    }
    return NULL; /* means function failed to find specified time slot */
}

/* function used for testing, prints out all times in a iNode list */
void printNodes(struct iNode* list) {
    struct iNode* curr = list;
    while(curr != NULL){
        printf("%02d:%02d", curr->interval.start.hr, curr->interval.start.min);
        printf("-%02d:%02d\n", curr->interval.end.hr, curr->interval.end.min);
        curr = curr->next;
    }
}

/* merges two adjacent idle times together into one node
** ex. (9:30 to 10:30) -> (10:30 to 11:30) becomes (9:30 to 11:30)
** done by extending end member of first node in merge, then freeing
** second node
*/
void mergeIdle(Schedule s) {
    struct iNode* curr = s->idle;
    struct iNode* next;
    if (curr == NULL) /* nothing to merge */
        return;
    while(curr->next != NULL) {
        while (equalTime(curr->interval.end, curr->next->interval.start)){
            next = curr->next;
            curr->interval.end = next->interval.end;
            curr->next = next->next;
            free(next);
            if (curr->next == NULL)
                break;
        }
        if (curr->next == NULL)
            break;
        curr = curr->next;
    }
}


/* marks start of assignment-specificed functions*/

/* initilizes schedule with one idle node spanning from start to end
** specified by parameters
** PARAMETERS:
** Time start ---> start time of schedule
** Time end ---> end time of schedule
** RETURN: initialized schedule
*/
Schedule createSchedule(Time start, Time end) {
    Schedule s = (Schedule) malloc(sizeof(Schedule));
    struct iNode* idle = (struct iNode*) malloc(sizeof(struct iNode));
    idle->interval.start = start;
    idle->interval.end = end;
    s->idle = idle;
    s->start = start;
    s->end = end;
    return s;
}


/* checks to see if specific time is found in shcedule by comparing
** given times to each busy list time
** PARAMETERS:
** Schedule s ---> schedule to see if time is busy
** Time start ---> start time of interval to look for
** Time end ---> end time of interval to look for
** RETURN: 1 or 0 indicating busy or free respectively
** returns 1 if start or end time given is between start and end time of any
** interval or start and end time are less than and greater than any interval
** respectively
*/
int isBusy(Schedule s, Time start, Time end){
    struct iNode* currNode = s->busy;
    struct iData currData;
    while(currNode != NULL) {
        currData = currNode->interval;
        if (lessThanEqualTime(currData.start, start) && lessThanTime(start, currData.end)){
            return 1;
        } else if (lessThanTime(currData.start, end) && lessThanEqualTime(end, currData.end)){
            return 1;
        } else if (lessThanEqualTime(start, currData.start) && lessThanEqualTime(currData.end, end)){
            return 1;
        }
        currNode = currNode->next;
    }
    return 0;
}

/* makes a reservation for time and name provided (busy list) */
int reserve(Schedule s, const char *name, Time start, Time end){
    if (isBusy(s, start, end))
        return 0;
    else if (notOpen(s, start, end))
        return 0;
    struct iNode* new = clearIdle(s, s->idle, start, end);
    strcpy(new->interval.owner, name);

    insertNode(s, s->busy, new, 1);
    mergeIdle(s);
    return 1; /* indicates success */
}

/* removes a busy time from the busy list of schedule and adds it to the
** idle times list
** PARAMETERS:
** Schedule s ---> schedule with time to rearrange
** const char* name ---> name of busy time owner to change
** Time start ---> start time for busy time to put in idle list
** RETURN: 1 or 0 indicating success or failure respectively
*/
int cancel(Schedule s, const char *name, Time start){
    struct iNode* cancelled = clearBusy(s, s->busy, start, name);
    if (cancelled == NULL)
        return 0;
    insertNode(s, s->idle, cancelled, 0);
    mergeIdle(s);
    return 1;
}

/* prints schedule */
void printSchedule(Schedule s, FILE* stream){
        struct iNode* busy = s->busy;
        struct iNode* idle = s->idle;

        while(busy != NULL || idle != NULL){
            if (busy != NULL && lessThanTime(busy->interval.start, idle->interval.start)) {
                fprintf(stream, "BUSY %02d:", busy->interval.start.hr);
                fprintf(stream, "%02d - ", busy->interval.start.min);
                fprintf(stream, "%02d:", busy->interval.end.hr);
                fprintf(stream, "%02d ", busy->interval.end.min);
                fprintf(stream, "%s\n", busy->interval.owner);
                busy = busy->next;
            } else if (idle != NULL){
                fprintf(stream, "IDLE %02d:", idle->interval.start.hr);
                fprintf(stream, "%02d - ", idle->interval.start.min);
                fprintf(stream, "%02d:", idle->interval.end.hr);
                fprintf(stream, "%02d ", idle->interval.end.min);
                fprintf(stream, "%s\n", idle->interval.owner);
                idle = idle->next;
            }
        }
}
