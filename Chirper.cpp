#include "Chirper.h"

Chirper::Chirper(){};
Chirper::Chirper(const Chirper &o){};
Chirper::~Chirper(){
	postPtr temp = head;		//create temporary postPtr to iterate through list
	while(temp != NULL){		//while temp is not NULL (ie there are still chirps to delete)
		postPtr tempNext = temp->next;	//create another temp postPtr and assign it to the next value (as to not lose current place)
		delete temp;					//delete current node
		temp = tempNext;				//continute to next node
	}
};


void Chirper::setDisplayName(std::string newName){displayName = newName;};
void Chirper::setUsername(std::string newName){username = newName;};
postPtr getHead(Chirper chirper){return chirper.head;};

void Chirper::setupAccount(){
	bool answer = false;
	while(!answer){					//while the user has not accepted the terms and conditions, loop through the intro
		clearPage();				//clear the page (this is to provide a cleaner interface)
		std::cout << "Welcome to Chirper! Do you wish to read the terms and conditions, or simply accept them? (Use numbers to navigate menus; invalid inputs will do nothing)\n\n";
		std::cout << "1) Read terms and conditions\n";
		std::cout << "2) Accept blindly\n";
		std::cin >> selector;		//take input in selector
		switch(selector){			//switch statement to go over the different values selector could have
			case 1:					//for 1, print terms and conditions
				printTermsAndConditions();
				break;
			case 2:					//for 2, continue with the running of the program
				answer = true;
				break;
			default:				//for all other cases, clear the cin buffer and ignore, in case the input was invalid
				std::cin.clear();
      			std::cin.ignore(10000,'\n');
		};
	};
	std::cout << "\nPlease choose a username: ";std::cin >> username;
    std::cin.ignore(10000,'\n');													//clear buffer because getline does not ignore whitespaces like cin does
	std::cout << "Please choose a display name: ";getline(std::cin, displayName);	//getline used to make it possible to include spaces in the display name
	std::cout << "\nYour chirper account is now all set up! Press enter to continue.";
	waitForEnter();					//wait for the user to press enter before continuing (to ensure they have read everything before the page is cleared)
};

Chirper* Chirper::addFriends(){
  	std::ifstream file("friends_list.dat");	//load the friend file into ifstream
  	std::string text;						//text being taken from file

	std::cout << "\nLoading friends...";

  	if (!file){																					//if the friend file did not open (ie does not exist)
    	std::cout << "\n\nWarning! You have no friends! Please get a friend, then try again.";	//warn the user that they have no friends
    	exit(1); 																				//terminate with an error
  	}

	std::cout << "\n\nFriends loaded! Press enter to continue.";	//while the friends aren't actually loaded yet, the file is loaded and the information in it will be loaded
																	//in addition, the user has to press enter anyway due to the cin.ignore; might as well provide an interface
  	Chirper* f = new Chirper; 	//create new object of friend class
  	
	file >> text;				//get username from friend file
  	f->setUsername(text);		//set username accordingly
  	file >> text;				//get display name from friend file
  	f->setDisplayName(text);	//set display name accordingly

  	std::cin.ignore(10000,'\n');					//clear buffer for getline
    for(std::string line; getline(file, line); ){	//for each subsequent line received from friend file...
      	if(line == "") continue;					//skip over blank lines
      	line.resize(MAX_MESSAGE_LENGTH);			//make sure message is in bounds
	  	postPtr friendHead = getHead(*f);			//get friend's head
      	f->addChirp(f->head, line, false);			//add chirp to friend's feed as original post
  	}
	return f;
}

