#include <iostream>
#include <windows.h>
#include <vector>
#include <fstream>
#include <cstdio>

using namespace std;

struct UserData {
    int addressBookUserId;
    string userName, password;
};

struct FriendData {
    int friendId, userId;
    string name, lastName, telNumber, eMail, adress;
};

string loadLine();
void loadingUsersData (vector <UserData> &users);
int selectingOptionFromMenu();
void registerNewUser(vector <UserData> &users);
string inputNewUserName(vector <UserData> &users);
int loggingUser(vector <UserData> &users);
int showingMenuOfLoggedUser(vector <UserData> &users, int &idLoggedInUser);
void savingUserDataAfterChange(vector <UserData> &users);
void changingThePassword(vector <UserData> &users, int idLoggedInUser);
void closeProgramm();

int loadingToVectorUserFriendsData(vector <FriendData> &friends, int idLoggedInUser);
int addFriendToList(vector <FriendData> &friends, int idLoggedInUser, int lastFriendId);
void showFriendsByName(vector <FriendData> &friends);
void showFriendsByLastName(vector <FriendData> &friends);
void showAllFriends(vector <FriendData> &friends);
void showSingleFriend(vector <FriendData> &friends, int friendToShowId);
int deletingContact(vector <FriendData> &friends, int lastFriendID);
int saveFileAfterFriendRemove(vector <FriendData> &friends, int idToDelete);
void editFriendData (vector <FriendData> &friends);
void saveFileAfterFriendDataEdit(vector <FriendData> &friends, int editedFriendId, int possitionInLocalVector);

