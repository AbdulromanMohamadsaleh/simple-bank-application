#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>

using namespace std;

const string ClientsFile = "Clients.txt";

void ShowMainMenue();
void ShowTransactionsMenueScreen();

struct sClient {
	string AccountNumber;
	string PinCode;
	string Name;
	string Phone;
	double AccountBalance;
	bool DeleteTag = false;
};

enum enMainMenueChoice {
	ListClient = 1, AddClient = 2,	
	DeleteClient = 3,UpdateClient=4,
	FindClient = 5,eTransaction=6,
	Exit=7
};

enum enTrasationsMenue {
	eDeposit=1,eWithdrow=2,
	eTotalBalance=3,eMainMenue=4
};

enum enTransactionType{eDepositType=1, eWithdrowType=2
};

vector<string> SplitString(string Line, string Delim) {
	vector<string> vDatas;
	string Word;
	short p = 0;

	while ((p = Line.find(Delim)) != std::string::npos) {
		Word = Line.substr(0, p);
		if (Word != "") {
			vDatas.push_back(Word);
		}
		Line.erase(0, p + Delim.length());
	}

	if (Line != "") {
		vDatas.push_back(Line);
	}
	return vDatas;
}

sClient ConvertLineToRecord(string Line, string Delim = "#//#") {
	vector<string> vDatas = SplitString(Line,Delim);
	sClient Client;

	Client.AccountNumber = vDatas[0];
	Client.PinCode = vDatas[1];
	Client.Name = vDatas[2];
	Client.Phone = vDatas[3];
	Client.AccountBalance = stod(vDatas[4]);

	return Client;
}

string ConvertRecordToLine(sClient Client,string Delim="#//#") {
	string Line = "";
	Line += Client.AccountNumber + Delim;
	Line += Client.PinCode + Delim;
	Line += Client.Name + Delim;
	Line += Client.Phone + Delim;
	Line += to_string(Client.AccountBalance);

	return Line;
	
}

vector<sClient> LoadClientsDataFromFile(string FileName) {
	vector<sClient> vClients;
	fstream MyFile;
	MyFile.open(FileName, ios::in);
	if (MyFile.is_open()) {
		string Line;
		sClient Client;
		while (getline(MyFile, Line)) {
			Client = ConvertLineToRecord(Line);
			vClients.push_back(Client);
		}
		MyFile.close();
	}
	return vClients;
}

bool FindClientByAccountNumber(string AccountNumber, vector<sClient> vClients, sClient& Client) {
	for (sClient c : vClients) {
		if (c.AccountNumber == AccountNumber) {
			Client = c;
			return 1;
		}
	}
	return 0;
}

void PrintClientCard(sClient Client) { 
	cout << "\nThe following are the client details:\n";
	cout << "\n=================================================\n";
	cout << "\nAccout Number: " << Client.AccountNumber;
	cout << "\nPin Code     : " << Client.PinCode;
	cout << "\nName         : " << Client.Name;
	cout << "\nPhone        : " << Client.Phone;
	cout << "\nAccount Balance: " << Client.AccountBalance;
	cout << "\n=================================================\n";

}

string ReadAccountNumber() {
	cout << "Enter Account Number?";
	string AccountNumber;
	getline(cin>>ws, AccountNumber);
	return AccountNumber;
}

void SaveClientsToFile(string FileName, vector<sClient> vClients) {
	fstream MyFile;
	MyFile.open(FileName, ios::out);
	if (MyFile.is_open()) {
		string Line;
		for (sClient c : vClients) {
			if (c.DeleteTag != true) {
				Line = ConvertRecordToLine(c);
				MyFile << Line << endl;
			}
		}

		MyFile.is_open();
	}
}

bool MarkClientToDeleteByAccountNumber(string AccountNumber, vector<sClient> &vClients) {
	for (sClient& c : vClients) {
		if (c.AccountNumber == AccountNumber) {
			c.DeleteTag = true;
			return 1;
		}
	}
	return 0;

}

bool DeleteClientByAccountNumber(string AccountNumber, vector<sClient>& vClient) {
	sClient Client;
	char a = 'a';

	if (FindClientByAccountNumber(AccountNumber, vClient,Client)) {
		PrintClientCard(Client);

		cout << "\n\nAre you sure want delete this client? y/n?";
		cin >> a;

		if (tolower(a) == 'y') {
			MarkClientToDeleteByAccountNumber(AccountNumber, vClient);
			SaveClientsToFile(ClientsFile, vClient);
			
			cout << "\n\nClient Deletes Succssessfukky.";
			
			vClient = LoadClientsDataFromFile(ClientsFile);
			return 1;

		}
	}
	else
	{
		cout << "\n\nClient not found!";
	}
	return 0;

}

