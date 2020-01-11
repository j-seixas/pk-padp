#include <stdlib.h>
#include <stdio.h>
#include <math.h> 
#include "mpi.h" 

#define MAX_PASSENGERS 10

enum ElevatorState {IDLE, UP, DOWN};
enum PassengerState {WAITING, REQUESTED, INSIDE, FINISHED};

typedef enum { false, true } bool;
typedef struct {
    int floor;
    enum ElevatorState eState;
    int passengerIDs[MAX_PASSENGERS];
    bool requested;
    int floorReq;
} Elevator;

typedef struct {
    int initFloor[MAX_PASSENGERS];
    int objectiveFloor[MAX_PASSENGERS];
    enum PassengerState pState[MAX_PASSENGERS];
} Passengers;

Passengers createPassengers(int size);
Elevator createElevator();
bool sameDirection(enum ElevatorState dir, int inicial, int final);
bool checkSomeoneLeaves(Elevator* elevator, Passengers* passengers);
void checkSomeoneEnters(Elevator* elevator, Passengers* passengers);
bool checkRequests(Elevator* elevator, Passengers* passengers);
int moveElevator(Elevator* elevator, int size);
bool allFinished(Passengers* passengers);


Passengers createPassengers(int size) {
    Passengers pass;
    for(int i = 0; i < MAX_PASSENGERS; i++) {
        pass.initFloor[i] = rand() % size;
        do {
            pass.objectiveFloor[i] = rand() % size;
        } while(pass.initFloor[i] == pass.objectiveFloor[i]);
        pass.pState[WAITING];
    }

    return pass;
}

Elevator createElevator() {
    Elevator el;
    el.floor = 0;
    el.eState = IDLE;
    el.requested = false;
    el.floorReq = -1;
    
    for(int i = 0; i < MAX_PASSENGERS; i++) {
        el.passengerIDs[i] = -1;
    }

    return el;
}

bool sameDirection(enum ElevatorState dir, int inicial, int final) {
    if(dir == UP)
        return final - inicial > 0;
    else if(dir == DOWN)
        return final - inicial < 0;

    return false;

}

bool checkSomeoneLeaves(Elevator* elevator, Passengers* passengers) {
    bool left = false;
    int tmp, counter = 0;
    for(int i = 0; i < MAX_PASSENGERS; i++) {
        tmp = (*elevator).passengerIDs[i];
        if(tmp != -1) {
            if((*passengers).objectiveFloor[tmp] == (*elevator).floor && (*passengers).pState[tmp] == INSIDE) {
                (*passengers).pState[tmp] = FINISHED;
                (*elevator).passengerIDs[i] = -1;
                printf("Passenger #%d left elevator on floor %d with destination %d\n", tmp, (*elevator).floor, (*passengers).objectiveFloor[tmp]);
                left = true;
                counter++;
            }
        } else 
            counter++;
    }

    if(counter == MAX_PASSENGERS)
        (*elevator).eState = IDLE;
}

void checkSomeoneEnters(Elevator* elevator, Passengers* passengers) {
    int j = 0;
    for(int i = 0; i < MAX_PASSENGERS; i++) {
        if((*passengers).initFloor[i] == (*elevator).floor && ((*passengers).pState[i] == WAITING || (*passengers).pState[i] == REQUESTED)) {

            if((*elevator).eState != IDLE && !sameDirection((*elevator).eState, (*passengers).objectiveFloor[i], (*passengers).initFloor[i]))
                continue;
            
            if((*elevator).eState == IDLE)
                (*elevator).eState = (*passengers).objectiveFloor[i] - (*passengers).initFloor[i] > 0 ? UP : DOWN;
            
            for(; j < MAX_PASSENGERS; j++) {
                if((*elevator).passengerIDs[j] == -1) {
                    (*elevator).passengerIDs[j] = i;
                    (*passengers).pState[i] = INSIDE;
                    printf("Passenger #%d entered elevator on floor %d with destination %d\n", i, (*elevator).floor, (*passengers).objectiveFloor[i]);
                    break;
                }
            }
        }
    }
}

bool checkRequests(Elevator* elevator, Passengers* passengers) {
    bool found = false;

    for(int i = 0; i < MAX_PASSENGERS; i++) {
        if(!found && (*passengers).pState[i] == REQUESTED) {
            found = true;
            (*elevator).requested = true;
            (*elevator).floorReq = (*passengers).initFloor[i];
        } else if(found && (*passengers).pState[i] == REQUESTED) 
            (*passengers).pState[i] = WAITING;
    }

    if(found) {
        printf("Found request from passenger on floor %d\n", (*elevator).floorReq);
        return found;
    }
    
    for(int i = 0; i < MAX_PASSENGERS; i++) {
        if(!found && (*passengers).pState[i] == WAITING) {
            found = true;
            (*elevator).requested = true;
            (*elevator).floorReq = (*passengers).initFloor[i];
            printf("Found request from passenger on floor %d\n", (*elevator).floorReq);
            return found;
        }
    }
    
    return false;
}

int moveElevator(Elevator* elevator, int size) {
    int nextFloor;
    if((*elevator).eState == UP) {
        if((*elevator).floor == size - 1) {
            nextFloor = (*elevator).floor - 1;
            (*elevator).eState = IDLE;
        } else
            nextFloor = (*elevator).floor + 1;
        
    } else if((*elevator).eState == DOWN) {
        if((*elevator).floor == 0) {
            nextFloor = (*elevator).floor + 1;
            (*elevator).eState = IDLE;
        } else
            nextFloor = (*elevator).floor - 1;
    }

    if((*elevator).eState != IDLE) {
        printf("-> Elevator going from %d to %d\n", (*elevator).floor, nextFloor);
        (*elevator).floor = nextFloor;
    }

    return nextFloor;
}

