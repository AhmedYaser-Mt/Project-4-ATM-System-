#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<iomanip>
using namespace std;

const string ClientsFileName = "Clients.txt";
void Login();
void ShowATM_MainMenue();
void ShowQuickWithdrawScreen();

enum enATM_MainMenueOptions
{
	QuickWithdraw = 1, NormalWithdraw = 2, Deposite = 3, CheckBalance = 4, Logout = 5
};

enum enQuickWithdrawMenueOptions
{
	en20 = 1, en50 = 2, en100 = 3, en200 = 4, en400 = 5, en600 = 6, en800 = 7,
	en1000 = 8, Exit = 9
};

struct stClient
{
	string AccountNumber = "";
	string PinCode = "";
	string Name = "";
	string Phone = "";
	double AccountBalance = 0.0;
	bool MarkForDelete = false;
};

stClient CurrentClient;

short ReadATM_MainMenueOption()
{
	short Number = 0;
	cout << "Choose what do you want to do? [1 to 5]? ";
	cin >> Number;
	return Number;
}

short ReadQuickWithdrawMenueOption()
{
	short Number = 0;
	cout << "Choose what to withdraw from [1] to [9] ? ";
	cin >> Number;
	return Number;
}

vector <string> SplitString(string S1, string delim)
{
	short pos = 0;
	string sWord = "";
	vector <string> vString;

	while ((pos = S1.find(delim)) != std::string::npos)
	{
		sWord = S1.substr(0, pos);
		if (sWord != "")
		{
			vString.push_back(sWord);
		}

		S1.erase(0, pos + delim.length());
	}

	if (S1 != "")
	{
		vString.push_back(S1);
	}

	return vString;
}

stClient ConvertLineToClientRecord(string LineRecord, string Seperator = "#//#")
{
	stClient Client;

	vector <string> vClientData = SplitString(LineRecord, Seperator);

	Client.AccountNumber = vClientData[0];
	Client.PinCode = vClientData[1];
	Client.Name = vClientData[2];
	Client.Phone = vClientData[3];
	Client.AccountBalance = stod(vClientData[4]);

	return Client;
}

vector <stClient> LoadClientsDataFromFile(string FileName)
{
	vector <stClient> vClients;

	fstream MyFile;
	MyFile.open(FileName, ios::in);

	if (MyFile.is_open())
	{
		string Line;
		stClient Client;

		while (getline(MyFile, Line))
		{
			Client = ConvertLineToClientRecord(Line);

			vClients.push_back(Client);
		}

		MyFile.close();
	}

	return vClients;
}

void ShowEndScreen()
{
	cout << "\n--------------------------------------\n";
	cout << "\tProgram Ends :-)\n";
	cout << "--------------------------------------\n\n";
}

string ConvertRecordToLine(stClient& BankClientData, string Seperator = "#//#")
{
	string sClientRecord = "";

	sClientRecord += BankClientData.AccountNumber + Seperator;
	sClientRecord += BankClientData.PinCode + Seperator;
	sClientRecord += BankClientData.Name + Seperator;
	sClientRecord += BankClientData.Phone + Seperator;
	sClientRecord += to_string(BankClientData.AccountBalance);

	return sClientRecord;
}

void SaveClientsDataToFile(string FileName, vector <stClient>& vClients)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out);

	if (MyFile.is_open())
	{
		string DataLine = "";

		for (stClient& C : vClients)
		{
			if (C.MarkForDelete == false)
			{
				DataLine = ConvertRecordToLine(C);

				MyFile << DataLine << endl;
			}
		}

		MyFile.close();
	}
}