sClient ChangeClientRecord(string AccountNumber) {
	sClient Client;
	Client.AccountNumber = AccountNumber;

	cout << "Enter PinCode?";
	getline(cin>>ws, Client.PinCode);
	cout << "Enter Name?";
	getline(cin, Client.Name);
	cout << "Enter Phone?";
	getline(cin, Client.Phone);
	cout << "Enter AccountBalance?";
	cin >> Client.AccountBalance;

	return Client;
}

bool UpdateClientByAccountNumber(string AccountNumber, vector<sClient>& vClients) {
	sClient Client;
	char a = 'y';
	if (FindClientByAccountNumber(AccountNumber, vClients, Client)) {
		PrintClientCard(Client);

		cout << "\n\nAre you sure want Update this Client? y/n?";
		cin >> a;

		if (tolower(a) == 'y') {
			for (sClient& c : vClients) {
				if (c.AccountNumber == AccountNumber) {
					c = ChangeClientRecord(AccountNumber);
					break;
				}
			}
			SaveClientsToFile(ClientsFile, vClients);
			cout << "\n\nClient Uodated Succssefully";
			return 1;
		}
	}
	else {
		cout << "\n\nClient [" << AccountNumber << "] Nit Found\n";
	}
	return 0;
}

bool IsExistedClient(string AccountNumber) {
	fstream MyFile;
	MyFile.open(ClientsFile, ios::in);
	if (MyFile.is_open()) {
		string Line;
		sClient c;
		while (getline(MyFile,Line))
		{
			c = ConvertLineToRecord(Line);
			if(c.AccountNumber==AccountNumber){
				MyFile.close();
				return true;
		}
		MyFile.close();
		return false;
	}
	

	}
}

sClient ReadNewClient() {
	sClient Client;

	cout << "Enter Account Number?";
	getline(cin >> ws, Client.AccountNumber);

	while (IsExistedClient(Client.AccountNumber)) {
		cout << "\nClient with [" << Client.AccountNumber << "] already exists,Enter anothe account number? ";
		getline(cin, Client.AccountNumber);
	}
	
	cout << "Enter PinCode?";
	getline(cin , Client.PinCode);

	cout << "Enter Name?";
	getline(cin, Client.Name);

	cout << "Enter Phone?";
	getline(cin, Client.Phone);

	cout << "Enter Account Balance?";
	cin >> Client.AccountBalance;

	return Client;

}

void AddnewClientToFile(string FileName, string Client) {
	fstream MyFile;
	MyFile.open(FileName,ios::out| ios::app);
	if (MyFile.is_open()) {

		MyFile << Client <<endl;

		MyFile.close();
	}
}

void AddNewClient() {

	sClient Client = ReadNewClient();
	AddnewClientToFile(ClientsFile, ConvertRecordToLine(Client));
	
}

void AddNewClients() {

	char a = 'y';
	do {
		
		cout << "Adding New Client:\n\n";

		AddNewClient();
		cout << "\n\nClient Added Successfully, do you want to add more clients?";
		
		cin >> a;

	} while (tolower(a) == 'y');

}

void PrintClient(sClient Client) {
	cout << "| " << setw(15) << left << Client.AccountNumber;
	cout << "| " << setw(10) << left << Client.PinCode ;
	cout << "| " << setw(30) << left << Client.Name ;
	cout << "| " << setw(10) << left << Client.Phone ;
	cout << "| " << setw(10) << left << Client.AccountBalance ;
}

void PrintAllClientsData(vector<sClient> vClients) {
	cout << "\n\t\t\tClient List (" << vClients.size() << ") Cllient(s).";
	cout << "\n________________________________________________________";
	cout << "________________________________________________________\n\n";
	if (vClients.size() > 0) {
		cout << "| " << setw(15) << left << "Account Number";
		cout << "| " << setw(10) << left << "Pin Code";
		cout << "| " << setw(30) << left << "Client Name";
		cout << "| " << setw(10) << left << "Phone";
		cout << "| " << setw(10) << left << "Balance";

		cout << "\n_______________________________________________________";
		cout << "________________________________________________________\n\n";

		for (sClient c : vClients) {
			PrintClient(c);
			cout << endl;
		}
	}
	else {
		cout << "\nNo Client on the system";
	}
	
	cout << "\n\n________________________________________________________";
	cout << "________________________________________________________\n\n";
}