int main() {
    vector <UserData> users;
    int idLoggedInUser = 0;
    loadingUsersData(users);

    while (true) {
        system("cls");
        cout << "   MAIN MENU" << endl;
        cout << endl;
        cout << "1. Registration." << endl;
        cout << "2. Login." << endl;
        cout << "9. Close the programm." << endl;
        cout << endl;
        cout << "Your choice: ";
        int userChoice = selectingOptionFromMenu();

        switch (userChoice) {
        case 1:
            registerNewUser(users);
            break;

        case 2:
            idLoggedInUser = loggingUser(users);
            if (idLoggedInUser) {
                showingMenuOfLoggedUser(users, idLoggedInUser);
            }
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

string loadLine() {
    string result = "";
    cin.sync();
    getline(cin, result);
    return result;
}

void loadingUsersData (vector <UserData> &users) {
    string laneToLoadFromTXT;
    fstream usersList;
    UserData singleUser;
    string dataOfSingleUser{};

    usersList.open("usersList.txt", ios::in);

    if (!usersList.good()) {
        return;
    }

    while (getline(usersList, dataOfSingleUser)) {
        string singleUserData{};
        int singlePersonNumber = 1;

        for (size_t index{}; index < dataOfSingleUser.length(); ++index) {
            if (dataOfSingleUser[index] != '|') {
                singleUserData += dataOfSingleUser[index];
            } else {
                switch(singlePersonNumber) {
                case 1:
                    singleUser.addressBookUserId = stoi(singleUserData);
                    break;
                case 2:
                    singleUser.userName = singleUserData;
                    break;
                case 3:
                    singleUser.password = singleUserData;
                    break;
                }
                singleUserData = "";
                singlePersonNumber++;
            }
        }
        users.push_back(singleUser);
    }
    usersList.close();
    return;
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

void registerNewUser(vector <UserData> &users) {
    UserData newUser;
    unsigned int amountOfUsers = users.size();

    cout << endl;
    cout << "Please enter a new user name: ";
    newUser.userName = inputNewUserName(users);

    cout << "Please enter a password: ";
    newUser.password = loadLine();
    cout << endl;

    newUser.addressBookUserId = ++amountOfUsers;
    users.push_back(newUser);

    fstream usersList;
    usersList.open("usersList.txt", ios::out | ios::app);

    usersList << newUser.addressBookUserId << "|";
    usersList << newUser.userName << "|";
    usersList << newUser.password << "|" << endl;

    usersList.close();

    cout << "User added successfully.";
    cout << endl << endl;
    system("pause");
}

string inputNewUserName(vector <UserData> &users) {
    string newUserName = "";
    bool isNewUserNameExist;

    do {
        isNewUserNameExist = false;
        newUserName = loadLine();

        for (unsigned int i = 0; i < users.size(); i++) {
            if (newUserName == users[i].userName) {
                isNewUserNameExist = true;
                break;
            }
        }
        if (isNewUserNameExist) {
            cout << "This username is already taken." << endl;
            cout << "Please try again." << endl;
        }
    } while (isNewUserNameExist);

    return newUserName;
}

int loggingUser(vector <UserData> &users) {
    string login, password;
    bool doesNameExist = false;
    int possitionInVector = 0;
    cout << "Please input user login: ";
    login = loadLine();

    for (unsigned int i = 0; i < users.size(); i++) {
        if (login == users[i].userName) {
            doesNameExist = true;
            possitionInVector = i;
            break;
        }
    }
    possitionInVector++;
    if (!doesNameExist) {
        cout << "Login does not exist." << endl;
        system("pause");
        return 0;
    } else if (doesNameExist) {
        cout << "Please input password: ";
        password = loadLine();

        if (password == users[(possitionInVector - 1)].password) {
            cout << "Login correct." << endl;
            cout << endl;
            system("pause");
        } else {
            cout << "Wrong password. Please try again." << endl;
            cout << endl;
            system("pause");
            return 0;
        }
    }
    return possitionInVector;
}

int showingMenuOfLoggedUser(vector <UserData> &users, int &idLoggedInUser) {
    system("cls");
    vector <FriendData> friends;
    int lastFriendID = loadingToVectorUserFriendsData(friends, idLoggedInUser);
    while (true) {
        system("cls");
        cout << "Welcome to address book program! Select an option." << endl;
        cout << endl;
        cout << "1. Add friend." << endl;
        cout << "2. Find friend by name." << endl;
        cout << "3. Find friend by last name." << endl;
        cout << "4. Show all friends." << endl;
        cout << "5. Delete contact." << endl;
        cout << "6. Edit data." << endl;
        cout << endl;
        cout << "7. Change the password." << endl;
        cout << "8. Log out." << endl;
        cout << "9. Close adress book." << endl;
        cout << endl;

        cout << "Your choice: ";
        int userChoice = selectingOptionFromMenu();

        if (userChoice == 8) {
            idLoggedInUser = 0;
            break;
        }

        switch (userChoice) {
        case 1:
            lastFriendID = addFriendToList(friends, idLoggedInUser, lastFriendID);
            break;

        case 2:
            showFriendsByName(friends);
            break;

        case 3:
            showFriendsByLastName(friends);
            break;

        case 4:
            showAllFriends(friends);
            break;

        case 5:
            lastFriendID = deletingContact(friends, lastFriendID);
            break;

        case 6:
            editFriendData(friends);
            break;

        case 7:
            changingThePassword(users, idLoggedInUser);
            break;

        case 9:
            closeProgramm();

        default:
            cout << "Select an option from 1 to 9." << endl;
            system("pause");
        }
    }
    return 0;
}

void savingUserDataAfterChange(vector <UserData> &users) {
    fstream usersList;
    usersList.open("usersList.txt",ios::out | ios::trunc);
    if (usersList.good()) {
        for (unsigned int i = 0; i < users.size(); i++) {
            usersList << users[i].addressBookUserId << "|";
            usersList << users[i].userName << "|";
            usersList << users[i].password << "|" << endl;
        }
        usersList.close();
    }
}

void changingThePassword(vector <UserData> &users, int idLoggedInUser) {
    string newAccountPassword;
    cout << endl;
    cout << "Please write new password: ";
    newAccountPassword = loadLine();

    users[idLoggedInUser - 1].password = newAccountPassword;
    savingUserDataAfterChange(users);

    cout << "Password changed corretly." << endl;
    cout << endl;
    system("pause");
}

void closeProgramm() {
    system("cls");
    cout << "Thanks for using our programm!";
    cout << endl;
    exit(0);
}

int loadingToVectorUserFriendsData(vector <FriendData> &friends, int idLoggedInUser) {
    int idToLoad = 0;
    string laneToLoadFromTXT;
    fstream friendsList;
    FriendData singleFriend;
    string dataOfSingleFriend{};
    int lastFriendID = 0;

    friendsList.open("friendsList.txt",ios::in);
    if (friendsList.good() == false) {
        return 0;
    }
    while (getline(friendsList, dataOfSingleFriend)) {
        string singleFriendalData{};
        int singlePersonNumber = 1;

        for (size_t index{}; index < dataOfSingleFriend.length(); ++index) {
            if (dataOfSingleFriend[index] != '|') {
                singleFriendalData += dataOfSingleFriend[index];
            } else {

                switch(singlePersonNumber) {
                case 1:
                    singleFriend.friendId = stoi(singleFriendalData);
                    break;
                case 2:
                    singleFriend.userId = stoi(singleFriendalData);
                    break;
                case 3:
                    singleFriend.name = singleFriendalData;
                    break;
                case 4:
                    singleFriend.lastName = singleFriendalData;
                    break;
                case 5:
                    singleFriend.telNumber = singleFriendalData;
                    break;
                case 6:
                    singleFriend.eMail = singleFriendalData;
                    break;
                case 7:
                    singleFriend.adress = singleFriendalData;
                    break;
                }
                lastFriendID = singleFriend.friendId;
                singleFriendalData = "";
                singlePersonNumber++;
            }
        }
        if (singleFriend.userId == idLoggedInUser) {
            friends.push_back(singleFriend);
        }
    }
    friendsList.close();
    return lastFriendID;
}

int addFriendToList(vector <FriendData> &friends, int idLoggedInUser, int lastFriendId) {
    FriendData newFriend;
    system("cls");

    newFriend.friendId = ++lastFriendId;
    newFriend.userId = idLoggedInUser;

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

    friends.push_back(newFriend);

    fstream friendsList;
    friendsList.open("friendsList.txt", ios::out | ios::app);

    friendsList << newFriend.friendId << "|";
    friendsList << newFriend.userId << "|";
    friendsList << newFriend.name << "|";
    friendsList << newFriend.lastName << "|";
    friendsList << newFriend.telNumber << "|";
    friendsList << newFriend.eMail << "|";
    friendsList << newFriend.adress << endl;

    friendsList.close();

    cout << "Friend added successfully!" << endl;
    cout << endl;
    system("pause");
    return lastFriendId;
}

void showFriendsByName(vector <FriendData> &friends) {
    system("cls");
    cout << "Enter the name to find: ";
    string nameToFind = loadLine();
    int occurrenceNameToFind = 0;

    for (unsigned int i = 0; i < friends.size(); i++) {
        if (friends[i].name == nameToFind) {
            occurrenceNameToFind++;
            showSingleFriend(friends, i);
            cout << endl;
        }
    }
    if (!occurrenceNameToFind) {
        cout << "Friend " << nameToFind << " not found." << endl;
        cout << endl;
        system("pause");
    } else {
        system("pause");
    }
}

void showFriendsByLastName(vector <FriendData> &friends) {
    system("cls");
    cout << "Enter the last name to find: ";
    string nameToFind = loadLine();
    int occurrenceLastNameToFind = 0;

    for (unsigned int i = 0; i < friends.size(); i++) {
        if (friends[i].lastName == nameToFind) {
            occurrenceLastNameToFind++;
            showSingleFriend(friends, i);
            cout << endl;
        }
    }
    if (!occurrenceLastNameToFind) {
        cout << "Friend " << nameToFind << " not found." << endl;
        cout << endl;
        system("pause");
    } else {
        system("pause");
    }
}

void showAllFriends(vector <FriendData> &friends) {
    system("cls");
    if (friends.empty()) {
        cout << "Friend list is empty." << endl;
        cout << endl;
        system("pause");
    } else {
        for(unsigned int i = 0; i < friends.size(); i++) {
            showSingleFriend(friends, i);
        }
        cout << endl;
        system("pause");
    }
}

void showSingleFriend(vector <FriendData> &friends, int friendToShowId) {
    cout << endl;
    cout << "ID:               " << friends[friendToShowId].friendId << endl;
    cout << "Name:             " << friends[friendToShowId].name << endl;
    cout << "Last name:        " << friends[friendToShowId].lastName << endl;
    cout << "Telephone number: " << friends[friendToShowId].telNumber << endl;
    cout << "eMail adress:     " << friends[friendToShowId].eMail << endl;
    cout << "Adress            " << friends[friendToShowId].adress << endl;
}

int deletingContact(vector <FriendData> &friends, int lastFriendID) {
    system("cls");
    if (friends.empty()) {
        cout << "Friend list is empty." << endl;
        cout << endl;
        system("pause");
        return 0;
    } else {
        int idToDelete;
        char deleteOrNot;

        cout << "Contact delete function." << endl;
        cout << endl;
        cout << "Please enter friend ID to be deleted: ";
        cin >> idToDelete;

        bool doesIdExist = false;

        for (unsigned int i = 0; i < friends.size(); i++) {
            if (friends[i].friendId == idToDelete) {
                doesIdExist = true;
                break;
            }
        }
        if (!doesIdExist) {
            cout << endl;
            cout << "The entered ID does not exist." << endl;
            system("pause");
        } else {
            cout << "Delete the contact? t/n : ";
            cin >> deleteOrNot;

            if (deleteOrNot == 't') {
                for (unsigned int i = 0; i < friends.size() ; i++) {
                    if (friends[i].friendId == idToDelete) {
                        auto it = friends.begin() + i;
                        friends.erase(it);
                    }
                }
                cout << "Contact deleted." << endl;
                cout << endl;
                system("pause");
                lastFriendID = saveFileAfterFriendRemove(friends, idToDelete);
            } else {
                cout << "The contact has not been deleted." << endl;
                cout << endl;
                system("pause");
            }
        }
    }
    return lastFriendID;
}

int saveFileAfterFriendRemove(vector <FriendData> &friends, int idToDelete) {
    int lastFriendId;
    FriendData singleFriend;
    string dataOfSingleFriend{};

    fstream friendsList;
    friendsList.open("friendsList.txt",ios::in);
    if (!friendsList.good()) {
        return 0;
    }

    fstream temporaryFriendsList;
    temporaryFriendsList.open("temporaryFriendsList.txt", ios::out | ios::app);

    while (getline(friendsList, dataOfSingleFriend)) {
        string singleFriendalData{};
        int singlePersonNumber = 1;

        for (size_t index{}; index < dataOfSingleFriend.length(); ++index) {
            if (dataOfSingleFriend[index] != '|') {
                singleFriendalData += dataOfSingleFriend[index];
            } else {
                switch(singlePersonNumber) {
                case 1:
                    singleFriend.friendId = stoi(singleFriendalData);
                    break;
                case 2:
                    singleFriend.userId = stoi(singleFriendalData);
                    break;
                case 3:
                    singleFriend.name = singleFriendalData;
                    break;
                case 4:
                    singleFriend.lastName = singleFriendalData;
                    break;
                case 5:
                    singleFriend.telNumber = singleFriendalData;
                    break;
                case 6:
                    singleFriend.eMail = singleFriendalData;
                    break;
                case 7:
                    singleFriend.adress = singleFriendalData;
                    break;
                }
                if (singleFriend.friendId != idToDelete) {
                    lastFriendId = singleFriend.friendId;
                }
                singleFriendalData = "";
                singlePersonNumber++;
            }
        }
        if (singleFriend.friendId != idToDelete) {
            temporaryFriendsList <<  singleFriend.friendId << "|";
            temporaryFriendsList << singleFriend.userId << "|";
            temporaryFriendsList <<  singleFriend.name << "|";
            temporaryFriendsList << singleFriend.lastName << "|";
            temporaryFriendsList << singleFriend.telNumber << "|";
            temporaryFriendsList << singleFriend.eMail << "|";
            temporaryFriendsList << singleFriend.adress << "|" << endl;
        }
    }
    temporaryFriendsList.close();
    friendsList.close();

    remove("friendsList.txt");
    rename("temporaryFriendsList.txt","friendsList.txt");
    return lastFriendId;
}

void editFriendData (vector <FriendData> &friends) {
    system("cls");
    int friendDataToChangeID;
    int userChoice;
    int possitionInLocalVector;
    string singleDataToChange = "";

    if (friends.empty()) {
        cout << "Friend list is empty." << endl;
        cout << endl;
        system("pause");
    }

    else {
        cout << "Please enter a friend ID: ";
        cin >> friendDataToChangeID;
        bool doesIdExist = false;

        for (unsigned int i = 0; i < friends.size(); i++) {
            if (friends[i].friendId == friendDataToChangeID) {
                doesIdExist = true;
                possitionInLocalVector = i;
                break;
            }
        }
        if (!doesIdExist) {
            cout << endl;
            cout << "The entered ID does not exist." << endl;
            cout << endl;
            system("pause");
            return;
        } else {
            cout << endl;

            cout << "Select data to change: " << endl;
            cout << "1. Name." << endl;
            cout << "2. Last name." << endl;
            cout << "3. Telephone number." << endl;
            cout << "4. Email adress. " << endl;
            cout << "5. Adress." << endl;
            cout << "6. Back to the menu." << endl;
            cout << endl;
            cout << "Your choice: ";
            cin >> userChoice;

            switch (userChoice) {
            case 1:
                cout << "Please enter a new name: " << endl;
                singleDataToChange = loadLine();
                friends[possitionInLocalVector].name = singleDataToChange;
                cout << "Name changed corretly." << endl;
                cout << endl;
                system("pause");
                break;

            case 2:
                cout << "Please enter new last name: " << endl;
                singleDataToChange = loadLine();
                friends[possitionInLocalVector].lastName = singleDataToChange;
                cout << "Last name changed corretly." << endl;
                cout << endl;
                system("pause");
                break;

            case 3:
                cout << "Please enter a new telephone number: " << endl;
                singleDataToChange = loadLine();
                friends[possitionInLocalVector].telNumber = singleDataToChange;
                cout << "Telephone number changed corretly." << endl;
                cout << endl;
                system("pause");
                break;

            case 4:
                cout << "Please enter a new email adress: " << endl;
                singleDataToChange = loadLine();
                friends[possitionInLocalVector].eMail = singleDataToChange;
                cout << "Email adress changed corretly." << endl;
                cout << endl;
                system("pause");
                break;

            case 5:
                cout << "Please enter a new adress: " << endl;
                singleDataToChange = loadLine();
                friends[possitionInLocalVector].adress = singleDataToChange;
                cout << "Adress changed corretly." << endl;
                cout << endl;
                system("pause");
                break;

            case 6:
                break;

            default:
                cout << "Select an option from 1 to 6." << endl;
                system("pause");
            }
        }
        saveFileAfterFriendDataEdit(friends, friendDataToChangeID, possitionInLocalVector);
    }
}

void saveFileAfterFriendDataEdit(vector <FriendData> &friends, int editedFriendId, int possitionInLocalVector) {
    FriendData singleFriend;
    string dataOfSingleFriend{};

    fstream friendsList;
    friendsList.open("friendsList.txt",ios::in);
    if (friendsList.good() == false) {
        return;
    }

    fstream temporaryFriendsList;
    temporaryFriendsList.open("temporaryFriendsList.txt", ios::out | ios::app);

    while (getline(friendsList, dataOfSingleFriend)) {
        string singleFriendalData{};
        int singlePersonNumber = 1;

        for (size_t index{}; index < dataOfSingleFriend.length(); ++index) {
            if (dataOfSingleFriend[index] != '|') {
                singleFriendalData += dataOfSingleFriend[index];
            } else {
                switch(singlePersonNumber) {
                case 1:
                    singleFriend.friendId = stoi(singleFriendalData);
                    break;
                case 2:
                    singleFriend.userId = stoi(singleFriendalData);
                    break;
                case 3:
                    singleFriend.name = singleFriendalData;
                    break;
                case 4:
                    singleFriend.lastName = singleFriendalData;
                    break;
                case 5:
                    singleFriend.telNumber = singleFriendalData;
                    break;
                case 6:
                    singleFriend.eMail = singleFriendalData;
                    break;
                case 7:
                    singleFriend.adress = singleFriendalData;
                    break;
                }
                singleFriendalData = "";
                singlePersonNumber++;
            }
        }
        if (singleFriend.friendId != editedFriendId) {
            temporaryFriendsList <<  singleFriend.friendId << "|";
            temporaryFriendsList << singleFriend.userId << "|";
            temporaryFriendsList <<  singleFriend.name << "|";
            temporaryFriendsList << singleFriend.lastName << "|";
            temporaryFriendsList << singleFriend.telNumber << "|";
            temporaryFriendsList << singleFriend.eMail << "|";
            temporaryFriendsList << singleFriend.adress << "|" << endl;

        } else if (singleFriend.friendId == editedFriendId) {
            temporaryFriendsList << friends[possitionInLocalVector].friendId << "|";
            temporaryFriendsList << friends[possitionInLocalVector].userId << "|";
            temporaryFriendsList << friends[possitionInLocalVector].name << "|";
            temporaryFriendsList << friends[possitionInLocalVector].lastName << "|";
            temporaryFriendsList << friends[possitionInLocalVector].telNumber << "|";
            temporaryFriendsList << friends[possitionInLocalVector].eMail << "|";
            temporaryFriendsList << friends[possitionInLocalVector].adress << "|" << endl;
        }
    }
    temporaryFriendsList.close();
    friendsList.close();

    remove("friendsList.txt");
    rename("temporaryFriendsList.txt", "friendsList.txt");
}
