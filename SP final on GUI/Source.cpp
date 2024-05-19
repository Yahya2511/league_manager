#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <time.h>

using namespace std;

//Structures declaration.

//date
struct Date
{
	int day = 0;
	int month = 0;
	int year = 0;
};

//player
struct Player
{
	int id = 0;
	string name;
	string position;
	string DOB;
	string nationality;
	int goals = 0;
	string joindTeam;
};

//team
struct Team
{
	int id = 0;
	int  numPlayers = 0;
	bool ready = false;
	string name;
	Player players[34];
};

//result
struct Result
{
	int id = 0;
	int TeamHomeGoals = 0;
	int TeamAwayGoals = 0;
	bool played = false;
};

//match
struct Match
{
	int id = 0, StartTime;
	string TeamHome, TeamAway;
	Date StartDate;
	Result Result;
};

//Table Record
struct Table
{
	Team team;
	int points = 0;
	int goals_for = 0;
	int goals_against = 0;
	int goals_diff = 0;
	int wins = 0;
	int losses = 0;
	int draws = 0;
};

//League
struct League
{
	int idGen = 868;
	int id = 0;
	int numTeams = 0;
	int readyTeams = 0;
	Team teams[20];
	Match Matches[190];
	Table table[20];
	bool gnrtd = 0;
	int lastResult = 1;
};

// User structure
struct User
{
	string username;
	string password;
	bool Role;
};

int numUsers = 0;
int activeUsr = -1;

//functions declaration
void signup(User u[], string usresFileName);
void addAdmin(User u[], string usresFileName);
void login(User u[]);
void logout();

void loadUsers(User user[], string fileName);
void saveUsers(User user[], string fileName);

int adChoose1(League& fcis, string, User u[], string usersFileName);
int adChoose2(League& fcis, string, User u[], string usersFileName);
int usChoose(League& fcis, string fileName);

bool gnrtMtches(League& l);
void gnrtSchdl(League& l);
void viewSchdl(League& l);
void incDate(int&, int&, int&);

int getInt(const std::string& userOutput);

void loadLeague(League& l, string fileName);
void saveLeagueData(const League& l, string fileName);

void viewLeague(const League& l);

void viewAllTeamsData(const League& l);
void viewCertainTeam(const League& l, int teamIndex);

void viewAllTeamPlayersData(const League& l, int teamIndex);
void viewCertainPlayerdata(const League& l, int teamIndex, int playerIndex);

void addTeam(League& l);
void getTeamData(League& l);

void addPlayer(League& l, int playersToAdd, int playerIndex);
void getPlayersData(League& l, int playersToAdd, int teamIndex);

void deleteTeam(League& l, int teamIndex);
void deletePlayer(League& l, int teamIndex, int playerIndex);

void editTeamName(League& l, int teamIndex);
void editPlayerInfo(League& l, int teamIndex, int playerIndex);

void enterResult(League& l);
void viewResult(League& l);

void update_table(League& league);
void view_table(League& league);

void addGoal(League& l, int goals, int teamIndex);
void setTopPlayers(League& l);
void search(League l);


int main()
{
	User users[100];
	League fcis;

	//filling the league struct and user struct variable from the file.

	string leagueFileName = "Data.txt";
	string usersData = "users.txt";

	loadLeague(fcis, leagueFileName);
	loadUsers(users, usersData);

	std::cout << "\t\tWelcome to FCIS League System\n" <<
		"\t*********************************************\n";

	int fn;
	bool exit = 0;
	do
	{
		do
		{
			int choice;
			bool exitProgram = false;
			std::cout << "1. Sign up" << endl;
			std::cout << "2. Log in" << endl;
			std::cout << "3. Exit" << endl;
			choice = getInt("Enter your choice: ");

			switch (choice)
			{
			case 1:
				signup(users, usersData);
				break;
			case 2:
				login(users);
				break;
			case 3:
				exit = 1;
				break;
			default:
				std::cout << "Invalid choice." << endl;
				break;
			}
		} while (activeUsr < 0 && !exit);

		if (exit)break;
		fn = 0;
		if (users[activeUsr].Role)
		{
			//12: generate schedule
			//13: logut
			//14: exit
			while (fn != 14 && fn != 13 && fn != 12 && !fcis.gnrtd)
			{
				fn = adChoose1(fcis, leagueFileName, users, usersData);
			}
			if (fn == 14)exit = 1;

			if (fn != 14 && fn != 13)
			{
				//11: logut
				//12: exit
				do {
					fn = adChoose2(fcis, leagueFileName, users, usersData);
				} while (fn != 12 && fn != 11);
			}
			if (fn == 12)exit = 1;
		}

		else
		{
			//8:logout
			//9:exit
			do {
				fn = usChoose(fcis, leagueFileName);
			} while (fn != 8 && fn != 9);
			if (fn == 9)exit = 1;
		}
	} while (!exit);

	saveUsers(users, usersData);
	system("pause");
	return 0;
}


//avoid incorrect input
int getInt(const std::string& userOutput)
{
	bool flage = false;
	string stringTocheck;
	while (flage == false)
	{
		flage = true;
		std::cout << userOutput;
		std::getline(cin, stringTocheck);

		if (stringTocheck.length() > 8)
		{
			std::cout << "STOP PLAYING!!!\n\n";
			flage = false;
			continue;
		}

		if (stringTocheck.empty())
		{
			flage = false;
			continue;
		}
		for (int i = 0; i < stringTocheck.length(); i++)
		{
			if (!isdigit(stringTocheck[i]))
			{
				std::cout << "Input error! (no letters allowed)\n\n";
				flage = false;
				break;
			}
		}
	}

	return stoi(stringTocheck);
}