void Chirper::displayMenu(Chirper* f){
	bool close = false;
	while(!close){					//while the user has not closed chirper, loop through the menu
		clearPage();
		std::cout << "Chirper\n=======\n\n";
		std::cout << "Welcome, " << displayName << "! What would you like to do?\n\n";	//address the user by name
		std::cout << "1) Make a Chirp!\n";
		std::cout << "2) Scroll through my Chirps\n";
		std::cout << "3) Scroll through my friend's Chirps\n";
		std::cout << "4) Change my display name\n";
		std::cout << "5) Close Chirper\n";
		std::cin >> selector;
		switch(selector){
			case 1:					//for 1, call the function to create a new chirp
				newChirp();
				break;
			case 2:					//for 2, call the viewFeed() function for the current user
				viewFeed(head);
				break;
			case 3:					//for 3, call the viewFeed() function for the user's friend
				f->viewFeed(head);
				break;
			case 4:					//for 4, call the function to set the user's display name to something else
				changeDisplayName();
				break;
			case 5:					//for 5, close the program by stopping the menu loop
				close = true;
				break;
			default:				//for all other cases, clear the cin buffer and ignore, in case the input was invalid
				std::cin.clear();
      			std::cin.ignore(10000,'\n');
		};
	};
};

void Chirper::newChirp(){
	std::string message;										//temporary storage for message
	std::cout << "\nWhat's happening? (Max 180 characters)\n";	//prompt for message
    std::cin.ignore(10000,'\n');								//clear buffer for getline
	getline(std::cin, message);									//get message from user
	if (message.length() <= MAX_MESSAGE_LENGTH) addChirp(head, message, false);//if message is valid, pass it through to addChirp as an original post
	else {
		std::cout << "\nYour message is too long. Press enter to return to the main menu.";
		waitForEnter();
	}
};

void Chirper::addChirp(postPtr &feedHead, std::string message, bool rechirp){
	std::cout << "\nFeed Head: " << feedHead;
	postPtr newPost = new post;					//create a new post
	newPost->next = feedHead;					//make the new post's next point to head
	newPost->prev = NULL;						//make the new post's prev point to NULL
	newPost->message = message;					//add message to new post
	if (rechirp) newPost->isRechirp = true;		//set as rechirp if it is
	if (feedHead != NULL) feedHead->prev = newPost;//if head isn't NULL, then make it's prev point to the new post
	feedHead = newPost;							//move head to the new post
	std::cout << "\nFeed Head (after): " << feedHead;
};

void Chirper::viewFeed(postPtr& userHead){
	std::cout << head;			//print out head (user will never see this unless they scroll up)
	postPtr temp = head;		//create a pointer to iterate through list
	while (temp != NULL){		//while the pointer is not null
		displayChirp(userHead, temp);//display the chirp as the current user
		switch(selector){		//based on the selection the user made within the displayChirp function, do something
			case 1:				//for 1, add a like to the chirp
				likeChirp(temp);
				break;
			case 2:				//for 2, rechirp the chirp to user's feed
				rechirp(userHead, temp);
			case 3:				//for 2, move on to the next chirp, if possible
				if (temp->next != NULL) temp = temp->next;
				break;
			case 4:				//for 3, move back to the previous chirp, if possible
				if (temp->prev != NULL) temp = temp->prev;
				break;
			case 5:				//for 4, set temp to NULL, causing the while loop to end and returning to the main menu
				temp = NULL;
				break;
			case 6:				//for 5, delete the current chirp
				if (head == userHead) {
					temp = deleteChirp(temp);
					userHead = head;			//since we are deleting nodes, the head of the feed could change; seeing as we just checked that head == userHead, this is the safest place to update userHead to the new value of head
				};
				break;
			default:			//for all other cases, clear the cin buffer and ignore, in case the input was invalid
				std::cin.clear();
    			std::cin.ignore(10000,'\n');
		};
	};
};

