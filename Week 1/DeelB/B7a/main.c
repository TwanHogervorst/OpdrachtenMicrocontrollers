/*
 * B7a.c
 *
 * Created: 23-2-2022 12:18:57
 * Author : twanh
 */ 

#include <avr/io.h>
#include <stdio.h>

#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>

#define BIT(x) (1 << x)

typedef enum StateId {
	Start,
	S1,
	S2,
	S3,
	End
} StateId;

/* For function pointers: parameter description is given using 'px = description',
	with 'x' being the parameter index.
*/
typedef struct State {
	const char* name;
	StateId id;
	void (*init)(struct State*); /* p1 = currentState */
	enum StateId (*update)(struct State*); /* p1 = currentState; Return newStateId */
	void (*exit)(struct State*, struct State*); /* p1 = oldState, p2 = newState */
} State;

State stateList[];

void wait( int ms )
{
	for (int i=0; i<ms; i++) {
		_delay_ms( 1 );		// library function (max 30 ms at 8MHz)
	}
}

/* 
	The state_init, update and start method are for fsm demo purposes.
	For a real implementation every state would have it's own methods,
	but this makes it easier for a simple demonstration
*/
void state_init(State* currentState) {
	
	PORTE = BIT(currentState->id);
	
}

StateId state_update(State* currentState) {
	
	unsigned char btnValue = PIND;
	
	if(btnValue & BIT(7))
		return Start;
	
	switch(currentState->id) {
		case Start:
		
			if(btnValue & BIT(6))
				return S1;
				
		break;
		case S1:
		
			if(btnValue & BIT(5))
				return S2;
		
		break;
		case S2:
		
			if(btnValue & BIT(5))
				return S3;
			if(btnValue & BIT(6))
				return S2;
		
		break;
		case S3:
		
			if(btnValue & (BIT(5) | BIT(6)))
				return End;
		
		break;
		case End:
		
		break;
		
		default:
			return currentState->id;
	}
}

void state_exit(State* oldState, State* newState) {
	
	PORTE = 0;
	
}

State stateList[] = {
	 {
		 .name = "Start",
		 .id = Start,
		 .init = &state_init,
		 .update = &state_update,
		 .exit = &state_exit,
	 },
	 {
		 .name = "State 1",
		 .id = S1,
		 .init = &state_init,
		 .update = &state_update,
		 .exit = &state_exit,
	 },
	 {
		 .name = "State 2",
		 .id = S2,
		 .init = &state_init,
		 .update = &state_update,
		 .exit = &state_exit,
	 },
	 {
		 .name = "State 3",
		 .id = S3,
		 .init = &state_init,
		 .update = &state_update,
		 .exit = &state_exit,
	 },
	 {
		 .name = "End",
		 .id = End,
		 .init = &state_init,
		 .update = &state_update,
		 .exit = &state_exit,
	 },
 };

int main(void)
{
	
	// Initialize I/O
	DDRD = 0x00; // PORTD is used for input
	DDRE = 0xFF; // PORTE is used as output for displaying the state
	
	// currentState is first item in list
	State* currentState = stateList;
	(*currentState->init)(currentState);
	
	StateId newStateId = currentState->id;
	State* newState = NULL;
    while (1) 
    {
		newStateId = (*currentState->update)(currentState);
		
		if(newStateId != currentState->id) {
			newState = stateList + newStateId;
			
			(*currentState->exit)(currentState, newState);
			currentState = newState;
			(*currentState->init)(currentState);
		}
    }
	
	return 0;
}
