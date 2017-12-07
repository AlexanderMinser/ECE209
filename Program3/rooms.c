/* Alexander Minser
** ECE 209 Fall 2017
** 11/26/2017
** rooms.c
** Run my code in EOS I suppose? Ths was tested on Ubuntu Linux so whichever
** environment is closest to that please.
*/

#include "rooms.h"
#include "schedule.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//#include "schedule.c"

Schedule* rooms;
int numberRooms;

/* compares two times, returning 1 if t1 <= t2, else 0
** PARAMETERS:
** Time t1 ---> first time to be compared
** Time t2 ---> second time to be compared
** RETURN: integer indicating true/false
*/
int lessThanEqualTime2(Time t1, Time t2) {
    if (t1.hr == t2.hr){
        return (t1.min <= t2.min ? 1 : 0);
    }
    return (t1.hr <= t2.hr ? 1 : 0);
}

/* inserts node into linked list */
struct rNode* insertRNode(struct rNode* reservations, struct reservation new, int roomnum){
    struct rNode* curr = reservations;
    struct rNode* newNode = (struct rNode*) malloc(sizeof(struct rNode));
    newNode->res = new;
    struct rNode temp;
    if (reservations == NULL){ /* list empty */
        reservations = newNode;
    } else if (lessThanEqualTime2(newNode->res.interval->end, reservations->res.interval->start) ){
        /* belongs first in list */
        temp.res = reservations->res;
        reservations->res = newNode->res;
        newNode->res = temp.res;
        newNode->next = reservations->next;
        reservations->next = newNode;
    } else {
        printf("ran\n");
        while (curr->next != NULL && lessThanEqualTime2(curr->next->res.interval->start, newNode->res.interval->start)) {
            printf("here\n");
            curr = curr->next;
        }
        newNode->next = curr->next;
        curr->next = newNode;
    }
    return reservations;
}

/* inserts new reservation into reservations list */
struct rNode* addReservation(struct rNode* reservations, struct iNode* intervals, const char* name, int roomNum){
    struct iNode* curr = intervals;
    struct reservation new;
    while(curr != NULL){
        if (!strcmp(curr->interval.owner, name)){
            new.roomNumber = roomNum;
            new.interval = &(curr->interval);
            reservations = insertRNode(reservations, new, roomNum);
        }
        curr = curr->next;
    }
    return reservations;
    printf("is null: %d\n", reservations == NULL);
}




/* begin assignment specified functions */
/* instantiates rooms */
void setupRooms(int nrooms, Time open, Time close){
    rooms = (Schedule*) malloc(sizeof(Schedule)*nrooms);
    numberRooms = nrooms;
    int i;
    Schedule s;
    for(i=0; i<nrooms; i++) {
        s = createSchedule(open, close);
        rooms[i] = s;
    }
}

/* returns number of rooms in rooms[]
** PARAMETERS: None
** RETURN: integer for number of rooms
*/
int numRooms(){
    return numberRooms;
}

/* adds a "reservation" to a specific room by finding the
** first room in the rooms array that time is open, then adds
** busy time to that room's schedule
** PARAMETERS:
** const char* name --> name of owner of new busy time
** Time start --> time the reservation will begin
** Time end --> time the reservation will end
** RETURN: integer 1 indicating success or 0 for failure
*/
int makeReservation(const char* name, Time start, Time end){
    int i;
    int flag;
    for(i=0; i<numberRooms; i++) {
        if(!isBusy(rooms[i], start, end)){
            flag = reserve(rooms[i], name, start, end);
            if (!flag){
                return 0; /* invalid time */
            }
            return i+1; /* must be 1 greater than i since room nums start at 1*/
        }
    }
    return 0;
}

/* removes a "reservation" from a specific room by finding the
** specified room in the rooms array, then removing the busy time
** specified in that room's schedule
** PARAMETERS:
** int room ---> index for room array + 1
** const char* name ---> name of busy time owner to delete
** Time start ---> time at which busy time for deletion starts
** RETURN: integer 1 indicating success or 0 for failure
*/
int cancelReservation(int room, const char* name, Time start){
    if (room > numberRooms)
        return 0;
    Schedule s = rooms[room-1];
    return cancel(s, name, start);
}
/* creates reservations list by name */
struct rNode * findReservations(const char * name){
    int i;
    struct rNode* reservations = NULL;
    for(i=0; i<numberRooms; i++) {
        reservations = addReservation(reservations, rooms[i]->busy, name, i+1);
        reservations = addReservation(reservations, rooms[i]->idle, name, i+1);
    }
    return reservations;
}

void printReservations(const struct rNode* list, FILE* stream){
    struct rNode* curr = (struct rNode*) list;
    while(curr != NULL) {
        fprintf(stream, "ROOM %d: ", curr->res.roomNumber);
        fprintf(stream, "%02d:", curr->res.interval->start.hr);
        fprintf(stream, "%02d - ", curr->res.interval->start.min);
        fprintf(stream, "%02d:", curr->res.interval->end.hr);
        fprintf(stream, "%02d ", curr->res.interval->end.min);
        fprintf(stream, "%s\n", curr->res.interval->owner);

        curr = curr->next;
    }
}

void printRoomSchedules(FILE* stream){
    int i;
    for(i=0; i<numberRooms; i++){
        fprintf(stream, "\n"); /* is this supposed to be here? */
        fprintf(stream, "Room %d:\n", i+1);
        printSchedule(rooms[i], stream);
    }
}
