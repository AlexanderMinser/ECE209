/* Alexander Minser
** ECE 209 Fall 2017
** 11/26/2017
** rooms.c
*/

#include "rooms.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Schedule* rooms;
int numberRooms;

/* method for comparing two times
** returns 1 if t1 > t2, else 0
** PARAMETERS:
** t1 --> first time to compare
** t2 --> second time to compare
** RETURN: int 1 or 0
*/
int lessThanTime(Time t1, Time t2) {
    if (t1.hr == t2.hr){
        return (t1.min < t2.min ? 1 : 0);
    }
    return (t1.hr < t2.hr ? 1 : 0);
}

/* test this to see if values are being changed or not, since res is not a pointer */
void insertRNode(struct rNode* reservations, struct reservation new){
    struct rNode* curr = reservations;
    struct rNode* newNode = (struct rNode*) malloc(sizeof(struct rNode));
    while (lessThanTime(curr->res.interval->end, new.interval->start)){
        curr = curr->next;
    }
    newNode->next = curr->next;
    curr->next = newNode;
}

void addReservation(struct rNode* reservations, struct iNode* intervals, const char* name, int roomNum){
    struct iNode* curr = intervals;
    struct reservation new;
    while(curr != NULL){
        if (!strcmp(curr->interval.owner, name)){
            new.roomNumber = roomNum;
            new.interval = &(curr->interval);
            insertRNode(reservations, new);
        }
        curr = curr->next;
    }
}




/* begin assignment specified functions */

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
                printf("something has gone wrong\n");
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
    Schedule s = rooms[room-1];
    return cancel(s, name, start);
}

struct rNode * findReservations(const char * name){
    int i;
    struct rNode* reservations = NULL;
    for(i=0; i<numberRooms; i++) {
        addReservation(reservations, rooms[i]->busy, name, i+1);
        addReservation(reservations, rooms[i]->idle, name, i+1);
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
        fprintf(stream, "\n");
        fprintf(stream, "Room %d:\n", i+1);
        printSchedule(rooms[i], stream);
    }
}
