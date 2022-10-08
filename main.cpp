#include <iostream>
#include <string.h>
#include <sstream>
#include <windows.h>
#include <mysql.h>
#include <mysqld_error.h>

using namespace std;
using std::string;

#include "Person.h"

MYSQL *obj;
MYSQL_ROW row;
MYSQL_RES *res;

char HOST[] = "localhost";
char USER[] = "c++";
char PASSWORD[] = "c++";
char DATABASE[] = "person_test";

void insertPerson(MYSQL *obj, char Name[100], int Age) {
	char *consult;
	char *sentence;
	string sentence_aux;
	// Setting our update.
	sentence_aux = "INSERT INTO first_test(id, fullname, age) VALUES(NULL, '%s', '%i')";
	sentence = new char[sentence_aux.length()+1];
	strcpy(sentence, sentence_aux.c_str());
						
	consult = new char[strlen(sentence)+strlen(Name)+sizeof(Age)];
	sprintf(consult, sentence, Name, Age);
	// Make our attempt.
	if(mysql_ping(obj)) {
		cout<<"ERROR: Imposible to connect."<<endl;
		cout<<mysql_error(obj)<<endl;
	}
	if(mysql_query(obj, consult)) {
		cout<<"ERROR: "<<mysql_error(obj)<<endl;
		rewind(stdin);
		getchar();
	} else {
		cout<<"Info added correctly."<<endl;
	}
	mysql_store_result(obj);
}

void selectPeople(MYSQL *obj) {
	char *consult;
	char *sentence;
	string sentence_aux;
	// Setting our update.
	sentence_aux = "SELECT * FROM first_test";
	sentence = new char[sentence_aux.length()+1];
	strcpy(sentence, sentence_aux.c_str());
						
	consult = new char[strlen(sentence)];
	sprintf(consult, sentence);
	// Make our attempt.
	if(mysql_ping(obj)) {
		cout<<"ERROR: Imposible to connect."<<endl;
		cout<<mysql_error(obj)<<endl;
	}
	if(mysql_query(obj, consult)) {
		cout<<"ERROR: "<<mysql_error(obj)<<endl;
		rewind(stdin);
		getchar();
	} else {
		cout<<"Info recuperacted correctly."<<endl;
	}
	res = mysql_use_result(obj);
	while((row = mysql_fetch_row(res))) {
		cout<<"********************* Person #"<<row[0]<<" ****************************"<<endl;
		printf("Id: %s\n", row[0]);
		printf("Full Name: %s\n", row[1]);
		printf("Age: %s\n", row[2]);
	}
	cout<<endl;
}

void selectPersonByName(MYSQL *obj, char Name[100]) {
	char *consult;
	char *sentence;
	string sentence_aux;
	
	// Setting our update.
	sentence_aux = "SELECT * FROM first_test WHERE fullname='%s'";
	sentence = new char[sentence_aux.length()+1];
	strcpy(sentence, sentence_aux.c_str());
						
	consult = new char[strlen(sentence)+strlen(Name)];
	sprintf(consult, sentence, Name);
	// Make our attempt.
	if(mysql_ping(obj)) {
		cout<<"ERROR: Imposible to connect."<<endl;
		cout<<mysql_error(obj)<<endl;
	}
	if(mysql_query(obj, consult)) {
		cout<<"ERROR: "<<mysql_error(obj)<<endl;
		rewind(stdin);
		getchar();
	} else {
		cout<<"Info recuperacted correctly."<<endl;
	}
	res = mysql_use_result(obj);
	while((row = mysql_fetch_row(res))) {
		cout<<"********************* Person #"<<row[0]<<" ****************************"<<endl;
		printf("Id: %s\n", row[0]);
		printf("Full Name: %s\n", row[1]);
		printf("Age: %s\n", row[2]);
	}
	cout<<endl;
}

