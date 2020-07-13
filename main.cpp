#include "Chirper.h"

int main(){
	Chirper ch;						//initiate user
	ch.setupAccount();				//setup user account
	Chirper* f = ch.addFriends();	//add friends
	ch.displayMenu(f);				//display menu
	delete f;						//delete friends
	return 0;
}