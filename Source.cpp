#include<iostream>
using namespace std;

void help() {
	cout << endl;
	cout << "\t\t\t-----------------------------------------------------------------------------------------------------------" << endl;
	cout << "\t\t\t\t\t\t -----------------" <<  endl;
	cout << "\t\t\t\t\t\t|Plot of the game |" << endl;
	cout << "\t\t\t\t\t\t -----------------" <<  endl;
	cout << "\t\t\t->The player is on a quest to retrieve a mystical crystal from dangerous and enchanted forest." << endl;
	cout << "\t\t\tThe forest is filled with mystical enemies and obstacles." << endl;
	cout << "\t\t\tThe player must cross through the forest to find that mystical crystal." << endl;
	cout << "\t\t\tThe player must cross through the forest to find that mystical crystal." << endl;
	cout << "\t\t\t-----------------------------------------------------------------------------------------------------------" << endl;
	cout << endl;

}

void anotherChoice() {
	cout << endl;
	cout << "Following are the algorithms to find the path: " << endl;
	cout << "a. dkjistra Algorithm " << endl;
	cout << "b. Krushkal Algorithm " << endl;
	cout << endl;
	cout << "Enter the algorithm you want to use -> " << " ";
	char choice;
	cin >> choice;

	if (choice == 'a') {
		// implement the dkjistra algorithm
		cout << "Implementing dkjistra algorithm " << endl;
	}
	else if (choice == 'b') {
		// Implement the krushkal algorithm 
		cout << "Implementing Krushkal algorithm " << endl;
	}
	else {
		cout << "Invalid Entry " << endl;
	}
}


int main()
{
	int choice;
	do {
		cout << "\t\t\t========================================" << endl;
		cout << "\t\t\t\t Quest In a Forest Game " << endl;
		cout << "\t\t\t========================================" << endl;
		cout << endl;

		cout << "\t\t\t-----------------------------------------" << endl;
		cout << "\t\t\t|1. Play Game                           |" << endl;
		cout << "\t\t\t|2. How To Play                         |" << endl;
		cout << "\t\t\t|3. Exit                                |" << endl;
		cout << "\t\t\t-----------------------------------------" << endl;
		cout << endl;

		cout << "Enter your choice: " << " ";
		
		cin >> choice;

		switch (choice) {
		case 1:
			anotherChoice();
			break;

		case 2:
			help();
			break;

		case 3:
			cout << "Exited from the game" << endl;
			break;

		default:
			cout << "Invalid Choice " << endl;
		}
	} while (choice!=3);

}