//fill league struct with data
void loadLeague(League& l, string fileName)
{
	//openning the file

	ifstream data;

	data.open(fileName);

	//if file op. fialed.
	if (data.fail())
	{
		std::cout << "Error opening file!!!!" << endl;
		std::exit(1);
	}

	//filling the struct league
	int teamIndex = 0;
	int playerIndex = 0;

	string checker;

	while (checker != "\\")
	{

		//team name.
		std::getline(data, l.teams[teamIndex].name, '|');

		int numOfPlayers;
		data >> numOfPlayers;
		data.ignore();
		l.teams[teamIndex].numPlayers = numOfPlayers;

		if (numOfPlayers > 21)
			l.teams[teamIndex].ready = true, l.readyTeams++;

		for (playerIndex = 0; playerIndex < numOfPlayers; playerIndex++)
		{

			//player postion.
			std::getline(data, l.teams[teamIndex].players[playerIndex].position, '|');

			//reading name.
			std::getline(data, l.teams[teamIndex].players[playerIndex].name, '|');

			//reading nationality.
			std::getline(data, l.teams[teamIndex].players[playerIndex].nationality, '|');

			//date
			std::getline(data, l.teams[teamIndex].players[playerIndex].DOB);
		}
		std::getline(data, checker);
		teamIndex++;
	}
	l.numTeams = teamIndex;

	int maxIdReached = 0;
	for (int teamIndex = 0; teamIndex < l.numTeams; teamIndex++)
	{


		//reading team id.
		data >> l.teams[teamIndex].id;;
		for (playerIndex = 0; playerIndex < l.teams[teamIndex].numPlayers; playerIndex++)
		{
			//player ID.
			data >> l.teams[teamIndex].players[playerIndex].id;
			if (l.teams[teamIndex].players[playerIndex].id > maxIdReached)
				maxIdReached = l.teams[teamIndex].players[playerIndex].id;
		}
	}

	l.idGen = (maxIdReached + 1);

	data.close();
}

//stores league data in the file
void saveLeagueData(const League& l, string fileName)
{
	char choice = 'y';
	std::cout << "Are you sure you want to save?(y/n)\n" <<
		"This will overwrite any previous data: ";
	std::cin >> choice;
	cin.ignore();
	std::cout << endl;

	//canceling the process.
	if (choice != 'y' && choice != 'Y')
	{
		std::cout << "Saving process cancelled!\n";
		return;
	}

	ofstream data;

	data.open(fileName);

	//if file op. fialed.
	if (data.fail())
	{
		std::cout << "Error saving the file!!!!" << endl;
		std::exit(1);
	}

	for (int teamIndex = 0; teamIndex < l.numTeams; teamIndex++)
	{
		data << l.teams[teamIndex].name << '|' << endl;
		data << l.teams[teamIndex].numPlayers << endl;

		for (int playerIndex = 0; playerIndex < l.teams[teamIndex].numPlayers; playerIndex++)
		{
			data << l.teams[teamIndex].players[playerIndex].position << '|' <<
				l.teams[teamIndex].players[playerIndex].name << '|' <<
				l.teams[teamIndex].players[playerIndex].nationality << '|';
			data << l.teams[teamIndex].players[playerIndex].DOB << endl;

		}
		if (teamIndex == l.numTeams - 1)
			data << "\\" << endl;
		else
			data << "|" << endl;
	}
	for (int teamIndex = 0; teamIndex < l.numTeams; teamIndex++)
	{
		data << l.teams[teamIndex].id << endl;
		for (int playerIndex = 0; playerIndex < l.teams[teamIndex].numPlayers; playerIndex++)
		{
			data << l.teams[teamIndex].players[playerIndex].id << endl;
		}
	}

	data.close();
}

//fill user struct with data
void loadUsers(User user[], string fileName)
{
	ifstream usersFile;

	usersFile.open(fileName);

	//if file op. fialed.
	if (usersFile.fail())
	{
		std::cout << "Error opening file!!!!" << endl;
		std::exit(1);
	}

	int userIndex = 0;
	int numberOfUsers;

	usersFile >> numberOfUsers;
	usersFile.ignore();

	for (userIndex = 0; userIndex < numberOfUsers; userIndex++)
	{
		string admin;
		std::getline(usersFile, user[userIndex].username, '|');

		std::getline(usersFile, user[userIndex].password, '|');

		std::getline(usersFile, admin);

		if (admin == "admin")
		{
			user[userIndex].Role = true;
			numUsers++;
		}
		else
		{
			user[userIndex].Role = false;
			numUsers++;
		}
	}
}

//stores user data in the file
void saveUsers(User user[], string fileName)
{
	ofstream usersFile;

	usersFile.open(fileName);

	//if file op. fialed.
	if (usersFile.fail())
	{
		std::cout << "Error opening file!!!!" << endl;
		std::exit(1);
	}

	usersFile << (numUsers) << endl;

	for (int userIndex = 0; userIndex < (numUsers); userIndex++)
	{
		usersFile << user[userIndex].username << '|' <<
			user[userIndex].password << '|';
		if (user[userIndex].Role)
			usersFile << "admin\n";
		else
			usersFile << "user\n";
	}
}

//create new account
void signup(User u[], string usresFileName)
{
	string username, password;
	std::cout << "Enter username: ";
	cin >> username;
	cin.ignore();
	std::cout << "Enter password: ";
	cin >> password;
	cin.ignore();

	// Check if user already exists in the users array
	for (int i = 0; i < numUsers; i++)
	{
		if (u[i].username == username)
		{
			std::cout << "Username already exists." << endl;
			return;
		}
	}

	// Add user to the users array
	User newUser = { username, password, false };
	u[numUsers] = newUser;
	numUsers++;

	std::cout << "User signed up successfully." << endl;
}
//add a new admin
void addAdmin(User u[], string usresFileName)
{
	string username, password;
	std::cout << "Enter username: ";
	cin >> username;
	cin.ignore();
	std::cout << "Enter password: ";
	cin >> password;
	cin.ignore();

	// Check if user already exists in the users array
	for (int i = 0; i < numUsers; i++)
	{
		if (u[i].username == username)
		{
			std::cout << "Username already exists." << endl;
			return;
		}
	}

	// Add user to the users array
	User newUser = { username, password, true };
	u[numUsers] = newUser;
	numUsers++;

	std::cout << "Admin added successfully." << endl;
}
//login into system
void login(User u[])
{
	string username, password;
	int attempts = 0;
	bool loggedIn = false;
	while (attempts < 3 && !loggedIn)
	{
		std::cout << "Enter username: ";
		cin >> username;
		cin.ignore();
		std::cout << "Enter password: ";
		cin >> password;
		cin.ignore();

		// Check if user exists in the users array
		for (int i = 0; i < (numUsers); i++)
		{
			if (u[i].username == username && u[i].password == password)
			{
				std::cout << "Logged in successfully." << endl;
				loggedIn = true;
				activeUsr = i;
				break;
			}
		}

		if (!loggedIn)
		{
			std::cout << "Invalid username or password. Please try again." << endl;
			attempts++;
		}
	}

	if (!loggedIn)
	{
		std::cout << "Maximum login attempts reached. Please try again later." << endl;
	}
}
//logout of system
void logout() {
	activeUsr = -1;
	std::cout << "Logged out." << endl;
}