void selectPersonById(MYSQL *obj, int Id) {
	char *consult;
	char *sentence;
	string sentence_aux;
	
	// Setting our update.
	sentence_aux = "SELECT * FROM first_test WHERE id='%i'";
	sentence = new char[sentence_aux.length()+1];
	strcpy(sentence, sentence_aux.c_str());
						
	consult = new char[strlen(sentence)+sizeof(Id)];
	sprintf(consult, sentence, Id);
	// Make our attempt.
	if(mysql_ping(obj)) {
		cout<<"ERROR: Imposible to connect."<<endl;
		cout<<mysql_error(obj)<<endl;
	}
	if(mysql_query(obj, consult)) {
		cout<<"ERROR: "<<mysql_error(obj)<<endl;
		rewind(stdin);
		getchar();
	} else {
		cout<<"Info recuperacted correctly."<<endl;
	}
	res = mysql_use_result(obj);
	while((row = mysql_fetch_row(res))) {
		cout<<"********************* Person #"<<row[0]<<" ****************************"<<endl;
		printf("Id: %s\n", row[0]);
		printf("Full Name: %s\n", row[1]);
		printf("Age: %s\n", row[2]);
	}
	cout<<endl;
}

void deletePerson(MYSQL *obj, int Id) {
	char *consult;
	char *sentence;
	string sentence_aux;
	
	// Setting our update.
	sentence_aux = "DELETE FROM first_test WHERE id='%i'";
	sentence = new char[sentence_aux.length()+1];
	strcpy(sentence, sentence_aux.c_str());
						
	consult = new char[strlen(sentence)+sizeof(Id)];
	sprintf(consult, sentence, Id);
	// Make our attempt.
	if(mysql_ping(obj)) {
		cout<<"ERROR: Imposible to connect."<<endl;
		cout<<mysql_error(obj)<<endl;
	}
	if(mysql_query(obj, consult)) {
		cout<<"ERROR: "<<mysql_error(obj)<<endl;
		rewind(stdin);
		getchar();
	} else {
		cout<<"Delete person was corrected."<<endl;
	}
	cout<<endl;
}

int main(int argc, char** argv) {
	system("color 02");
	
	int Id;
	char Name[100];
	int Age;
	
	bool ProgramIsOpened = true;
	int answer;
	
	// char *consult;
	// char *sentence;
	// string sentence_aux;
	
	int opc=0;
	
	// -----------------------------------------------------------------------------
	// Connection
	if(!(obj = mysql_init(0))) {
		cout<<"ERROR: MySQL object could not be created."<<endl;
	} else {
		if(!mysql_real_connect(obj, HOST, USER, PASSWORD, DATABASE, 3306, NULL, 0)) {
			cout<<"ERROR: Some database info is wrong or do not exist."<<endl;
			cout<<mysql_error(obj)<<endl;
		} else {
			do {
				system("cls");
				cout<<"Logged in."<<endl<<endl;
				cout<<"\t\t\tMenu"<<endl
					<<"1. Insert person"<<endl
					<<"2. Show people"<<endl
					<<"3. Show person by name"<<endl
					<<"4. Delete person"<<endl
					<<"0. Salir"<<endl;
				cin>>opc;
				cin.ignore(100, '\n');
				switch(opc){
					case 1:
						cout<<"*************************************** Ingrese los datos del empleado ***************************************"<<endl<<endl;
						cout<<"Full name: ";
						cin.getline(Name, 100, '\n');
						
						cout<<"Age: ";
						cin>>Age;
						cin.ignore(100, '\n');
						
						insertPerson(obj, Name, Age);
						
						system("pause");
						break;
					case 2:
						cout<<"*************************************** Lista de Empleados ***************************************"<<endl;
						
						selectPeople(obj);
						
						system("pause");
						break;
					case 3:
						cout<<"Who is the person what do you want to find: ";
						cin.getline(Name, 100, '\n');
						
						selectPersonByName(obj, Name);
						
						system("pause");
						break;
					case 4:
						cout<<"Insert id from the person what you want to delete: ";
						cin>>Id;
						cin.ignore(100, '\n');
						
						selectPersonById(obj, Id);
						
						deletePerson(obj, Id);
						
						system("pause");
						break;
					case 0:
						cout<<endl<<"Another?"<<endl;
						cout<<"[1]: Yes"<<endl;
						cout<<"[0]: No"<<endl;
						cout<<"Answer: ";
						cin>>answer;
						cin.ignore(100,'\n');
						switch(answer){
							case 1:
								//system("pause");
								break;
							case 0:
								ProgramIsOpened = false;
								//system("pause");
								break;
							default:
								cout<<"Te has equivocado de opcion"<<endl;
								//system("pause");
								break;
						}
						system("pause");
						break;
					default:
						cout<<"Te has equivocado de opcion"<<endl;
						system("pause");
						break;
				}
			} while(ProgramIsOpened);
		}
	}
	cout<<"BYE!!!"<<endl;
	
	system("pause");
	return 0;
}