short ReadMenue(short to = 6) {
	short c = 0;
	cout << "Choose wat do you want to do? [1 to "<< to << "] ? ";
	cin >> c;
	return c;
}

void ShowClientsScreen() {
	vector<sClient>vClients = LoadClientsDataFromFile(ClientsFile);
	PrintAllClientsData(vClients);
}

void ShowExitScreen() {
	cout << "\n===================================\n";
	cout << "\tProgram Ends ^ ^";
	cout << "\n===================================\n";

	system("pause>0");
}

void BackToMainMenue() {
	cout << "\n\nPress any key to go back to Main Menue...";
	system("pause>0");
	ShowMainMenue();
}

void BackToTransactionsMenue() {

	cout << "\n\nPress any key to go back to Transactions Menue...";
	system("pause>0");
	ShowTransactionsMenueScreen();
}

void ShowAddNewClientsScreen() {
	cout << "\n--------------------------------\n";
	cout << "\tAdd New Clients Screen\n";
	cout << "\n--------------------------------\n";
	AddNewClients();

}

void ShowDeleteClientScreen() {
	cout << "\n--------------------------------\n";
	cout << "\tDelete Client Screen\n";
	cout << "\n--------------------------------\n";
	string AccountNumber = ReadAccountNumber();
	vector<sClient> vClients = LoadClientsDataFromFile(ClientsFile);

	DeleteClientByAccountNumber(AccountNumber,vClients);
}

void ShowUpdateClientScreen() {
	cout << "\n--------------------------------\n";
	cout << "\tUpdate Client Screen\n";
	cout << "\n--------------------------------\n";
	string AccountNumber = ReadAccountNumber();
	vector<sClient> vClients = LoadClientsDataFromFile(ClientsFile);

	UpdateClientByAccountNumber(AccountNumber, vClients);
}

void ShowFindClientScreen() {
	cout << "\n--------------------------------\n";
	cout << "\tFind Client Screen\n";
	cout << "\n--------------------------------\n";
	string AccountNumber = ReadAccountNumber();
	vector<sClient> vClients = LoadClientsDataFromFile(ClientsFile);
	sClient c;

	if (FindClientByAccountNumber(AccountNumber, vClients, c)) {
		PrintClientCard(c);
	}
	else {
		cout << "\nClient with [" << AccountNumber << "] Not Found.\n";
	}
}

bool DepositBalanceToClientByAccountNumber(string AccountNumber,double Ammount,vector <sClient> &vClients) {
		
	char a = 'm';

	cout << "\n\nAre you sure want to perform this transaction? y/n";
	cin >> a;

	if (tolower(a) == 'y') {
		for (sClient& c : vClients) {
			if (c.AccountNumber == AccountNumber) {

				c.AccountBalance += Ammount;
				SaveClientsToFile(ClientsFile, vClients);
				cout << "\nTransaction done successfully.";

				cout << "\n\nNew Balance is: " << c.AccountBalance;
				return true;
			}
		}
	}
	
	return false;

}

void ShowDepositScreen() {
	cout << "\n--------------------------------\n";
	cout << "\tDeposit Screen\n";
	cout << "\n--------------------------------\n";	
	sClient Client;
	vector <sClient> vClients = LoadClientsDataFromFile(ClientsFile);
	string AccountNumber = ReadAccountNumber();

	while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		cout << "Client with [" << AccountNumber << "] does not exist.\n";
		AccountNumber = ReadAccountNumber();
	}
	PrintClientCard(Client);

	double Ammount = 0;

	cout << "\n\nPlease Enter deposite amount?";
	cin >> Ammount;

	DepositBalanceToClientByAccountNumber(AccountNumber,Ammount,vClients);
}

void PrintBalancesList() {

	vector<sClient> vClients = LoadClientsDataFromFile(ClientsFile);

	double TotalBalances = 0;
	cout << "\n\t\t\Balance List (" << vClients.size() << ") Cllient(s).";
	cout << "\n________________________________________________________";
	cout << "________________________________________________________\n\n";
	cout << "| " << setw(15) << left << "Account Number";
	cout << "| " << setw(30) << left << "Client Name";
	cout << "| " << setw(10) << left << "Balance";
	cout << "\n_______________________________________________________";
	cout << "________________________________________________________\n\n";

	if (vClients.size() > 0) {
		
		for (sClient Client : vClients) {
			cout << "| " << setw(15) << left << Client.AccountNumber;
			cout << "| " << setw(30) << left << Client.Name;
			cout << "| " << setw(10) << left << Client.AccountBalance;
			cout << endl;
			TotalBalances += Client.AccountBalance;
		}

	}
	else {
		cout << "\nNo Client on the system";
	}

	cout << "\n\n________________________________________________________";
	cout << "________________________________________________________\n\n";

	cout << "\t\t\tTotal Balances = " << TotalBalances << endl;

	
}

