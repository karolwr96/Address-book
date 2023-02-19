#include <iostream>
#include <windows.h>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <vector>

using namespace std;

struct FriendData {
    int id;
    string name, lastName, telNumber, eMail, adress;
};

int addFriendToList(vector <FriendData> &friends, int lastFriendID);
void showSingleFriend(vector <FriendData> &friends, int friendID);
void showFriendsByName(vector <FriendData> &friends, int lastFriendID);
void showFriendsByLastName(vector <FriendData> &friends, int lastFriendID);
void showAllFriends(vector <FriendData> &friends);
void closeProgramm();
string loadLine();
int selectingOptionFromMenu();
int loadListFromFile(vector <FriendData> &friends);
void saveAfterConversion(vector <FriendData> &friends);
void editFriendData(vector <FriendData> &friends);
int deletingContact(vector <FriendData> &friends);

int main() {
    vector <FriendData> friends;
    int userChoice;
    int lastFriendID = 0;
    lastFriendID = loadListFromFile(friends);

    while (true) {
        cout << "Welcome to address book program! Select an option." << endl;
        cout << endl;
        cout << "1. Add friend." << endl;
        cout << "2. Find friend by name." << endl;
        cout << "3. Find friend by last name." << endl;
        cout << "4. Show all friends." << endl;
        cout << "5. Delete contact." << endl;
        cout << "6. Edit data." << endl;
        cout << "9. Close adress book." << endl;
        cout << endl;
        userChoice = selectingOptionFromMenu();

        switch (userChoice) {
        case 1:
            lastFriendID = addFriendToList(friends,lastFriendID);
            break;

        case 2:
            showFriendsByName(friends, lastFriendID);
            break;

        case 3:
            showFriendsByLastName(friends, lastFriendID);
            break;

        case 4:
            showAllFriends(friends);
            break;

        case 5:
            lastFriendID = deletingContact(friends);
            break;

        case 6:
            editFriendData(friends);
            saveAfterConversion(friends);
            break;

        case 9:
            closeProgramm();

        default:
            cout << "Select an option from 1 to 9." << endl;
            Sleep(1000);
            system("cls");
        }
    }
    return 0;
}

int selectingOptionFromMenu() {
    string userChoice = "";
    bool isChoiceNumber;
    do {
        isChoiceNumber = true;
        userChoice = loadLine();
        for (unsigned int i = 0; i < userChoice.length(); i++) {
            if (!isdigit(userChoice[i])) {
                isChoiceNumber = false;
                break;
            }
        }
        if (!isChoiceNumber) {
            cout << "Option contains invalid characters. Please try again." << endl;
        }
    } while (!isChoiceNumber);
    return atoi(userChoice.c_str());
}

string loadLine() {
    string result = "";
    cin.sync();
    getline(cin, result);
    return result;
}

int addFriendToList(vector <FriendData> &friends, int lastFriendID) {
    FriendData newFriend;
    system("cls");
    cout << "Please enter a name: ";
    newFriend.name = loadLine();
    cout << endl;

    cout << "Please enter a last name: ";
    newFriend.lastName = loadLine();
    cout << endl;

    cout << "Please enter a telephone number: ";
    newFriend.telNumber = loadLine();
    cout << endl;

    cout << "Please enter a e-mail adress: ";
    newFriend.eMail = loadLine();
    cout << endl;

    cout << "Please enter a adress: ";
    newFriend.adress = loadLine();
    cout << endl;

    newFriend.id = lastFriendID + 1;

    friends.push_back(newFriend);

    fstream friendsList;
    friendsList.open("friendsList.txt", ios::out | ios::app);

    friendsList << newFriend.id << "|";
    friendsList << newFriend.name << "|";
    friendsList << newFriend.lastName << "|";
    friendsList << newFriend.telNumber << "|";
    friendsList << newFriend.eMail << "|";
    friendsList << newFriend.adress << "|" << endl;

    friendsList.close();

    cout << "Friend added successfully!" << endl;
    cout << endl;
    system("pause");
    system("cls");
    return ++lastFriendID;
}