//admin's functions before generating schedule
int adChoose1(League& fcis, string fileName, User u[], string usersFileName)
{
	int teamIndex = 0, playerIndex = 0;
	int fn;
	std::cout << "\nPlease choose a function from below : \n\n" <<
		"1.View All Teams and all of it's data\n" <<
		"2.View Teams Info\n" <<
		"3.View Player Info\n" <<
		"4.Add a Team\n" <<
		"5.Edit a Team\n" <<
		"6.Delete a Team\n" <<
		"7.Add a Player\n" <<
		"8.Edit a Player\n" <<
		"9.Delete a Player\n" <<
		"10.Search for a player\n" <<
		"11.Add admin\n" <<
		"12.Generate the Schedule\n" <<
		"13.Logout\n" <<
		"14.Exit\n\n";
	fn = getInt("Enter the number of the function : ");
	std::cout << endl;
	switch (fn)
	{
		//view all
	case 1:
		viewLeague(fcis);
		std::cout << endl;
		break;
		//view teams
	case 2:
		viewAllTeamsData(fcis);
		std::cout << endl;
		break;
		//view players
	case 3:
	{
		viewAllTeamsData(fcis);

		teamIndex = getInt("Enter team index to view it's players : ");
		//error check. 
		if (teamIndex > fcis.numTeams)
		{
			std::cout << "Error! Invalid team index.\n";
			break;
		}

		std::cout << endl;

		viewAllTeamPlayersData(fcis, teamIndex - 1);
		break;
	}
	//add team
	case 4:
		addTeam(fcis); break;
		//edit team
	case 5:
		viewAllTeamsData(fcis);
		//if no teams.
		if (fcis.numTeams == 0)
		{
			break;
		}

		teamIndex = getInt("\nEnter team index to edit it's name: ");
		if (teamIndex > fcis.numTeams)
		{
			std::cout << "Error! Invalid team index.\n";
			break;
		}

		editTeamName(fcis, teamIndex - 1);
		break;
		//delete team
	case 6:

		viewAllTeamsData(fcis);
		//if no teams.
		if (fcis.numTeams == 0)
		{
			break;
		}

		teamIndex = getInt("\nEnter team index to delete it: ");
		if (teamIndex > fcis.numTeams)
		{
			std::cout << "Error! Invalid team index.\n";
			break;
		}

		deleteTeam(fcis, teamIndex - 1);
		break;
		//add palyer
	case 7:
	{
		viewAllTeamsData(fcis);

		teamIndex = getInt("\nEnter team index to add player(s) to it: ");
		if (teamIndex > fcis.numTeams || teamIndex == 0)
		{
			std::cout << "Error! Invalid team index.\n";
			break;
		}

		int playeraToAdd;
		playeraToAdd = getInt("\nEnter number of players you want to add: ");


		addPlayer(fcis, playeraToAdd, teamIndex - 1);
		viewAllTeamPlayersData(fcis, teamIndex - 1);
		break;
	}
	//edit player
	case 8:
		viewAllTeamsData(fcis);
		//if no teams.
		if (fcis.numTeams == 0)
		{
			break;
		}
		teamIndex = getInt("\nFirst, Enter team index that you want to edit a player from it: ");
		if (teamIndex > fcis.numTeams || teamIndex == 0)
		{
			std::cout << "Error! Invalid team index.\n";
			break;
		}

		viewAllTeamPlayersData(fcis, teamIndex - 1);
		//if no players.
		if (fcis.teams[teamIndex - 1].numPlayers == 0)
		{
			std::cout << "There is no player!!\n\n";
			break;
		}

		playerIndex = getInt("Second, Enter player index to edit him: ");
		if (playerIndex > fcis.teams[teamIndex - 1].numPlayers)
		{
			std::cout << "Error! Invalid player index.\n";
			break;
		}

		editPlayerInfo(fcis, teamIndex - 1, playerIndex - 1);
		break;
		//delete player
	case 9:
		viewAllTeamsData(fcis);
		//if no teams.
		if (fcis.numTeams == 0)
		{
			break;
		}
		teamIndex = getInt("\nFirst, Enter team index that you want to delete from it: ");
		if (teamIndex > fcis.numTeams || teamIndex == 0)
		{
			std::cout << "Error! Invalid team index.\n";
			break;
		}

		viewAllTeamPlayersData(fcis, teamIndex - 1);
		//if no players.
		if (fcis.teams[teamIndex - 1].numPlayers == 0)
		{
			std::cout << "There is no player!!\n\n";
			break;
		}

		playerIndex = getInt("Second, Enter player index to delete him: ");
		if (playerIndex > fcis.teams[teamIndex - 1].numPlayers)
		{
			std::cout << "Error! Invalid player index.\n";
			break;
		}

		deletePlayer(fcis, teamIndex - 1, playerIndex - 1);
		break;
	case 10:

		search(fcis);
		break;
	case 11:
		addAdmin(u, usersFileName); break;
	case 12:
	{
		bool ok = gnrtMtches(fcis);
		if (!ok)fn *= -1;
		break;
	}
	case 13:
		logout(); break;
	case 14:
		char okSave;
		std::cout << "Do you want to save before closeing the program?(y/n): ";
		std::cin >> okSave;
		cin.ignore();


		if (okSave == 'y' || okSave == 'Y')
		{
			saveLeagueData(fcis, fileName);
		}
		break;
	default:
		std::cout << "Sorry there is no function with this number, please try agian.\n";
	}
	return fn;
}
//admin's functions after generating schedule
int adChoose2(League& fcis, string fileName, User u[], string usersFileName)
{
	int teamIndex;
	int fn;
	std::cout << "\nPlease choose a function from below : \n\n" <<
		"1.View All Teams and all of it's data\n" <<
		"2.View Teams Info\n" <<
		"3.View Player Info\n" <<
		"4.View Schedule\n" <<
		"5.Enter the result of match(es)\n" <<
		"6.View the result of match(es)\n" <<
		"7.View the table\n" <<
		"8.View top scoring Players\n" <<
		"9.Search for players\n" <<
		"10.Add admin\n" <<
		"11.Logout\n" <<
		"12.Exit\n\n";
	fn = getInt("Enter the number of the function : ");
	switch (fn)
	{
		//view all
	case 1:
		viewLeague(fcis);
		std::cout << endl;
		break;
		//view teams
	case 2:
		viewAllTeamsData(fcis);
		std::cout << endl;
		break;
		//view players
	case 3:
	{
		viewAllTeamsData(fcis);
		teamIndex = getInt("Enter team index to view it's players : ");
		//error check. 
		if (teamIndex > fcis.numTeams)
		{
			std::cout << "Error! Invalid team index.\n";
			break;
		}

		std::cout << endl;

		viewAllTeamPlayersData(fcis, teamIndex - 1);
		break;
	}
	case 4:
		viewSchdl(fcis); break;
	case 5:
		enterResult(fcis); break;
	case 6:
		viewResult(fcis); break;
	case 7:
		view_table(fcis); break;
	case 8:
		setTopPlayers(fcis);
		break;
	case 9:
		search(fcis);
		break;
	case 10:
		addAdmin(u, usersFileName); break;
	case 11:
		logout(); break;
	case 12:
		char okSave;
		std::cout << "Do you want to save before closeing the program?(y/n): ";
		std::cin >> okSave;
		cin.ignore();

		if (okSave == 'y' || okSave == 'Y')
		{
			saveLeagueData(fcis, fileName);
		}
		break;
	default:
		std::cout << "Sorry there is no function with this number, please try again.\n";
	}
	return fn;
}
//user's functions
int usChoose(League& fcis, string fileName)
{
	int teamIndex = 0, playerIndex = 0;
	int fn;
	std::cout << "\nPlease choose a function from below : \n\n" <<
		"1.View All Teams and all of it's data\n" <<
		"2.View Teams Info\n" <<
		"3.View Player Info\n" <<
		"4.View the result of match(es)\n" <<
		"5.View the table\n" <<
		"6.View top scoring players\n" <<
		"7.Search for players\n" <<
		"8.Log out\n" <<
		"9.Exit\n\n";
	fn = getInt("Enter the number of the function : ");
	std::cout << endl;
	switch (fn)
	{
		//view all
	case 1:
		viewLeague(fcis);
		std::cout << endl;
		break;
		//view teams
	case 2:
		viewAllTeamsData(fcis);
		std::cout << endl;
		break;
		//view players
	case 3:
	{
		viewAllTeamsData(fcis);
		teamIndex = getInt("Enter team index to view it's players : ");
		//error check. 
		if (teamIndex > fcis.numTeams)
		{
			std::cout << "Error! Invalid team index.\n";
			break;
		}

		std::cout << endl;

		viewAllTeamPlayersData(fcis, teamIndex - 1);
		break;
	}
	case 4:
	{
		if (!fcis.gnrtd)
		{
			std::cout << "League hasn't started yet\n";
			return 4;
		}
		viewResult(fcis);
		break;
	}
	case 5:
	{
		view_table(fcis);
		break;
	}
	case 6:
		setTopPlayers(fcis);
		break;
	case 7:
		search(fcis);
		break;
	case 8:
		logout(); break;
	case 9:
		break;
	default:
		std::cout << "Sorry there is no function with this number, please try agian.\n";
	}
	return fn;
}

