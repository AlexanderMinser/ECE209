/* Alexander Minser
** ECE 209 Fall 2017
** 11/26/2017
** rooms.c
*/

Schedule* rooms;
int numberRooms;

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

}

struct rNode * findReservations(const char * name){

}

void printReservations(const struct rNode* list, FILE* stream){

}

void printRoomSchedules(FILE* stream){

}