bool allFinished(Passengers* passengers) {
    for(int i = 0; i < MAX_PASSENGERS; i++) {
        if((*passengers).pState[i] != FINISHED)
            return false;
    }

    return true;
}

int main(int argc, char **argv) {
	int rank, size;	
    
	MPI_Status statusElevator, statusPassengers;
	MPI_Datatype new_type_elevator, typeElevator[5] = {MPI_INT, MPI_INT, MPI_INT, MPI_C_BOOL, MPI_INT};
	int block_lengths_array_elevator[5] = {1, 1, MAX_PASSENGERS, 1 , 1};

	MPI_Aint dispElevator[5], baseElevator, addrElevator;
	Elevator elevator;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(rank == 0) 
		printf("Creating Elevator and Passengers data type \n");

	MPI_Get_address(&elevator, &baseElevator);
	MPI_Get_address(&(elevator.floor), &addrElevator);
	dispElevator[0] = addrElevator - baseElevator;
	MPI_Get_address(&(elevator.eState), &addrElevator);
	dispElevator[1] = addrElevator - baseElevator;
    MPI_Get_address(&(elevator.passengerIDs), &addrElevator);
	dispElevator[2] = addrElevator - baseElevator;
    MPI_Get_address(&(elevator.requested), &addrElevator);
	dispElevator[3] = addrElevator - baseElevator;
    MPI_Get_address(&(elevator.floorReq), &addrElevator);
	dispElevator[4] = addrElevator - baseElevator;
	
	MPI_Type_create_struct(5, block_lengths_array_elevator, dispElevator, typeElevator, &new_type_elevator);
	MPI_Type_commit(&new_type_elevator);


    MPI_Datatype new_type_passengers, typePassengers[3] = {MPI_INT, MPI_INT, MPI_INT};
	int block_lengths_array_passengers[3] = {MAX_PASSENGERS, MAX_PASSENGERS, MAX_PASSENGERS};

    MPI_Aint dispPassengers[5], basePassengers, addrPassengers;
	Passengers passengers;
    MPI_Get_address(&passengers, &basePassengers);
	MPI_Get_address(&(passengers.initFloor), &addrPassengers);
	dispPassengers[0] = addrPassengers - basePassengers;
	MPI_Get_address(&(passengers.objectiveFloor), &addrPassengers);
	dispPassengers[1] = addrPassengers - basePassengers;
    MPI_Get_address(&(passengers.pState), &addrPassengers);
	dispPassengers[2] = addrPassengers - basePassengers;
	
	MPI_Type_create_struct(3, block_lengths_array_passengers, dispPassengers, typePassengers, &new_type_passengers);
	MPI_Type_commit(&new_type_passengers);


    bool firstRun = true;
    bool left = false;
    int nextFloor;

    do {

        if(firstRun && rank == 0) {
            firstRun = false;
            passengers = createPassengers(size);
            elevator = createElevator();

            checkSomeoneEnters(&elevator, &passengers);

            if(elevator.eState == IDLE) {
                if(checkRequests(&elevator, &passengers)) {
                    MPI_Send(&elevator, 1, new_type_elevator, elevator.floorReq, 0, MPI_COMM_WORLD);
                    MPI_Send(&passengers, 1, new_type_passengers, elevator.floorReq, 0, MPI_COMM_WORLD);
                }
            } else {
                nextFloor = moveElevator(&elevator, size);
                MPI_Send(&elevator, 1, new_type_elevator, nextFloor, 0, MPI_COMM_WORLD);
                MPI_Send(&passengers, 1, new_type_passengers, nextFloor, 0, MPI_COMM_WORLD);

            }

        } else {

            MPI_Recv(&elevator, 1, new_type_elevator, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &statusElevator);
            MPI_Recv(&passengers, 1, new_type_passengers, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &statusPassengers);

            if(rank == elevator.floor && elevator.eState != IDLE) {
                left = checkSomeoneLeaves(&elevator, &passengers);

                if(left) {
                    left = false;
                    checkSomeoneEnters(&elevator, &passengers);
                }
            }
            
            if(elevator.eState == IDLE) {
                if(elevator.requested && elevator.floorReq == rank) {
                    elevator.floor = rank;
                    elevator.requested = false;
                    checkSomeoneEnters(&elevator, &passengers);
                } else {
                    if(checkRequests(&elevator, &passengers)) {
                        MPI_Send(&elevator, 1, new_type_elevator, elevator.floorReq, 0, MPI_COMM_WORLD);
                        MPI_Send(&passengers, 1, new_type_passengers, elevator.floorReq, 0, MPI_COMM_WORLD);
                        continue;
                    }
                }
            } 
            
            if(elevator.eState != IDLE) {
                nextFloor = moveElevator(&elevator, size);
                MPI_Send(&elevator, 1, new_type_elevator, nextFloor, 0, MPI_COMM_WORLD);
                MPI_Send(&passengers, 1, new_type_passengers, nextFloor, 0, MPI_COMM_WORLD);
            }

        }


    } while (!allFinished(&passengers));

	// MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();
	return 0;
}