bool DepositBalanceToCurrentClient(double Amount)
{
	char Answer = 'n';

	cout << "\n\nAre you sure you want perform this Transaction ?  Y/N? ";
	cin >> Answer;

	if (Answer == 'Y' || Answer == 'y')
	{
		vector <stClient> vClients = LoadClientsDataFromFile(ClientsFileName);

		for (stClient& C : vClients)
		{
			if (C.AccountNumber == CurrentClient.AccountNumber)
			{
				C.AccountBalance += Amount;
				SaveClientsDataToFile(ClientsFileName, vClients);
				CurrentClient.AccountBalance = C.AccountBalance;
				cout << "\n\nDone Successfully. New Balance is : " << C.AccountBalance << endl;;
				return true;
			}
		}

		return false;
	}
}

void GoBackToATM_MainMenue()
{
	cout << "\n\n\nPress any key to go back to ATM Main Menue... ";
	system("pause>0");
	ShowATM_MainMenue();
}

void ShowAmountExceedMessage()
{
	cout << "\n\nAmount Exceeds your balance, make another choice.\n";
	cout << "Press any key to continue...";
	system("pause>0");
}

int ReadWithdrawAmount()
{
	int Amount = 0;

	do
	{
		cout << "\nPlease enter an amount multiple of 5\'s ? ";
		cin >> Amount;
	} while (Amount % 5 != 0);

	return Amount;
}

void QuickWithdrawBalanceToCurrentClient(double Amount)
{
	bool ExceedBalance = false;

	do
	{
		if (Amount > CurrentClient.AccountBalance)
		{
			ExceedBalance = true;
			ShowAmountExceedMessage();
			ShowQuickWithdrawScreen();
		}
		else
			ExceedBalance = false;

	} while (ExceedBalance);

	DepositBalanceToCurrentClient(Amount * -1);
}

void PerformQuickWithdrawMenueOption(enQuickWithdrawMenueOptions QuickWithdrawMenueOption)
{
	switch (QuickWithdrawMenueOption)
	{
	case enQuickWithdrawMenueOptions::en20:
		QuickWithdrawBalanceToCurrentClient(20);
		GoBackToATM_MainMenue();
		break;

	case enQuickWithdrawMenueOptions::en50:
		QuickWithdrawBalanceToCurrentClient(50);
		GoBackToATM_MainMenue();
		break;

	case enQuickWithdrawMenueOptions::en100:
		QuickWithdrawBalanceToCurrentClient(100);
		GoBackToATM_MainMenue();
		break;

	case enQuickWithdrawMenueOptions::en200:
		QuickWithdrawBalanceToCurrentClient(200);
		GoBackToATM_MainMenue();
		break;

	case enQuickWithdrawMenueOptions::en400:
		QuickWithdrawBalanceToCurrentClient(400);
		GoBackToATM_MainMenue();
		break;

	case enQuickWithdrawMenueOptions::en600:
		QuickWithdrawBalanceToCurrentClient(600);
		GoBackToATM_MainMenue();
		break;

	case enQuickWithdrawMenueOptions::en800:
		QuickWithdrawBalanceToCurrentClient(800);
		GoBackToATM_MainMenue();
		break;

	case enQuickWithdrawMenueOptions::en1000:
		QuickWithdrawBalanceToCurrentClient(1000);
		GoBackToATM_MainMenue();
		break;

	case enQuickWithdrawMenueOptions::Exit:
		ShowATM_MainMenue();
		break;
	}
}

void ShowQuickWithdrawScreen()
{
	system("cls");

	cout << "=============================================\n";
	cout << "\tQuick Withdraw\n";
	cout << "=============================================\n";
	cout << "\t[1] 20 \t\t[2] 50\n";
	cout << "\t[3] 100\t\t[4] 200\n";
	cout << "\t[5] 400\t\t[6] 600\n";
	cout << "\t[7] 800\t\t[8] 1000\n";
	cout << "\t[9] Exit\n";
	cout << "=============================================\n";
	cout << "Your Balance is " << CurrentClient.AccountBalance << endl;

	PerformQuickWithdrawMenueOption(enQuickWithdrawMenueOptions(ReadQuickWithdrawMenueOption()));
}