//view all teams and players of the league
void viewLeague(const League& l)
{
	for (int teamIndex = 0; teamIndex < l.numTeams; teamIndex++) {

		viewAllTeamPlayersData(l, teamIndex);
	}
}

//view all teams
void viewAllTeamsData(const League& l)
{
	if (l.numTeams == 0)
	{
		std::cout << "There is no teams yet!!!!\n\n";
		return;
	}
	for (int teamIndex = 0; teamIndex < l.numTeams; teamIndex++)
	{
		viewCertainTeam(l, teamIndex);
	}
	std::cout << endl;
	if (l.readyTeams == 20)
		std::cout << "All teams are ready.\n";
	else
		std::cout << l.readyTeams << " teams are ready and " << 20 - l.readyTeams << " are remaining\n";
}
//view a certain team
void viewCertainTeam(const League& l, int teamIndex)
{
	int nameLen = 20 - l.teams[teamIndex].name.length();

	std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	std::cout << '(' << teamIndex + 1 << ')' << " -> " <<
		l.teams[teamIndex].name;
	for (int i = 0; i < nameLen; i++)
	{
		std::cout << ' ';
	}
	std::cout << l.teams[teamIndex].id << "  |  " <<
		l.teams[teamIndex].numPlayers << "  |  ";
	if (l.teams[teamIndex].ready == true)
		std::cout << "team is ready\n";
	else
		std::cout << "not ready still need " << 22 - l.teams[teamIndex].numPlayers << " player(s)!\n";
	std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
}

//view all players of a team
void viewAllTeamPlayersData(const League& l, int teamIndex)
{
	//error check.
	if (l.teams[teamIndex].numPlayers < 1)
	{
		std::cout << "There is no players yet!\n";
		return;
	}
	viewCertainTeam(l, teamIndex);

	int playerNum = l.teams[teamIndex].numPlayers;
	for (int playerIndex = 0; playerIndex < playerNum; playerIndex++) {
		viewCertainPlayerdata(l, teamIndex, playerIndex);
	}
	std::cout << endl;
}
//view a certain player
void viewCertainPlayerdata(const League& l, int teamIndex, int playerIndex)
{
	string nameLen = { l.teams[teamIndex].players[playerIndex].name };

	int spacesToAdd = 30 - nameLen.length();

	std::cout << '(' << playerIndex + 1 << ')' << "  -> " <<
		l.teams[teamIndex].players[playerIndex].name;
	for (int i = 0; i < spacesToAdd; i++)
	{
		std::cout << ' ';
	}
	std::cout << l.teams[teamIndex].players[playerIndex].position << " | " <<
		l.teams[teamIndex].players[playerIndex].id << " | " <<
		l.teams[teamIndex].players[playerIndex].nationality << " | " <<
		l.teams[teamIndex].players[playerIndex].DOB << endl;
	if (l.gnrtd && l.teams[teamIndex].players[playerIndex].goals > 0)
	{
		std::cout << "Scored goals: " << l.teams[teamIndex].players[playerIndex].goals << endl;
	}
	std::cout << "------------------------------------------------------------------------------" << endl;
}

//add a new team
void addTeam(League& l)
{
	if (l.numTeams > 19)
	{
		std::cout << "Error max number of teams is 20!\n\n";
		return;
	}
	getTeamData(l);

	std::cout << "Team added successfuly!\n\n";

	l.teams[l.numTeams - 1].numPlayers = 0;
	l.teams[l.numTeams - 1].ready = false;

	string intCheck;
	bool flage = true;
	int playersToAdd = 0;

	do
	{
		std::cout << "Enter number of players you want to add.\n" <<
			"NOTE: There must be at least one player in the team.\n";

		string userOP = "Number of players to add: ";

		playersToAdd = getInt(userOP);

	} while (playersToAdd < 1);

	int teamIndex = l.numTeams;
	if (playersToAdd > 21)
		l.teams[teamIndex - 1].ready = true, l.readyTeams++;

	addPlayer(l, playersToAdd, teamIndex - 1);
	viewAllTeamPlayersData(l, l.numTeams - 1);
}
void getTeamData(League& l)
{
	l.teams[l.numTeams].id = l.idGen;
	l.idGen++;

	string tname;
	std::cout << "Enter team name: ";
	std::getline(std::cin, tname);
	l.teams[l.numTeams].name = tname;

	l.numTeams++;

	std::cout << "\n";
}

