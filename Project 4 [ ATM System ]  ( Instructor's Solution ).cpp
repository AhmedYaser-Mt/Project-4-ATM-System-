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
void ShowNormalWithdrawScreen();

enum enATM_MainMenueOptions
{
	QuickWithdraw = 1, NormalWithdraw = 2, Deposite = 3, CheckBalance = 4, Logout = 5
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
	short Choice = 0;
	cout << "Choose what do you want to do? [1 to 5]? ";
	cin >> Choice;
	return Choice;
}

short ReadQuickWithdrawOption()
{
	short Choice = 0;
	while (Choice < 1 || Choice > 9)
	{
		cout << "Choose what to do from [1] to [9] ? ";
		cin >> Choice;
	}
	return Choice;
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

bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector <stClient>& vClients)
{
	char Answer = 'n';

	cout << "\n\nAre you sure you want perform this Transaction ?  Y/N? ";
	cin >> Answer;

	if (Answer == 'Y' || Answer == 'y')
	{
		for (stClient& C : vClients)
		{
			if (C.AccountNumber == AccountNumber)
			{
				C.AccountBalance += Amount;
				SaveClientsDataToFile(ClientsFileName, vClients);
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

short GetQuickWithdrawAmount(short QuickWithdrawOption)
{
	switch (QuickWithdrawOption)
	{
	case 1:
		return 20;
	case 2:
		return 50;
	case 3:
		return 100;
	case 4:
		return 200;
	case 5:
		return 400;
	case 6:
		return 600;
	case 7:
		return 800;
	case 8:
		return 1000;
	}
}

void PerformQuickWithdrawOption(short QuickWithdrawOption)
{
	if (QuickWithdrawOption == 9)
		return;

	short WithdrawBalance = GetQuickWithdrawAmount(QuickWithdrawOption);

	if (WithdrawBalance > CurrentClient.AccountBalance)
	{
		cout << "\n\nThe Amount Exceeds your balance, make another choice.\n";
		cout << "Press any key to continue...";
		system("pause>0");
		ShowQuickWithdrawScreen();
		return;
	}

	vector <stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
	DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, WithdrawBalance * -1, vClients);
	CurrentClient.AccountBalance -= WithdrawBalance;
}

void ShowQuickWithdrawScreen()
{
	system("cls");

	cout << "=============================================\n";
	cout << "\t\tQuick Withdraw\n";
	cout << "=============================================\n";
	cout << "\t[1] 20 \t\t[2] 50\n";
	cout << "\t[3] 100\t\t[4] 200\n";
	cout << "\t[5] 400\t\t[6] 600\n";
	cout << "\t[7] 800\t\t[8] 1000\n";
	cout << "\t[9] Exit\n";
	cout << "=============================================\n";
	cout << "Your Balance is " << CurrentClient.AccountBalance << endl;

	PerformQuickWithdrawOption(ReadQuickWithdrawOption());
}

void PerformNormalWithdrawOption()
{
	int WithdrawBalance = ReadWithdrawAmount();

	if (WithdrawBalance > CurrentClient.AccountBalance)
	{
		cout << "\n\nThe Amount Exceeds your balance, make another choice.\n";
		cout << "Press any key to continue...";
		system("pause>0");
		ShowNormalWithdrawScreen();
		return;
	}

	vector <stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
	DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, WithdrawBalance * -1, vClients);
	CurrentClient.AccountBalance -= WithdrawBalance;
}

void ShowNormalWithdrawScreen()
{
	system("cls");

	cout << "=====================================\n";
	cout << "\tNormal Withdraw Screen";
	cout << "\n=====================================\n";

	PerformNormalWithdrawOption();
}

double ReadDepositAmount()
{
	double Amount = 0;
	do
	{
		cout << "\nEnter a positive Deposit Amount ? ";
		cin >> Amount;
	} while (Amount < 0);

	return Amount;
}

void PerformDepositOption()
{
	double DepositAmount = ReadDepositAmount();

	vector <stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
	DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, DepositAmount, vClients);
	CurrentClient.AccountBalance += DepositAmount;
}

void ShowDepositScreen()
{
	system("cls");

	cout << "=====================================\n";
	cout << "\tDeposit Screen\n";
	cout << "=====================================\n";

	PerformDepositOption();
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
