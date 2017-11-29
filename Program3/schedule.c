/* Alexander Minser
** ECE 209 Fall 2017
** 11/26/2017
** schedule.c
*/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "schedule.h"


int lessThanTime(Time t1, Time t2) {
    if (t1.hr == t2.hr){
        return (t1.min < t2.min ? 1 : 0);
    }
    return (t1.hr < t2.hr ? 1 : 0);
}

int equalTime(Time t1, Time t2) {
    if ((t1.hr == t2.hr) && (t1.min == t2.min))
        return 1;
    return 0;
}

void insertNode(struct iNode* list, struct iNode* new) {
    struct iNode* curr = list;
    while (lessThanTime(curr->interval.end, new->interval.start)){
        curr = curr->next;
    }
    new->next = curr->next;
    curr->next = new;
}

struct iNode* deleteNode(struct iNode* list, Time start){
    struct iNode* curr = list;
    struct iNode* next;
    while(curr->next != NULL){ /* ***MAY NOT WORK IF ONLY ONE ITEM IN LIST *** */
        if (equalTime(curr->next->interval.start, start)) {
            next = curr->next;
            curr->next = curr->next->next;
            return next;
        }
    }
    return NULL; /* means function failed to find specified time slot */
}

/*assumes time attempting to clear is all already in list
** (no error checking for user input)
*/
void clearTime(struct iNode* list, Time start, Time end) {
    struct iNode* curr = list;
    struct iNode* prev = NULL;
    while(curr != NULL){ /* ***MAY NOT WORK IF ONLY ONE ITEM IN LIST *** */
        if (equalTime(curr->interval.start, start)) { /* does not account for excess time not reserved *** */
            prev->next = curr->next;
            return curr;
        } else if (lessThanTime(start, curr->interval.end)){
            curr->interval.end =
        }
        prev = curr;
        curr = curr->next;
    }
    return NULL; /* means function failed to find specified time slot */
}

/* merges two adjacent idle times together into one node
** ex. (9:30 to 10:30) -> (10:30 to 11:30) becomes (9:30 to 11:30)
** done by extending end member of first node in merge, then freeing
** second node
*/
void mergeIdle(Schedule s) {
    struct iNode* curr = s->idle;
    struct iNode* next;
    while(curr->next != NULL) {
        if (equalTime(curr->interval.end, curr->next->interval.start)){
            next = curr->next;
            curr->interval.end = next->interval.end;
            curr->next = next->next;
            free(next);
        }
        curr = curr->next;
    }
}


/* marks start of assignment-specificed functions*/

Schedule createSchedule(Time start, Time end) {
    Schedule s = {NULL, NULL, start, end};
    struct iNode* idle = (struct iNode*) malloc(sizeof(struct iNode));
    idle->interval.start = start;
    idle->interval.end = end;
    s->idle = idle;
    return s;
}



int isBusy(Schedule s, Time start, Time end){
    struct iNode* currNode = s->busy;
    struct iData currData;
    while(currNode != NULL) {
        currData = currNode->interval;
        if (lessThanTime(currData.start, start) && lessThanTime(start, currData.start)){
            return 1;
        } else if (lessThanTime(currData.end, end) && lessThanTime(end, currData.end)){
            return 1;
        }
        currNode = currNode->next;
    }
    return 0;
}


int reserve(Schedule s, const char *name, Time start, Time end){
    if (isBusy(s, start, end))
        return 0;
    struct iNode* new = (struct iNode*) malloc(sizeof(struct iNode));
    new->interval.start = start;
    new->interval.end = end;
    strcpy(new->interval.owner, name);

    insertNode(s->busy, new);
    clearTime(s->idle, start, end);
    return 1;
}
/* ****REMEMBER TO CLEAR OWNER STRING IN THIS FUNCTION**** */
int cancel(Schedule s, const char *name, Time start){
    if (!isBusy(s, start, start)) /*start passed in twice b/c isBusy requires end time, none given*/
        return 0;
    struct iNode* cancelled = deleteNode();
    insertNode(s->idle, cancelled);

    mergeIdle(s);
    return 1;
}

void printSchedule(Schedule s, FILE* stream){
        struct iNode* busy = s->busy;
        struct iNode* idle = s->idle;

        while(busy != NULL && idle != NULL){
            if (lessThanTime(busy->interval.start, idle->interval.start)) {
                fprintf(stream, "BUSY %d: ", busy->interval.start.hr);
                fprintf(stream, "%d - ", busy->interval.start.min);
                fprintf(stream, "%d:", busy->interval.end.hr);
                fprintf(stream, "%d ", busy->interval.end.min);
                fprintf(stream, "%s\n", busy->interval.owner);
            } else {
                fprintf(stream, "IDLE %d: ", idle->interval.start.hr);
                fprintf(stream, "%d - ", idle->interval.start.min);
                fprintf(stream, "%d:", idle->interval.end.hr);
                fprintf(stream, "%d ", idle->interval.end.min);
                fprintf(stream, "%s\n", idle->interval.owner);
            }
        }
}