void ShowNormalWithdrawScreen()
{
	int Amount = 0;
	bool ExceedBalance = false;

	do
	{
		system("cls");
		cout << "=====================================\n";
		cout << "\tNormal Withdraw Screen";
		cout << "\n=====================================\n";

		Amount = ReadWithdrawAmount();

		if (Amount > CurrentClient.AccountBalance)
		{
			ExceedBalance = true;
			ShowAmountExceedMessage();
		}
		else
			ExceedBalance = false;

	} while (ExceedBalance);

	DepositBalanceToCurrentClient(double(Amount * -1));
}

void ShowDepositScreen()
{
	cout << "=====================================\n";
	cout << "\tDeposit Screen\n";
	cout << "\n=====================================\n";

	double Amount = 0;

	do
	{
		cout << "\nPlease enter a positive Deposit Amount ? ";
		cin >> Amount;
	} while (Amount <= 0);

	DepositBalanceToCurrentClient(Amount);
}

void ShowCheckBalanceScreen()
{
	cout << "=====================================\n";
	cout << "\tCheck Balance Screen";
	cout << "\n=====================================\n";

	cout << "Your Balance is " << CurrentClient.AccountBalance << endl << endl;
}

void PerformATM_MainMenueOption(enATM_MainMenueOptions ATM_MainMenueOption)
{
	switch (ATM_MainMenueOption)
	{
	case enATM_MainMenueOptions::QuickWithdraw:
		system("cls");
		ShowQuickWithdrawScreen();
		GoBackToATM_MainMenue();
		break;

	case enATM_MainMenueOptions::NormalWithdraw:
		system("cls");
		ShowNormalWithdrawScreen();
		GoBackToATM_MainMenue();
		break;

	case enATM_MainMenueOptions::Deposite:
		system("cls");
		ShowDepositScreen();
		GoBackToATM_MainMenue();
		break;

	case enATM_MainMenueOptions::CheckBalance:
		system("cls");
		ShowCheckBalanceScreen();
		GoBackToATM_MainMenue();
		break;

	case enATM_MainMenueOptions::Logout:
		system("cls");
		Login();
		break;
	}
}

void ShowATM_MainMenue()
{
	system("cls");

	cout << "=============================================\n";
	cout << "\tATM Main Menue Screen\n";
	cout << "=============================================\n";
	cout << "\t[1] Quick Withdraw.\n";
	cout << "\t[2] Normal Withdraw.\n";
	cout << "\t[3] Deposite.\n";
	cout << "\t[4] Check Balance.\n";
	cout << "\t[5] Logout.\n";
	cout << "=============================================\n";

	PerformATM_MainMenueOption(enATM_MainMenueOptions(ReadATM_MainMenueOption()));
}

bool FindClientByAccountNumberAndPinCode(string AccountNumber, string PinCode, stClient& CurrentClient)
{
	vector <stClient> vClients = LoadClientsDataFromFile(ClientsFileName);

	for (stClient C : vClients)
	{
		if (C.AccountNumber == AccountNumber && C.PinCode == PinCode)
		{
			CurrentClient = C;
			return true;
		}
	}

	return false;
}

bool LoadClientInfo(string AccountNumber, string PinCode)
{
	if (FindClientByAccountNumberAndPinCode(AccountNumber, PinCode, CurrentClient))
		return true;
	else
		return false;
}

void Login() 
{
	bool LoginFailed = false;
	string AccountNumber, PinCode;

	do
	{
		system("cls");

		cout << "=============================================\n";
		cout << "\t\tLogin Screen\n";
		cout << "=============================================\n";

		if (LoginFailed)
		{
			cout << "Invalid AccountNumber/PinCode!\n";
		}

		cout << "Enter AccountNumber ? ";
		cin >> AccountNumber;

		cout << "Enter PinCode ? ";
		cin >> PinCode;

		LoginFailed = !LoadClientInfo(AccountNumber, PinCode);

	} while (LoginFailed);

	ShowATM_MainMenue();
}

int main()
{
	Login();

	system("pause>0");
	return 0;
}