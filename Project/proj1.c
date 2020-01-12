#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "mpi.h" 

#define MAX_PASSENGERS 10

/*
    This is my implementation of the Elevator (Lift):
        - The threads are the floors;
        - The passengers are randomized into a certain floor and with a random final floor. They have a state of WAITING for elevator, 
        REQUESTED if they requested the elevator, INSIDE if they are inside the elevator and FINISHED if they arrived at destination. 
        The passengers are a struct that is sent between threads;
        - The elevator is a struct also sent between threads, that has the passengers ids that are inside, the current floor, the state
        (if IDLE, going UP or going DOWN), and if it was requested while it was IDLE;
        - There are  some functions with description explaining the objective of the function
*/


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
    int iter;
    int finished;
} Passengers;

Passengers createPassengers(int size);
Elevator createElevator();
bool sameDirection(enum ElevatorState dir, int inicial, int final);
bool checkSomeoneLeaves(Elevator* elevator, Passengers* passengers);
void checkSomeoneEnters(Elevator* elevator, Passengers* passengers);
bool checkRequests(Elevator* elevator, Passengers* passengers);
int moveElevator(Elevator* elevator, Passengers* passengers, int size);
bool allFinished(Passengers* passengers);

/*
    Creates the passengers with random starting floor and random finishing floor (can't have the same
    finishing and start floor)
*/
Passengers createPassengers(int size) {
    Passengers pass;
    pass.iter = 0;
    pass.finished = 0;
    time_t t;  
    /* Intializes random number generator */
    srand((unsigned) time(&t));

    for(int i = 0; i < MAX_PASSENGERS; i++) {
        pass.initFloor[i] = rand() % size;
        do {
            pass.objectiveFloor[i] = rand() % size;
        } while(pass.initFloor[i] == pass.objectiveFloor[i]);
        pass.pState[i] = WAITING;
        printf("--> Passenger #%d created - init: %d, obj: %d\n", i, pass.initFloor[i], pass.objectiveFloor[i]);
    }

    return pass;
}

/*
    Creates elevator
*/
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

/*
    Function to check if the person's objective floor is the same direction the elevator is going
*/
bool sameDirection(enum ElevatorState dir, int final, int inicial) {
    if(dir == UP)
        return final - inicial > 0;
    else if(dir == DOWN)
        return final - inicial < 0;

    return false;

}

/*
    Function to check if any passenger leaves on the current floor and to make him leave if true
*/
bool checkSomeoneLeaves(Elevator* elevator, Passengers* passengers) {
    bool left = false;
    int tmp, counter = 0;
    for(int i = 0; i < MAX_PASSENGERS; i++) {
        tmp = (*elevator).passengerIDs[i];
        if(tmp != -1) {
            if((*passengers).objectiveFloor[tmp] == (*elevator).floor && (*passengers).pState[tmp] == INSIDE) {
                (*passengers).pState[tmp] = FINISHED;
                (*elevator).passengerIDs[i] = -1;
                printf("iter: %d, Passenger #%d left elevator on floor %d with destination %d\n", (*passengers).iter, tmp, (*elevator).floor, (*passengers).objectiveFloor[tmp]);
                left = true;
                counter++;
            }
        } else 
            counter++;
    }

    if(counter == MAX_PASSENGERS)
        (*elevator).eState = IDLE;
    
    return left;
}

/*
    Function to check if any passenger enters on the current floor and make him enter if true
*/
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
                    printf("iter: %d, Passenger #%d entered elevator on floor %d with destination %d\n", (*passengers).iter, i, (*elevator).floor, (*passengers).objectiveFloor[i]);
                    break;
                }
            }
        }
    }
}

/*
    Function to check next request or to some waiting person make a request
*/
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
        printf("iter: %d, Found request from passenger on floor %d\n", (*passengers).iter, (*elevator).floorReq);
        return found;
    }
    
    for(int i = 0; i < MAX_PASSENGERS; i++) {
        if(!found && (*passengers).pState[i] == WAITING) {
            found = true;
            (*elevator).requested = true;
            (*elevator).floorReq = (*passengers).initFloor[i];
            printf("iter: %d, Found request from passenger on floor %d\n", (*passengers).iter, (*elevator).floorReq);
            return found;
        }
    }
    
    return false;
}

/*
    Function to get next floor for the elevator
*/
int moveElevator(Elevator* elevator, Passengers* passengers, int size) {
    int nextFloor = 0;
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
        printf("iter: %d, --> Elevator going from %d to %d\n", (*passengers).iter, (*elevator).floor, nextFloor);
        (*elevator).floor = nextFloor;
    }

    return nextFloor;
}

