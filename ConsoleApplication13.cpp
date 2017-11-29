#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

const int READERROR = 100;														
const int WRITEERROR = 200;
const int ARRAYSIZE = 100;

class FavoriteGame																		//a class that used to represent your favorite thing
{
	string Name;
	string Developer;
	string Genre;
	string Release;
	int Rating;

public:
	FavoriteGame();
	FavoriteGame(string n, string d, string g, string l, int r);
	void CaptureGame();
	void ShowGame();
	int SaveGame(ofstream &outfile);
};

class GameList																		
{
	FavoriteGame **list;
	int numrecords;
	int listsize;
	int ArraySize();

public:
	GameList();
	~GameList();
	void GetInput();
	void ShowList();
	int SaveGameList(string filename);
	int ReadGameList(string filename);

};

FavoriteGame::FavoriteGame()														//constructor
{
	Name = ""; Developer = ""; Genre = ""; Release = ""; Rating = 0;				//at least 4 private member variables that represent your favorite thing
}

FavoriteGame::FavoriteGame(string n, string d, string g, string l, int r)			//overload constructor
{
	Name = n; Developer = d; Genre = g; Release = l; Rating = r;
}

void FavoriteGame::CaptureGame()													//capture a new favorite item from the user
{
	cout << "Name of the Game: ";
	getline(cin, Name);
	cout << "Developer of the game: ";
	getline(cin, Developer);
	cout << "Genre of the game: ";
	getline(cin, Genre);
	cout << "Initial release date: ";
	getline(cin, Release);
	cout << "Rating? (0-100)";
	cin >> Rating;

	cin.ignore();
	cin.clear();
}

void FavoriteGame::ShowGame()														//show your favorite item to the console.
{
	cout << "" << endl;
	cout << "Name of the Game: " << Name << endl;
	cout << "Developer of the game: " << Developer << endl;
	cout << "Genre of the game: " << Genre << endl;
	cout << "Initial release date:" << Release << endl;
	cout << "Rating: " << Rating << endl;
}

int FavoriteGame::SaveGame(ofstream &outfile)
{
	if (outfile.is_open())
	{
		outfile << Name << "|" << Developer << "|" << Genre << "|" << Release << "|" << Rating << endl;
		return 0;
	}
	else
		return WRITEERROR;
}

GameList::GameList()											//Dynamically allocate a double pointer list of favorite objects.
{
	list = new FavoriteGame*[ARRAYSIZE];
	numrecords = 0;
	listsize = ARRAYSIZE;
}

GameList::~GameList()
{
	for (int i = 0; i < numrecords; i++)
	{
		delete list[i];
	}
	delete[]list;
}

int GameList::ArraySize()
{
	FavoriteGame **temp;
	temp = new FavoriteGame*[listsize + ARRAYSIZE];
	listsize = listsize + ARRAYSIZE;
	for (int i = 0; i < numrecords; i++)
	{
		temp[i] = list[i];
	}
	delete[]list;
	list = temp;
	return 0;
}

void GameList::GetInput()
{
	string answer = "Y";
	cout << "Enter Game Y/N?" << endl;
	getline(cin, answer);

	while (answer == "Y" || answer == "y")										//
	{
		list[numrecords] = new FavoriteGame();
		list[numrecords]->CaptureGame();
		numrecords++;
		cout << "Enter another list of game? (Y/N?)" << endl;
		getline(cin, answer);
	}
}

void GameList::ShowList()
{
	for (int i = 0; i < numrecords; i++)
	{
		list[i]->ShowGame();
	}
}

int GameList::SaveGameList(string filename)
{
	ofstream output(filename, ios::trunc);
	bool duplicate = false;
	if (output)
	{
		for (int i = 0; i < numrecords; i++)
		{
			list[i]->SaveGame(output);
		}
	}
	else
	{
		return WRITEERROR;
	}
	return 0;
}



int GameList::ReadGameList(string filename)
{
	string aname, adeveloper, agenre, arelease, arating;
	ifstream infile(filename, ios::in);

	if (!infile)
	{
		cout << "No File Found" << endl;
		return READERROR;
	}

	while (!infile.eof())
	{
		if (numrecords == listsize)																//
		{
			ArraySize();
		}
		getline(infile, aname, '|');
		if (!aname.empty())
		{
			getline(infile, adeveloper, '|');
			getline(infile, agenre, '|');
			getline(infile, arelease, '|');
			getline(infile, arating);
			int rating = stoi(arating);
			list[numrecords] = new FavoriteGame(aname, adeveloper, agenre, arelease, rating);
			numrecords++;
		}
	}

	infile.close();
	return 0;
}

int main()
{
	string filename;
	cout << "Enter full-path filename: ";
	getline(cin, filename);
	GameList myList;
	string answer;
	int error;
	error = myList.ReadGameList(filename);

	if (error)
	{
		cout << "Cannot read list - Create new list of the game? Y/N -->";
		getline(cin, answer);
		if (answer != "Y" && answer != "y")
		{
			return -1;
		}
	}
	myList.GetInput();
	myList.SaveGameList(filename);
	myList.ShowList();

	return 0;
}