void showFriendsByName(vector <FriendData> &friends, int lastFriendID) {
    system("cls");

    cout << "Enter the name to find: ";
    string nameToFind = loadLine();
    int occurrenceNameToFind = 0;

    for (int i = 0; i < lastFriendID; i++) {
        if (friends[i].name == nameToFind) {
            occurrenceNameToFind++;
            showSingleFriend(friends, i);
            cout << endl;
        }
    }
    if (occurrenceNameToFind == 0) {
        cout << "Friend " << nameToFind << " not found.";
        cout << endl;
        system("pause");
        system("cls");
    } else {
        system("pause");
        system("cls");
    }
}

void showFriendsByLastName(vector <FriendData> &friends, int lastFriendID) {
    system("cls");
    cout << "Enter the last name to find: ";
    string lastNameToFind = loadLine();
    int occurrenceLastNameToFind = 0;

    for (int i = 0; i < lastFriendID; i++) {
        if (friends[i].lastName == lastNameToFind) {
            occurrenceLastNameToFind++;
            showSingleFriend(friends, i);
            cout << endl;
        }
    }
    if (occurrenceLastNameToFind == 0) {
        cout << "Friend " << lastNameToFind << " not found.";
        cout << endl;
        system("pause");
        system("cls");
    } else {
        system("pause");
        system("cls");
    }
}

void showSingleFriend(vector <FriendData> &friends, int friendID) {
    cout << endl;
    cout << "ID:               " << friends[friendID].id << endl;
    cout << "Name:             " << friends[friendID].name << endl;
    cout << "Last name:        " << friends[friendID].lastName << endl;
    cout << "Telephone number: " << friends[friendID].telNumber << endl;
    cout << "eMail adress:     " << friends[friendID].eMail << endl;
    cout << "Adress            " << friends[friendID].adress << endl;
}

void showAllFriends(vector <FriendData> &friends) {
    system("cls");
    if (friends.empty()) {
        cout << "Friend list is empty.";
        cout << endl;
        system("pause");
        system("cls");
    } else {
        for(unsigned int i = 0; i < friends.size(); i++) {
            cout << "ID:               " << friends[i].id << endl;
            cout << "Name:             " << friends[i].name << endl;
            cout << "Last name:        " << friends[i].lastName << endl;
            cout << "Telephone number: " << friends[i].telNumber << endl;
            cout << "eMail adress:     " << friends[i].eMail << endl;
            cout << "Adress            " << friends[i].adress << endl;
            cout << endl;
        }
        cout << endl;
        system("pause");
        system("cls");
    }
}

void closeProgramm() {
    system("cls");
    cout << "Thanks for using our programm!";
    cout << endl;
    exit(0);
}

int loadListFromFile(vector <FriendData> &friends) {
    int idToLoad = 0;
    string laneToLoadFromTXT;
    fstream friendsList;
    FriendData singleFriend;
    string DataOfSingleFriend{};

    friendsList.open("friendsList.txt",ios::in);
    if (friendsList.good() == false) {
        return 0;
    }

    while (getline(friendsList, DataOfSingleFriend)) {
        string singleFriendalData{};
        int singlePersonNumber = 1;

        for (size_t index{}; index < DataOfSingleFriend.length(); ++index) {
            if (DataOfSingleFriend[index] != '|') {
                singleFriendalData += DataOfSingleFriend[index];
            } else {
                switch(singlePersonNumber) {
                case 1:
                    singleFriend.id = stoi(singleFriendalData);
                    break;
                case 2:
                    singleFriend.name = singleFriendalData;
                    break;
                case 3:
                    singleFriend.lastName = singleFriendalData;
                    break;
                case 4:
                    singleFriend.telNumber = singleFriendalData;
                    break;
                case 5:
                    singleFriend.eMail = singleFriendalData;
                    break;
                case 6:
                    singleFriend.adress = singleFriendalData;
                    break;
                }
                idToLoad =  singleFriend.id;
                singleFriendalData = "";
                singlePersonNumber++;
            }
        }
        friends.push_back(singleFriend);
    }
    friendsList.close();
    return idToLoad;
}

