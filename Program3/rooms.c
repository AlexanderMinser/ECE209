/* Alexander Minser
** ECE 209 Fall 2017
** 11/26/2017
** rooms.c
*/

#include "rooms.h"

Schedule* rooms;
int numberRooms;

/* test this to see if values are being changed or not, since res is not a pointer */
void insertRNode(struct rNode* reservations, reservation new){
    struct rNode* curr = reservations;
    while (lessThanTime(curr->res.interval.end, new->res.interval.start)){
        curr = curr->next;
    }
    new->next = curr->next;
    curr->next = new;
}

void addReservation(struct rNode* reservations, struct iNode* intervals, const char* name, int roomNum){
    struct iNode* curr = intervals;
    struct reservation new;
    while(curr != NULL){
        if (!strcmp(curr->interval.owner, name)){
            res = {roomNum, curr->interval};
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

int numRooms(){
    return numberRooms;
}

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


int cancelReservation(int room, const char* name, Time start){
    Schedule s = rooms[room-1];
    return cancel(s, name, start);
}

struct rNode * findReservations(const char * name){
    int i;
    struct rNode* reservations;
    for(i=0; i<numberRooms; i++) {
        curr = rooms[i]->busy;
        addReservation(reservations, rooms[i]->busy, name, i+1);
        addReservation(reservations, rooms[i]->idle, name, i+1);
    }
}

void printReservations(const struct rNode* list, FILE* stream){
    struct rNode* curr = list;
    while(curr != NULL) {
        fprintf(stream, "ROOM %d: ", curr->res.roomNumber);
        fprintf(stream, "%02d:", curr->res.interval->start.hr);
        fprintf(stream, "%02d - ", curr->res.interval->start.min);
        fprintf(stream, "%02d:", curr->res.interval->end.hr);
        fprintf(stream, "%02d ", curr->res.interval->end.min);
        fprintf(stream, "%s\n", curr->res.interval.owner);

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