void ShowBalancesListScreen() {
	
	PrintBalancesList();
}

void ShowWithdrowScreen() {
	cout << "\n--------------------------------\n";
	cout << "\Withdrow Screen\n";
	cout << "\n--------------------------------\n";

	vector <sClient> vClients = LoadClientsDataFromFile(ClientsFile);
	string AccountNumber = ReadAccountNumber();
	sClient Client;

	while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		cout << "Client with [" << AccountNumber << "] does not exist.\n";
		AccountNumber = ReadAccountNumber();
	}

	PrintClientCard(Client);

	double Ammount = 0;
	cout << "\n\nPlease Enter Withdrow amount?";
	cin >> Ammount;
	
	while (Ammount > Client.AccountBalance)
	{
		cout << "\nAmount Exceeds the balance, you can withdraw Up to : " << Client.AccountBalance;
		cout << "\n\nEnter Ammount Again?";
		cin >> Ammount;
	}

	DepositBalanceToClientByAccountNumber(AccountNumber, Ammount*-1, vClients);
}

void PerformTransactionsMenue(enTrasationsMenue enChoise) {
	switch (enChoise)
	{
	case enTrasationsMenue::eDeposit:
		system("cls");
		ShowDepositScreen();
		BackToTransactionsMenue();
		break;
	case enTrasationsMenue::eWithdrow:
		system("cls");
		ShowWithdrowScreen();
		BackToTransactionsMenue();
		break;
	case enTrasationsMenue::eTotalBalance:
		system("cls");
		ShowBalancesListScreen();
		BackToTransactionsMenue();

		break;
	case enTrasationsMenue::eMainMenue:
		ShowMainMenue();
	}
}

void ShowTransactionsMenueScreen() {
	system("cls");
	cout << "\n--------------------------------\n";
	cout << "\tTransactions Menue Screen";
	cout << "\n--------------------------------\n";

	cout << "\t[1] Deposite.\n";
	cout << "\t[2] Withdraw.\n";
	cout << "\t[3] Total Balances.\n";
	cout << "\t[4] Main Menue.\n";
	cout << "====================================\n";

	PerformTransactionsMenue((enTrasationsMenue)ReadMenue(4));


}

void PerformMainMenue(enMainMenueChoice enChoice) {
	switch (enChoice)
	{
	case enMainMenueChoice::ListClient:
		system("cls");
		ShowClientsScreen();
		BackToMainMenue();
		break;
	case enMainMenueChoice::AddClient:
		system("cls");
		ShowAddNewClientsScreen();
		BackToMainMenue();
		break;
	case enMainMenueChoice::DeleteClient:
		system("cls");
		ShowDeleteClientScreen();
		BackToMainMenue();
		break;
	case enMainMenueChoice::UpdateClient:
		system("cls");
		ShowUpdateClientScreen();
		BackToMainMenue();
		break;
	case enMainMenueChoice::FindClient:
		system("cls");
		ShowFindClientScreen();
		BackToMainMenue();
		break;
	case enMainMenueChoice::eTransaction:
		system("cls");
		ShowTransactionsMenueScreen();
		break;
	case enMainMenueChoice::Exit:
		system("cls");
		ShowExitScreen();
		break;
	}
}

void ShowMainMenue() {
	system("cls");
	cout << "====================================\n";
	cout << "\tMain Menue Screen\n";
	cout << "====================================\n";

	cout << "\t[1] Show Client List.\n";
	cout << "\t[2] Add New Client.\n";
	cout << "\t[3] Delete Client.\n";
	cout << "\t[4] Update Client Info.\n";
	cout << "\t[5[ Find Client.\n";
	cout << "\t[6[ Transactions.\n";
	cout << "\t[7] Exit.\n";

	cout << "====================================\n";
	PerformMainMenue((enMainMenueChoice)ReadMenue());




}

int main() {

	ShowMainMenue();
	return 0;
}