void Chirper::displayChirp(postPtr userHead, postPtr chirp){
	clearPage();
	for (int i=0; i < CHIRP_WIDTH;i++) std::cout << "+";std::cout << "\n";	//print out the top border of the chirp
	if (!chirp->isRechirp) formatMessage(displayName + " (@" + username + ")", 0);//print out the name line (original post)
	else formatMessage(displayName + " (@" + username + ") (rechirp)", 0);
	formatMessage(" ", 0);													//draw empty line between name and message
	formatMessage(chirp->message, 0);										//print out the message
	formatMessage(" ", 0);formatMessage(" ", 0);							//draw two empty lines
	if (chirp->liked) formatMessage("\xe2\x9d\xa4 " + std::to_string(chirp->likes) + " \xe2\x86\x94 " + std::to_string(chirp->rechirps), -4);	//print out likes and rechirps with empty heart if not liked
	else formatMessage("\xe2\x99\xa1 " + std::to_string(chirp->likes) + " \xe2\x86\x94 " + std::to_string(chirp->rechirps), -4);				//print out likes and rechirps with full heart if likes
	for (int i=0; i < CHIRP_WIDTH;i++) std::cout << "+";std::cout << "\n";	//print out the bottom border of the chirp

	std::cout << "\n1) Like post\n";										//present options for each post
	std::cout << "2) Rechirp post\n";
	if (chirp->next != NULL) std::cout << "3) Go to next post\n";			//only display next/previous buttons when applicable
	if (chirp->prev != NULL) std::cout << "4) Go to previous post\n";
	std::cout << "5) Close Feed\n";
	if (head == userHead) std::cout << "6) Delete Post\n";
	std::cin >> selector;
};

void Chirper::formatMessage(std::string text, int shift){
	int spaceLeftOnLine = CHIRP_WIDTH - (CHIRP_BORDER_WIDTH + text.length() + shift);	//the amount of space to fill with spaces
	int lastSpaceBeforeCutoff = 0;														//the last space in a message before it needs to cut to the next line
	if (text.length() > CHIRP_TEXT_WIDTH){												//for multi line messages
		for (int i=0; i <= CHIRP_TEXT_WIDTH; i++) lastSpaceBeforeCutoff = i;			//find out where the message needs to be cut off
		std::string output = text.substr(0,lastSpaceBeforeCutoff);						//create substring for first part of message
		spaceLeftOnLine = CHIRP_WIDTH - (CHIRP_BORDER_WIDTH + lastSpaceBeforeCutoff);	//recalculate for substring
		std::cout << "+++   " << output;												//output substring
		for (int i=0; i < spaceLeftOnLine; i++) std::cout << " ";std::cout << "+++\n";	//fill in spaces and formatting
		formatMessage(text.substr(lastSpaceBeforeCutoff), 0);							//recursively call the function again with the rest of the message
	}
	else{																				//if the message is not multi line
		std::cout << "+++   " << text;													//output message
		for (int i=0; i < spaceLeftOnLine; i++) std::cout << " ";std::cout << "+++\n";	//fill in spaces and formatting
	}
};

void Chirper::likeChirp(postPtr chirp){
	chirp->likes += 1;		//increase likes by 1 (no limit on how many times you can like your own post)
	chirp->liked = true;	//if liked, set to true
};

void Chirper::rechirp(postPtr& userHead, postPtr chirp){
	chirp->rechirps += 1;					//increases rechirps by 1
	std::cout << "\nUser Head: " << userHead;
	addChirp(userHead, chirp->message, true);		//rechirps the chirp
	std::cout << "\nUser Head (after): " << userHead;
};

postPtr Chirper::deleteChirp(postPtr chirp){				//since this function is only accessible if the list is not empty, no checks for empty lists are necessary (also means chirp is not NULL)
	postPtr nextPost = NULL;								//set the next post to go to as NULL (if this is not changed, upon function return, the user will be taken to the main menu)
	if (chirp->next != NULL) nextPost = chirp->next;		//if the next post exists, set it as the next post to go to
	else if (chirp->prev != NULL) nextPost = chirp->prev;	//else, if the previous post exists, set it as the next post to go to
	
	if (chirp->prev != NULL) chirp->prev->next = chirp->next;	//if chirp->prev exists, make its next point to chirp's next (chirp's next can be NULL)
	else head = chirp->next;									//if chirp->prev doesn't exist, chirp is the first post, so head can be made to point to chirp->next (whether it is NULL or not)
	if (chirp->next != NULL) chirp->next->prev = chirp->prev;	//if chirp->next exists, make its prev point to chirp's prev (chirp's prev can be NULL)
	//no course of action is required otherwise

	delete chirp;		//finally, delete chirp

	if (nextPost == NULL) std::cout << "\nPost deleted. You have no more posts. Press enter to return to main menu.";
	else std::cout << "\nPost deleted. Press enter to continue.";

	std::cin.ignore(10000,'\n');
	waitForEnter();
	return nextPost;
};