//add a new player
void addPlayer(League& l, int playersToAdd, int teamIndex)
{
	int remanningNumOfPlayers = 34 - l.teams[teamIndex].numPlayers;

	//if too many players.
	if (playersToAdd > remanningNumOfPlayers)
	{
		std::cout << "Error max number of players is 34!\nYou can add " << remanningNumOfPlayers << " only!\n\n";
		return;
	}

	if (l.teams[teamIndex].ready == false && l.teams[teamIndex].numPlayers + playersToAdd > 21)
	{
		l.readyTeams++;
	}

	if (l.teams[teamIndex].numPlayers + playersToAdd > 21)
	{
		l.teams[teamIndex].ready = true;
	}

	for (int pCount = 1; pCount <= playersToAdd; pCount++)
	{
		getPlayersData(l, pCount, teamIndex);
		std::cout << endl;
	}
	std::cout << "Player(s) added successfuly!\n\n";
	if (l.teams[teamIndex].numPlayers > 21)
		l.teams[teamIndex].ready = true;
}
void getPlayersData(League& l, int playersCount, int teamIndex)
{
	int playersIndex = l.teams[teamIndex].numPlayers;

	std::cout << "Player number " << playersCount << " to add.\n";

	//player ID.
	l.teams[teamIndex].players[playersIndex].id = l.idGen;
	l.idGen++;

	//player name.
	std::cout << "Enter player name : ";
	std::getline(std::cin, l.teams[teamIndex].players[playersIndex].name);


	//player postion.
	std::cout << "Enter player postion (GK), (DEFF), .... :";
	std::getline(std::cin, l.teams[teamIndex].players[playersIndex].position);


	//player nationality.
	std::cout << "Enter player nationality : ";
	std::getline(std::cin, l.teams[teamIndex].players[playersIndex].nationality);

	//player DOB.
	std::cout << "Enter player Date of Birth (DD/MM/YYYY): ";
	std::getline(std::cin, l.teams[teamIndex].players[playersIndex].DOB);

	l.teams[teamIndex].numPlayers++;
	std::cout << '\n';
}

//delete a team
void deleteTeam(League& l, int teamIndex)
{
	int lastTeam = l.numTeams - 1;
	//special case
	if (teamIndex == lastTeam)
	{
		l.numTeams--, l.readyTeams--;
		std::cout << "Team deleted successfully!!\n";
		system("pause");
		return;
	}

	l.teams[teamIndex] = l.teams[lastTeam];
	l.numTeams--, l.readyTeams--;
	std::cout << "Team deleted successfully!!\n";
}
//delete a player
void deletePlayer(League& l, int teamIndex, int playerIndex)
{
	int lastPlayer = l.teams[teamIndex].numPlayers - 1;

	//special case.
	if (playerIndex == lastPlayer)
	{
		l.teams[teamIndex].numPlayers--;
		if (l.teams[teamIndex].numPlayers < 22)
		{
			l.teams[teamIndex].ready = false;
			l.readyTeams--;
			std::cout << "Player deleted successfully!!\n\n";
		}
		return;
	}

	l.teams[teamIndex].players[playerIndex] = l.teams[teamIndex].players[lastPlayer];

	l.teams[teamIndex].numPlayers--;

	//check if team is not ready.

	std::cout << "Player deleted successfully!!\n\n";
}

//edit a team's data
void editTeamName(League& l, int teamIndex)
{
	std::cout << "Enter team's new name: ";
	std::getline(std::cin, l.teams[teamIndex].name);
	std::cout << "\n";
	viewCertainTeam(l, teamIndex);
}
//edit a player's data
void editPlayerInfo(League& l, int teamIndex, int playerIndex)
{
	int choice = 0;


	cout << "Options: \n" <<
		"1.Name\n" <<
		"2.Position\n" <<
		"3.Nationality\n" <<
		"4.Date of birth\n" <<
		"5.All Data\n";

	choice = getInt("Choose the field that you want to change:");

	do
	{

		switch (choice)
		{
		case 1:
			//player name.
			std::cout << "Enter player's new name: ";
			std::getline(std::cin, l.teams[teamIndex].players[playerIndex].name);
			break;
		case 2:
			//player postion.
			std::cout << endl << "Enter player's new postion((GK), (MID), ....): ";
			std::getline(std::cin, l.teams[teamIndex].players[playerIndex].position);
			break;
		case 3:
			//player nationality.
			std::cout << endl << "Enter player's new nationality: ";
			std::getline(std::cin, l.teams[teamIndex].players[playerIndex].nationality);
			break;
		case 4:
			//player DOB.
			std::cout << endl << "Enter player's new date of birth (DD/MM/YYYY): ";
			std::getline(std::cin, l.teams[teamIndex].players[playerIndex].DOB);
			break;
		case 5:
			//player name.
			std::cout << "Enter player's new name: ";
			std::getline(std::cin, l.teams[teamIndex].players[playerIndex].name);

			//player postion.
			std::cout << endl << "Enter player's new postion((GK), (MID), ....): ";
			std::getline(std::cin, l.teams[teamIndex].players[playerIndex].position);

			//player nationality.
			std::cout << endl << "Enter player's new nationality: ";
			std::getline(std::cin, l.teams[teamIndex].players[playerIndex].nationality);

			//player DOB.
			std::cout << endl << "Enter player's new date of birth (DD/MM/YYYY): ";
			std::getline(std::cin, l.teams[teamIndex].players[playerIndex].DOB);

			break;
		default:
			cout << "Invalid choice.\n";
			break;
		}
	} while (choice > 5 || choice < 1);

	std::cout << "____________________________________________________________________\n";
	viewCertainPlayerdata(l, teamIndex, playerIndex);
}