void editFriendData (vector <FriendData> &friends) {
    int friendDataToChangeID;
    int userChoice;
    string singleDataToChange = "";
    system("cls");

    if (friends.empty()) {
        cout << "Friend list is empty.";
        cout << endl;
        system("pause");
        system("cls");
    }

    else {
        cout << "Please enter a friend ID: ";
        cin >> friendDataToChangeID;
        bool isIdExist = false;

        for (unsigned int i = 0; i < friends.size(); i++) {
            if (friends[i].id == friendDataToChangeID) {
                isIdExist = true;
                break;
            }
        }

        if (!isIdExist) {
            cout << endl;
            cout << "The entered ID does not exist." << endl;
            system("pause");
            system("cls");
        }

        else {
            friendDataToChangeID -= 1;
            cout << endl;

            cout << "Select data to change: " << endl;
            cout << "1. Name." << endl;
            cout << "2. Last name." << endl;
            cout << "3. Telephone number." << endl;
            cout << "4. Email adress. " << endl;
            cout << "5. Adress." << endl;
            cout << "6. Back to the main menu." << endl;

            cin >> userChoice;

            switch (userChoice) {
            case 1:
                cout << "Please enter a new name: " << endl;
                cin >> singleDataToChange;
                friends[friendDataToChangeID].name = singleDataToChange;
                cout << "Name changed corretly.";
                Sleep(1000);
                system("cls");
                break;

            case 2:
                cout << "Please enter new last name: " << endl;
                cin >> singleDataToChange;
                friends[friendDataToChangeID].lastName = singleDataToChange;
                cout << "Last name changed corretly.";
                Sleep(1000);
                system("cls");
                break;

            case 3:
                cout << "Please enter a new telephone number: " << endl;
                cin >> singleDataToChange;
                friends[friendDataToChangeID].telNumber = singleDataToChange;
                cout << "Telephone number changed corretly.";
                Sleep(1000);
                system("cls");
                break;

            case 4:
                cout << "Please enter a new email adress: " << endl;
                cin >> singleDataToChange;
                friends[friendDataToChangeID].eMail = singleDataToChange;
                cout << "Email adress changed corretly.";
                Sleep(1000);
                system("cls");
                break;

            case 5:
                cout << "Please enter a new adress: " << endl;
                cin >> singleDataToChange;
                friends[friendDataToChangeID].adress = singleDataToChange;
                cout << "Adress changed corretly.";
                Sleep(1000);
                system("cls");
                break;

            case 6:
                system("cls");
                break;

            default:
                cout << "Select an option from 1 to 6." << endl;
                Sleep(1000);
                system("cls");
            }
        }
    }
}

void saveAfterConversion(vector <FriendData> &friends) {
    vector <FriendData> ::iterator it = friends.begin();
    fstream friendsList;
    friendsList.open("friendsList.txt",ios::out | ios::trunc);
    if (friendsList.good()) {
        for (unsigned int i = 0; i < friends.size(); i++) {
            friendsList << it->id << "|" << it->name << "|" << it->lastName << "|" << it->telNumber << "|" << it->eMail << "|" << it->adress << "|" << endl;
        }
        friendsList.close();
    }
}

int deletingContact (vector <FriendData> &friends) {
    system("cls");
    int lastID = 0;
    if (friends.empty()) {
        cout << "Friend list is empty.";
        cout << endl;
        system("pause");
        system("cls");
        return 0;
    }

    else {
        int idToDelete;
        char trueOrFalse;

        cout << "Contact delete function." << endl;
        cout << "Please enter friend ID to be deleted:" << endl;
        cin >> idToDelete;

        bool isIdExist = false;

        for (unsigned int i = 0; i < friends.size(); i++) {
            if (friends[i].id == idToDelete) {
                isIdExist = true;
                break;
            }
        }

        if (!isIdExist) {
            cout << endl;
            cout << "The entered ID does not exist." << endl;
            system("pause");
            system("cls");
        } else {
            cout << "Delete the contact? t/n :" << endl;
            cin >> trueOrFalse;

            if (trueOrFalse == 't') {
                for (unsigned int i = 0; i < friends.size() ; i++) {
                    if (friends[i].id == idToDelete) {
                        auto it = friends.begin() + i;
                        friends.erase(it);
                    }
                }

                cout << "Contact deleted." << endl;
                cout << endl;
                system("pause");
                system("cls");

                saveAfterConversion(friends);

                if (friends.empty()) {
                    lastID = 0;
                } else {
                    lastID = friends[friends.size() - 1].id;
                }
            }
        }
    }
    return lastID;
}