void Chirper::changeDisplayName(){
	std::cout << "\nPlease enter a new name: ";		//prompt user for new name
	std::cin.ignore(10000,'\n');					//clear buffer for getline
	getline(std::cin, displayName);					//get line from user (including spaces)
	std::cout << "\nAll set! Press enter to return to main menu.";
	waitForEnter();
};

void Chirper::clearPage(){for (int i = 0;i < 100; i++) std::cout << "\n";};	//write 100 newlines to the terminal

void Chirper::waitForEnter(){
	char input = 0;									//set the input to 0
	while (input != '\n'){std::cin.get(input);}		//get keypress from the user until the key pressed is enter
};

void Chirper::printTermsAndConditions(){
	clearPage();
	std::cout << "Terms and Conditions\n\n";
	std::cout << "Welcome to Chirper!\n\n";
	std::cout << "These terms and conditions outline the rules and regulations for the use of Chirper Inc.'s Website, located at www.chirper.com.\n\n";
	std::cout << "By accessing this website we assume you accept these terms and conditions. Do not continue to use Chirper if you do not agree to take all of the terms and conditions stated on this page. Our Terms and Conditions were created with the help of the Terms And Conditions Generator and the Free Terms & Conditions Generator.\n\n";
	std::cout << "The following terminology applies to these Terms and Conditions, Privacy Statement and Disclaimer Notice and all Agreements: 'Client', 'You' and 'Your' refers to you, the person log on this website and compliant to the Company’s terms and conditions. 'The Company', 'Ourselves', 'We', 'Our' and 'Us', refers to our Company. 'Party', 'Parties', or 'Us', refers to both the Client and ourselves. All terms refer to the offer, acceptance and consideration of payment necessary to undertake the process of our assistance to the Client in the most appropriate manner for the express purpose of meeting the Client’s needs in respect of provision of the Company’s stated services, in accordance with and subject to, prevailing law of Netherlands. Any use of the above terminology or other words in the singular, plural, capitalization and/or he/she or they, are taken as interchangeable and therefore as referring to same.\n\n";
	std::cout << "License\n\n";
	std::cout << "Unless otherwise stated, Chirper Inc. and/or its licensors own the intellectual property rights for all material on Chirper. All intellectual property rights are reserved. You may access this from Chirper for your own personal use subjected to restrictions set in these terms and conditions.\n\n";
	std::cout << "You must not: Republish material from Chirper; Sell, rent or sub-license material from Chirper; Reproduce, duplicate or copy material from Chirper; Redistribute content from Chirper\n\n";
	std::cout << "This Agreement shall begin on the date hereof.\n\n";
	std::cout << "Parts of this website offer an opportunity for users to post and exchange opinions and information in certain areas of the website. Chirper Inc. does not filter, edit, publish or review Comments prior to their presence on the website. Comments do not reflect the views and opinions of Chirper Inc.,its agents and/or affiliates. Comments reflect the views and opinions of the person who post their views and opinions. To the extent permitted by applicable laws, Chirper Inc. shall not be liable for the Comments or for any liability, damages or expenses caused and/or suffered as a result of any use of and/or posting of and/or appearance of the Comments on this website.\n\n";
	std::cout << "Chirper Inc. reserves the right to monitor all Comments and to remove any Comments which can be considered inappropriate, offensive or causes breach of these Terms and Conditions.\n\n";
	std::cout << "Press enter to return to account setup.";
	std::cin.ignore(10000,'\n');
	waitForEnter();
};