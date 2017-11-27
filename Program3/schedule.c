/* Alexander Minser
** ECE 209 Fall 2017
** 11/26/2017
** schedule.c
*/


#include <stdlib.h>

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

void insertNode(iNode* list, iNode* new) {
    iNode* curr = list;
    while (lessThanTime(curr->interval.end, new->interval.start)){
        curr = curr->next;
    }
    new->next = curr->next;
    curr->next = new;
}

void deleteNode(iNode* list, ){

}

/* merges two adjacent idle times together into one node
** ex. (9:30 to 10:30) -> (10:30 to 11:30) becomes (9:30 to 11:30)
** done by extending end member of first node in merge, then freeing
** second node
*/
void mergeIdle(Schedule s) {
    iNode* curr = s->idle;
    iNode* next;
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

int isBusy(Schedule s, Time start, Time end){
    iNode* currNode = s.busy;
    iData currData;
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
    iNode* new = (iNode*) malloc(sizeof(iNode));
    new->interval.start = start;
    new->interval.end = end;
    new->interval.owner = memcpy(new->interval.owner, name, NAME_LENGTH+1);

    insertNode(s.busy, new);
    clearTime(s.idle, start, end);
    return 1;
}

int cancel(Schedule s, const char *name, Time start){
    if (!isBusy(s, start, start)) /*start passed in twice b/c isBusy requires end time, none given*/
        return 0;


    mergeIdle(s);
    return 1;
}
