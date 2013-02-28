/**
 * This is a sample to demonstrate using the Bowler embedded C stack.
 */

#include <stdio.h>
#include "Bowler/Bowler.h"

void simpleBowlerExample();
void advancedBowlerExample();

int main(void){

	/**
	 * This is a simple example of using the BowlerStack interfaces to interact with a host entirely in user code
	 */
	simpleBowlerExample();
	/**
	 * This example shows how to integrate the Framework using the built in bcs.core namespace handler
	 */
	advancedBowlerExample();

	return 0;
}
