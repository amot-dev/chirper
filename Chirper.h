#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <fstream>
#define MAX_MESSAGE_LENGTH 180
#define CHIRP_WIDTH 120
#define CHIRP_BORDER_WIDTH 9
#define CHIRP_TEXT_WIDTH 108

struct post{
	std::string message;	//contains the message of a chirper post (limited to 180 characters)
	bool liked = false;		//whether the post has been liked or not
	bool isRechirp = false;	//whether the post is a repost or not
	int likes = 0;			//contains the amount of likes a post has
	int rechirps = 0;		//contains the amount of rechirps a post has
	post *prev;				//pointer to the previous post
	post *next;				//pointer to the next post
};
typedef post* postPtr;		//define post pointer as a type

class Chirper;
class Chirper{
public:
	Chirper();					//constructor
	Chirper(const Chirper &o); 	//copy constructor
	~Chirper();					//deconstructor

	void setDisplayName(std::string);	//sets the user's display name
	void setUsername(std::string);		//sets the user's username
	friend postPtr getHead(Chirper);	//takes an object and returns the head of its feed

	void setupAccount();			//sets up the user's account
	Chirper* addFriends();			//adds a friend from a file
	void displayMenu(Chirper* f);	//takes the user's friend (to pass it on to viewFeed if necessary) and displays all options available to the user
	
private:
	postPtr head = NULL;			//head of doubly linked list of posts
	std::string username;			//stores the username
	std::string displayName;		//stores the display name
	int selector;					//used for selecting options throughout Chirper

	void newChirp();							//grabs a 180 character message from the user and pipes it through to addChirp. returns false if failed
	void addChirp(postPtr &head, std::string, bool rechirp);//takes a message, the head of a doubly linked list, and whether it should be a repost, and inserts the message into a new post at the top of the list
	void viewFeed(postPtr& userHead);			//takes in the head of the user (as a way to identify them) and lets them navigate their own or their friends' posts
	void displayChirp(postPtr userHead, postPtr);//displays a chirp and lets the user like it, rechirp it, delete it, or move to the next or previous chirp
	void formatMessage(std::string, int);		//formats messages into posts
	void likeChirp(postPtr);					//likes a certain chirp
	void rechirp(postPtr& userHead, postPtr);	//rechirps a post to your feed (it can be your post too)
	postPtr deleteChirp(postPtr);				//deletes a certain chirp and returns the next chirp to be displayed (NULL will cause a return to the main menu)

	void changeDisplayName();					//allows a user to modify their display name

	void clearPage();					//clears the console of text by inserting 100 newlines
	void waitForEnter();				//waits until the user presses enter, then lets the program continue
	void printTermsAndConditions();		//prints the terms and conditions of Chirper Inc. (fake terms, obviously)
};