//specify which player scored goals
void addGoal(League& l, int goals, int teamIndex)
{
	//set goal prob.
	if (goals == 0)
	{
		return;
	}


	int remainingGoals = goals;
	int playerIndex = 0;
	int goalstoadd = 0;

	while (remainingGoals > 0)
	{
		viewAllTeamPlayersData(l, teamIndex);

		if (goals == remainingGoals)
		{
			cout << "Team " << l.teams[teamIndex].name << " scored " << goals << "\nChoose the players who scored these(this) goal\n\n";
		}
		else
		{
			cout << remainingGoals << " goal(s) still remainning\n\n";
		}

		playerIndex = getInt("Choose player index who scored goals from above: ");
		if (playerIndex < 1 || playerIndex > l.teams[teamIndex].numPlayers)
		{
			cout << "Error, Invalide player index!!\n";
			system("pause");
			continue;
		}

		cout << "You Chose " << l.teams[teamIndex].players[playerIndex - 1].name << "\n";
		goalstoadd = getInt("How many goals did he score: ");

		if (goalstoadd > remainingGoals || goalstoadd < 1)
		{
			cout << "Invalide number of goals!!!\n";
			system("pause");
			continue;
		}
		l.teams[teamIndex].players[playerIndex - 1].goals += goalstoadd;

		remainingGoals = remainingGoals - goalstoadd;
	}

}