/*
    Function to check if every passenger is already on current final destination
*/
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


    MPI_Datatype new_type_passengers, typePassengers[5] = {MPI_INT, MPI_INT, MPI_INT, MPI_INT, MPI_INT};
	int block_lengths_array_passengers[5] = {MAX_PASSENGERS, MAX_PASSENGERS, MAX_PASSENGERS, 1, 1};

    MPI_Aint dispPassengers[5], basePassengers, addrPassengers;
	Passengers passengers;
    MPI_Get_address(&passengers, &basePassengers);
	MPI_Get_address(&(passengers.initFloor), &addrPassengers);
	dispPassengers[0] = addrPassengers - basePassengers;
	MPI_Get_address(&(passengers.objectiveFloor), &addrPassengers);
	dispPassengers[1] = addrPassengers - basePassengers;
    MPI_Get_address(&(passengers.pState), &addrPassengers);
	dispPassengers[2] = addrPassengers - basePassengers;
    MPI_Get_address(&(passengers.iter), &addrPassengers);
	dispPassengers[3] = addrPassengers - basePassengers;
    MPI_Get_address(&(passengers.finished), &addrPassengers);
	dispPassengers[4] = addrPassengers - basePassengers;
	
	MPI_Type_create_struct(5, block_lengths_array_passengers, dispPassengers, typePassengers, &new_type_passengers);
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
                    printf("iter: %d, --> Elevator went to request on %d\n", passengers.iter, elevator.floorReq);
                    MPI_Send(&elevator, 1, new_type_elevator, elevator.floorReq, 0, MPI_COMM_WORLD);
                    MPI_Send(&passengers, 1, new_type_passengers, elevator.floorReq, 0, MPI_COMM_WORLD);
                }
            } else {
                nextFloor = moveElevator(&elevator, &passengers, size);
                MPI_Send(&elevator, 1, new_type_elevator, nextFloor, 0, MPI_COMM_WORLD);
                MPI_Send(&passengers, 1, new_type_passengers, nextFloor, 0, MPI_COMM_WORLD);

            }

        } else {

            MPI_Recv(&elevator, 1, new_type_elevator, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &statusElevator);
            MPI_Recv(&passengers, 1, new_type_passengers, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &statusPassengers);
            passengers.iter++;

            if(passengers.finished == 1) {
                if(rank != 0 && rank < size - 1) {
                    MPI_Send(&elevator, 1, new_type_elevator, rank + rank - statusElevator.MPI_SOURCE, 0, MPI_COMM_WORLD);
                    MPI_Send(&passengers, 1, new_type_passengers, rank + rank - statusPassengers.MPI_SOURCE, 0, MPI_COMM_WORLD);
                }

            } else {

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
                            printf("iter: %d, --> Elevator went to request on %d\n", passengers.iter, elevator.floorReq);
                            MPI_Send(&elevator, 1, new_type_elevator, elevator.floorReq, 0, MPI_COMM_WORLD);
                            MPI_Send(&passengers, 1, new_type_passengers, elevator.floorReq, 0, MPI_COMM_WORLD);
                            continue;
                        }
                    }
                } 
                
                if(elevator.eState != IDLE) {
                    nextFloor = moveElevator(&elevator, &passengers, size);
                    MPI_Send(&elevator, 1, new_type_elevator, nextFloor, 0, MPI_COMM_WORLD);
                    MPI_Send(&passengers, 1, new_type_passengers, nextFloor, 0, MPI_COMM_WORLD);
                } 

                if(allFinished(&passengers)){
                    passengers.finished = 1;
                    if(rank == 0) {
                        MPI_Send(&elevator, 1, new_type_elevator, rank + 1, 0, MPI_COMM_WORLD);
                        MPI_Send(&passengers, 1, new_type_passengers, rank + 1, 0, MPI_COMM_WORLD);

                    } else if(rank >= size - 1) {
                        MPI_Send(&elevator, 1, new_type_elevator, rank - 1, 0, MPI_COMM_WORLD);
                        MPI_Send(&passengers, 1, new_type_passengers, rank - 1, 0, MPI_COMM_WORLD);

                    } else {
                        MPI_Send(&elevator, 1, new_type_elevator, rank + 1, 0, MPI_COMM_WORLD);
                        MPI_Send(&passengers, 1, new_type_passengers, rank + 1, 0, MPI_COMM_WORLD);
                        MPI_Send(&elevator, 1, new_type_elevator, rank - 1, 0, MPI_COMM_WORLD);
                        MPI_Send(&passengers, 1, new_type_passengers, rank - 1, 0, MPI_COMM_WORLD);
                    }

                }
            }

        }


    } while (passengers.finished == 0);

	// MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();
	return 0;
}