//set and display top 10 scoring payers
void setTopPlayers(League& l)
{
	int allPlayersNum = 0;

	for (int teamIndex = 0; teamIndex < l.numTeams; teamIndex++)
	{
		for (int playerIndex = 0; playerIndex < l.teams[teamIndex].numPlayers; playerIndex++)
		{
			allPlayersNum++;

		}
	}

	Player* allPlayerPtr = new Player[allPlayersNum];

	int index = 0;

	//filling the all arr
	for (int teamIndex = 0; teamIndex < l.numTeams; teamIndex++)
	{
		for (int playerIndex = 0; playerIndex < l.teams[teamIndex].numPlayers; playerIndex++)
		{
			allPlayerPtr[index] = l.teams[teamIndex].players[playerIndex];
			allPlayerPtr[index].joindTeam = l.teams[teamIndex].name;
			index++;
		}
	}

	//sort to know top 10 players
	for (index = 0; index < allPlayersNum; index++)
	{
		for (int index2 = index + 1; index2 < allPlayersNum; index2++)
		{
			if (allPlayerPtr[index].goals < allPlayerPtr[index2].goals)
			{
				Player temp = allPlayerPtr[index];
				allPlayerPtr[index] = allPlayerPtr[index2];
				allPlayerPtr[index2] = temp;
			}

		}
	}

	//check loop for how many players scored
	int playersToView = 0;
	for (int i = 0; i < 10; i++)
	{
		if (allPlayerPtr[i].goals == 0)
		{
			break;
		}
		playersToView++;
	}

	if (playersToView == 0)
	{
		cout << "No players scored yet!\n\n";
	}
	//view top players
	for (int i = 0; i < playersToView; i++)
	{
		std::cout << allPlayerPtr[i].position << "\t" << allPlayerPtr[i].name <<
			"\t\tGoals scored: " << allPlayerPtr[i].goals << "\tJoined team: " << allPlayerPtr[i].joindTeam << endl <<
			"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	}

	delete[] allPlayerPtr;
	allPlayerPtr = NULL;
}

//search for a player by name or id
void search(League l)
{
	char check = 'n';
	do
	{
		int choice = 0;
		do
		{
			choice = getInt("How do you want to search?\n1:By ID.\n2:By name.\nChoice: ");
		} while (choice > 2 || choice < 1);

		bool found = false;
		string nameToFind;
		int numOfFound = 0;

		int idToFind = 0;

		switch (choice)
		{

		case 1:

			idToFind = getInt("Enter ID you want to search for: ");

			for (int teamIndex = 0; teamIndex < l.numTeams; teamIndex++)
			{
				for (int playerIndex = 0; playerIndex < l.teams[teamIndex].numPlayers; playerIndex++)
				{
					if (idToFind == l.teams[teamIndex].players[playerIndex].id)
					{
						std::cout << "Found\n";
						viewCertainPlayerdata(l, teamIndex, playerIndex);
						std::cout << "Team: " << l.teams[teamIndex].name << "\n~~~~~~~~~~~~~~~~~~~\n";
						found = true;
					}
				}
			}
			break;
		case 2:

			//get name.
			std::cout << "Enter player name: ";
			do
			{
				getline(std::cin, nameToFind);
				if (nameToFind.length() < 3)
				{
					std::cout << "Too short!\Please Enter more characters.\n";
				}
			} while (nameToFind.length() < 3);

			//check name
			for (int teamIndex = 0; teamIndex < l.numTeams; teamIndex++)
			{
				for (int playerIndex = 0; playerIndex < l.teams[teamIndex].numPlayers; playerIndex++)
				{
					bool isFound = l.teams[teamIndex].players[playerIndex].name.find(nameToFind) != string::npos;
					if (isFound)
					{
						std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
						viewCertainPlayerdata(l, teamIndex, playerIndex);
						l.teams[teamIndex].players[playerIndex].joindTeam = l.teams[teamIndex].name;
						std::cout << "Team: " << l.teams[teamIndex].players[playerIndex].joindTeam << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
						found = true;
						numOfFound++;
					}
					if (numOfFound > 4)
					{
						return;
					}
				}
			}
			break;
		default:
			break;
		}
		if (!found && choice == 1)
		{
			std::cout << "This ID is not found!!\n\n";
		}
		if (!found && choice == 2)
		{
			std::cout << "This name is not found!!\n\n";
		}
		cout << "Do you want to search again?(Y/N): ";
		cin >> check;
		cin.ignore();
	} while (check == 'Y' || check == 'y');
}


//adds a week and modifies the date
void incDate(int& dy, int& mnth, int& yr)
{
	dy += 7;
	switch (mnth)
	{
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
		if (dy > 31)dy -= 31, mnth++; break;
	case 4:
	case 6:
	case 9:
	case 11:
		if (dy > 30)dy -= 30, mnth++; break;
	case 2:
	{
		if (((yr % 4 == 0 && yr % 100 != 0) || yr % 400 == 0) && dy > 29)dy -= 29, mnth++;
		else if (!((yr % 4 == 0 && yr % 100 != 0) || yr % 400 == 0) && dy > 28)dy -= 28, mnth++;
		break;
	}
	case 12:
		if (dy > 31)dy -= 31, mnth = 1, yr++; break;
	}
}
//generates the matches
bool gnrtMtches(League& l)
{
	if (l.readyTeams < 20)
	{
		std::cout << "Not enough ready teams to generate the schedule!\n";
		return 0;
	}
	int mtch[10][2];
	bool chk[20] = { 0 };
	for (int i = 0; i < 10; i++)
	{
		srand(time(0));
		do
		{
			mtch[i][0] = rand() % 20;
		} while (chk[mtch[i][0]]);
		chk[mtch[i][0]]++;
		do
		{
			mtch[i][1] = rand() % 20;
		} while (chk[mtch[i][1]]);
		chk[mtch[i][1]]++;
	}
	for (int i = 0; i < 19; i++)
	{
		for (int j = i * 10; j < (i + 1) * 10; j++)
		{
			int x = mtch[j % 10][0];
			int y = mtch[j % 10][1];
			l.Matches[j].id = l.idGen;
			l.idGen++;
			l.Matches[j].TeamHome = l.teams[x].name;
			l.Matches[j].TeamAway = l.teams[y].name;
			if (j % 10 != 0)
			{
				mtch[j % 10][0] += (20 - 1);
				mtch[j % 10][0] %= 20;
				if (mtch[j % 10][0] == mtch[0][0])
				{
					mtch[j % 10][0] += (20 - 1);
				}
			}
			mtch[j % 10][1] += (20 - 1);
			mtch[j % 10][1] %= 20;
			if (mtch[j % 10][1] == mtch[0][0])mtch[j % 10][1] += (20 - 1);
			mtch[j % 10][0] %= 20;
			mtch[j % 10][1] %= 20;
		}
	}
	for (int i = 0; i < 19; i++)
	{
		for (int j = i * 10; j < (i + 1) * 10; j++)
		{
			int rnd = rand() % 10;
			swap(l.Matches[j].id, l.Matches[(i * 10) + rnd].id);
			swap(l.Matches[j].TeamHome, l.Matches[(i * 10) + rnd].TeamHome);
			swap(l.Matches[j].TeamAway, l.Matches[(i * 10) + rnd].TeamAway);
		}
	}
	gnrtSchdl(l);
	return 1;
}
//generate the dates of the schedule
void gnrtSchdl(League& l)
{
	bool ok = true;
	int d, m, y;
	do
	{
		if (!ok)std::cout << "Sorry, unavailable date. Please try again.\n";
		std::cout << "Please enter season start date (day month year)\n";
		d = getInt("Day : ");
		m = getInt("Month : ");
		y = getInt("Year : ");
		switch (m)
		{
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
		{
			if (d > 31 || d < 1)ok = false;
			else ok = true;
			break;
		}
		case 4:
		case 6:
		case 9:
		case 11:
		{
			if (d > 30 || d < 1)ok = false;
			else ok = true;
			break;
		}
		case 2:
		{
			if (((y % 4 == 0 && y % 100 != 0) || y % 400 == 0) && (d > 29 || d < 1))ok = false;
			else if (!((y % 4 == 0 && y % 100 != 0) || y % 400 == 0) && (d > 28 || d < 1))ok = false;
			else ok = true;
			break;
		}
		default:
			ok = false;
		}
	} while (!ok);

	for (int i = 0; i < 19; i++)
	{
		for (int j = i * 10, cnt = 0; j < (i + 1) * 10; j++, cnt++)
		{
			l.Matches[j].StartDate.day = d;
			l.Matches[j].StartDate.month = m;
			l.Matches[j].StartDate.year = y;
			if (cnt < 3)l.Matches[j].StartTime = 4;
			else if (cnt < 6)l.Matches[j].StartTime = 6;
			else l.Matches[j].StartTime = 8;
		}
		incDate(d, m, y);
	}

	//filling table
	for (int i = 0; i < 20; i++)
	{
		l.table[i].team = l.teams[i];
	}

	l.gnrtd = 1;
	viewSchdl(l);
	std::cout << "Schedule generated successfuly.\n";
}
//view matches and their date
void viewSchdl(League& l)
{
	if (!l.gnrtd)
	{
		std::cout << "League hasn't started yet\n";
		return;
	}
	for (int i = 0; i < 190; i++)
	{
		if (i % 10 == 0)
		{
			std::cout << "\nWeek " << (i / 10) + 1 << endl;
			std::cout << "==================================\n";
		}
		std::cout << i + 1 << " : " << l.Matches[i].id << "\t"
			<< l.Matches[i].TeamHome << " vs "
			<< l.Matches[i].TeamAway;
		if (l.Matches[i].TeamHome.size() + l.Matches[i].TeamAway.size() + 4 >= 32)
			std::cout << "\t";
		else if (l.Matches[i].TeamHome.size() + l.Matches[i].TeamAway.size() + 4 >= 24)
			std::cout << "\t\t";
		else if (l.Matches[i].TeamHome.size() + l.Matches[i].TeamAway.size() + 4 >= 16)
			std::cout << "\t\t\t";
		else std::cout << "\t\t\t\t";
		std::cout << l.Matches[i].StartDate.day << "/"
			<< l.Matches[i].StartDate.month << "/"
			<< l.Matches[i].StartDate.year << "\t"
			<< l.Matches[i].StartTime << " PM\n";
	}
	std::cout << "====================================================================\n";
}

//enters the result of a match
void enterResult(League& l)
{
	if (!l.gnrtd)
	{
		std::cout << "League hasn't started yet\n";
		return;
	}
	int matchID;
	matchID = l.lastResult;
	viewSchdl(l);
	char ans;
	do
	{
		int homeGoals, awayGoals;

		std::cout << "Enter the result of match " << matchID << endl;
		homeGoals = getInt("Enter goals of " + l.Matches[matchID - 1].TeamHome + " : ");

		awayGoals = getInt("Enter goals of " + l.Matches[matchID - 1].TeamAway + " : ");

		l.Matches[matchID - 1].Result.id = matchID;
		l.Matches[matchID - 1].Result.TeamHomeGoals = homeGoals;
		l.Matches[matchID - 1].Result.TeamAwayGoals = awayGoals;
		l.Matches[matchID - 1].Result.played = true;

		int teamHomeIndex = 0;
		int teamAwayIndex = 0;

		for (int teamIndex = 0; teamIndex < l.numTeams; teamIndex++)
		{
			if (l.Matches[matchID - 1].TeamHome == l.teams[teamIndex].name)
			{
				teamHomeIndex = teamIndex;
			}
			else if (l.Matches[matchID - 1].TeamAway == l.teams[teamIndex].name)
			{
				teamAwayIndex = teamIndex;

			}
		}

		addGoal(l, homeGoals, teamHomeIndex);
		addGoal(l, awayGoals, teamAwayIndex);

		if (homeGoals > awayGoals)
		{
			for (int j = 0; j < 20; j++)
			{

				if (l.Matches[matchID - 1].TeamHome == l.table[j].team.name) {
					l.table[j].points += 3;
					l.table[j].goals_for += homeGoals;
					l.table[j].goals_against += awayGoals;
					l.table[j].goals_diff = l.table[j].goals_for - l.table[j].goals_against;
					l.table[j].wins += 1;
				}
				else if (l.Matches[matchID - 1].TeamAway == l.table[j].team.name) {
					l.table[j].goals_for += awayGoals;
					l.table[j].goals_against += homeGoals;
					l.table[j].goals_diff = l.table[j].goals_for - l.table[j].goals_against;
					l.table[j].losses += 1;
				}
			}
		}
		else if (awayGoals > homeGoals)
		{
			for (int j = 0; j < 20; j++)
			{
				if (l.Matches[matchID - 1].TeamAway == l.table[j].team.name) {
					l.table[j].points += 3;
					l.table[j].goals_for += awayGoals;
					l.table[j].goals_against += homeGoals;
					l.table[j].goals_diff = l.table[j].goals_for - l.table[j].goals_against;
					l.table[j].wins += 1;
				}
				else if (l.Matches[matchID - 1].TeamHome == l.table[j].team.name) {
					l.table[j].goals_for += homeGoals;
					l.table[j].goals_against += awayGoals;
					l.table[j].goals_diff = l.table[j].goals_for - l.table[j].goals_against;
					l.table[j].losses += 1;
				}
			}
		}
		else
		{
			for (int j = 0; j < 20; j++)
			{
				if (l.Matches[matchID - 1].TeamAway == l.table[j].team.name) {
					l.table[j].points += 1;
					l.table[j].goals_for += awayGoals;
					l.table[j].goals_against += homeGoals;
					l.table[j].goals_diff = l.table[j].goals_for - l.table[j].goals_against;
					l.table[j].draws += 1;
				}
				else if (l.Matches[matchID - 1].TeamHome == l.table[j].team.name) {
					l.table[j].points += 1;
					l.table[j].goals_for += homeGoals;
					l.table[j].goals_against += awayGoals;
					l.table[j].goals_diff = l.table[j].goals_for - l.table[j].goals_against;
					l.table[j].draws += 1;
				}
			}
		}
		matchID++;
		std::cout << "Do you want to enter more results (Y/N) : ";
		cin >> ans;
		cin.ignore();
	} while (ans == 'y' || ans == 'Y');
	l.lastResult = matchID;
}
//view the result of a certain match
void viewResult(League& l)
{
	if (!l.gnrtd)
	{
		std::cout << "League hasn't started yet\n";
		return;
	}
	char ans;
	do
	{
		int teamID;
		viewSchdl(l);

		teamID = getInt("Enter the number of the match to view result : ");
		if (teamID > 190 || teamID < 1)
		{
			std::cout << "Invalid ID" << endl;
			std::cout << "Do you want to show more results : ";
			cin >> ans;
			cin.ignore();
			continue;
		}
		if (!l.Matches[teamID - 1].Result.played)
		{
			std::cout << "This match hasn't been played yet." << endl;
			std::cout << "Do you want to show more results : ";
			cin >> ans;
			cin.ignore();
			continue;
		}
		std::cout << "Result of match " << l.Matches[teamID - 1].Result.id << " : "
			<< l.Matches[teamID - 1].Result.TeamHomeGoals << " - "
			<< l.Matches[teamID - 1].Result.TeamAwayGoals;

		if (l.Matches[teamID - 1].Result.TeamHomeGoals > l.Matches[teamID - 1].Result.TeamAwayGoals)
			std::cout << " and the winner is " << l.Matches[teamID - 1].TeamHome << endl;
		else if (l.Matches[teamID - 1].Result.TeamHomeGoals < l.Matches[teamID - 1].Result.TeamAwayGoals)
			std::cout << " and the winner is " << l.Matches[teamID - 1].TeamAway << endl;
		else std::cout << " and the result is a draw" << endl;

		std::cout << "Do you want to show more results (Y/N) : ";
		cin >> ans;
		cin.ignore();
	} while (ans == 'y' || ans == 'Y');
}

//update the order of a table
void update_table(League& league)
{
	for (int i = 0; i < 20; i++) // i = 0
	{
		for (int j = i + 1; j < 20; j++)
		{
			if (league.table[i].points < league.table[j].points)
			{
				swap(league.table[i], league.table[j]);
			}
		}
	}
	for (int i = 0; i < 20; i++)
	{
		for (int j = i + 1; j < 20; j++)
		{
			if (league.table[i].points == league.table[j].points)
			{
				if (league.table[i].goals_diff < league.table[j].goals_diff)
				{
					swap(league.table[i], league.table[j]);
				}
			}
		}
	}
	for (int i = 0; i < 20; i++)
	{
		for (int j = i + 1; j < 20; j++)
		{
			if (league.table[i].points == league.table[j].points)
			{
				if (league.table[i].goals_diff == league.table[j].goals_diff)
				{
					if (league.table[i].goals_for < league.table[j].goals_for)
						swap(league.table[i], league.table[j]);
				}
			}
		}
	}
	for (int i = 0; i < 20; i++)
	{
		for (int j = i + 1; j < 20; j++)
		{
			if (league.table[i].points == league.table[j].points)
			{
				if (league.table[i].goals_diff == league.table[j].goals_diff)
				{
					if (league.table[i].goals_for == league.table[j].goals_for)
					{
						if (league.table[i].goals_against > league.table[j].goals_against)
							swap(league.table[i], league.table[j]);
					}
				}
			}
		}
	}
}
//view the table
void view_table(League& league)
{
	if (!league.gnrtd)std::cout << "Warning!! League hasn't started yet\n";
	update_table(league);
	std::cout << "    Club"
		<< "\t\t  W"
		<< "\t      L"
		<< "\t\t  D"
		<< "\t      GF"
		<< "\t  GA"
		<< "\t      GD"
		<< "\t Pts\n"
		<< "-------------------------------------------------------------------------------------------------------- \n";
	for (int i = 0; i < 20; i++) // i = 0 
	{
		for (int j = 0; j < 20; j++) { // j = 0

			if (j + 1 == i + 1)
			{
				std::cout << setw(2) << right
					<< j + 1
					<< ". "
					<< setw(22) << left
					<< league.table[j].team.name
					<< setw(12) << left
					<< league.table[j].wins
					<< setw(12) << left
					<< league.table[j].losses
					<< setw(12) << left
					<< league.table[j].draws
					<< setw(12) << left
					<< league.table[j].goals_for
					<< setw(12) << left
					<< league.table[j].goals_against
					<< setw(12) << left
					<< league.table[j].goals_diff
					<< setw(12) << left
					<< league.table[j].points
					<< endl;
			}
		}
	}
}