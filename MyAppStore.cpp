#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <math.h>
using namespace std;
#include <cstring>
#include <string.h>

#define CAT_NAME_LEN 25
#define APP_NAME_LEN 50
#define VERSION_LEN 10
#define UNIT_SIZE 3

//Structure of each AppInfo node of BST
struct app_info
{
	char category[CAT_NAME_LEN];  // Name of category
	char app_name[APP_NAME_LEN];  // Name of the application
	char version[VERSION_LEN];    // Version number
	float size;                   // Size of the application
	char units[UNIT_SIZE];        // GB or MB
	float price;                  // Price in $ of the application
};

// A binary search tree
struct tree
{
	struct app_info app;          // Information about the application
	struct tree *left = NULL;     // Pointer to the left subtree 
	struct tree *right = NULL;      // Pointer to the right subtree
	tree()
	{}
};

//structure of each category
struct Category
{
	char category[CAT_NAME_LEN];
	struct tree *root = NULL;
};

struct hash_table_entry
{
	char app_name[APP_NAME_LEN];  // Name of the application
	struct tree *app_node=NULL;        // Pointer to node in tree containing the application information
};

//Function to determine primality
bool isPrime(int n)
{
	int i, root;
	if (n % 2 == 0 || n % 3 == 0)
		return 0;
	root = (int)sqrt(n);
	for (i = 5; i <= root; i += 6)
	{
		if (n%i == 0)
			return 0;
	}
	for (i = 7; i <= root; i += 6)
	{
		if (n%i == 0)
			return 0;
	}
	return 1;
}

//Function which returns the smallest prime number greater than given number
int GetNextPrime(int x)
{
	int i = x + 1;
	while (1)
	{
		if (isPrime(i))
			break;
		i++;
	}
	return i;
}

//Function to compute ASCII equivalent of a char array
int ComputeASCIIEquivalent(char *a)
{
	int ASCIISum = 0;
	while (*a != '\0')
	{
		ASCIISum += (int)*a;
		a++;
	}
	return ASCIISum;
}

int ComputeHashValue(int x, int hashTableSize)
{
	return (x % hashTableSize);
}

tree InsertNode(string appCategory, string appName, string appVersion, string appSize, string appUnit, string appPrice)
{
	tree *currentNode;
	currentNode = new tree();
	strcpy((*currentNode).app.category, (char*)appCategory.c_str());
	strcpy((*currentNode).app.app_name, (char*)appName.c_str());
	strcpy((*currentNode).app.version, (char*)appVersion.c_str());
	(*currentNode).app.size = stof(appSize);
	strcpy((*currentNode).app.units, (char*)appUnit.c_str());
	(*currentNode).app.price = stof(appPrice);
	return (*currentNode);
}

void Inorder(tree *p)
{
	if (p != NULL)
	{
		Inorder((*p).left);
		cout << "\n" << (*p).app.app_name<<"\n";
		Inorder((*p).right);
	}
}

int main()
{
	Category *CatArray;
	hash_table_entry *HashTable;
	string input;
	int n = 0, m = 0;
	getline(cin, input);
	n = atoi(input.c_str());

	if ((CatArray = (Category*)malloc(n*sizeof(Category))) == NULL)
		perror("Array allocation error!");
	
	//Reading all categories
	for (int i = 0; i < n; i++)
	{
		getline(cin, input);
		strcpy(CatArray[i].category, (char*)input.c_str());
		CatArray[i].root = NULL;
	}

	getline(cin, input);
	m = atoi(input.c_str());

	int nextPrime = GetNextPrime(2 * m);
	if ((HashTable = (hash_table_entry*)malloc(nextPrime*sizeof(hash_table_entry))) == NULL)
		perror("Hash table allocation error!");
	
	//Initializing the hash table pointers to null
	for (int a = 0; a < nextPrime; a++)
	{
		HashTable[a].app_node = NULL;
	}

	for (int i = 0; i < m; i++)
	{
		string appCategory, appName, appVersion, appSize, appUnit, appPrice;
		getline(cin, appCategory);
		getline(cin, appName);
		getline(cin, appVersion);
		getline(cin, appSize);
		getline(cin, appUnit);
		getline(cin, appPrice);

		for (int j = 0; j < n; j++)
		{
			if (CatArray[j].category == appCategory)
			{
				if (CatArray[j].root == NULL)
				{
					CatArray[j].root = &InsertNode(appCategory, appName, appVersion, appSize, appUnit, appPrice);
					break;
				}
				else
				{
					tree *a = NULL;
					a= new tree;
				
					a = CatArray[j].root;
					while (a != NULL)
					{
						if (appName <= (*a).app.app_name)
						{
							if ((*a).left == NULL)
							{
								//cout << &InsertNode(appCategory, appName, appVersion, appSize, appUnit, appPrice);
								(*a).left = &InsertNode(appCategory, appName, appVersion, appSize, appUnit, appPrice);
								break;
							}
							else
								a = (*a).left;
						}
						else
						{
							if ((*a).right == NULL)
							{
								//cout << &InsertNode(appCategory, appName, appVersion, appSize, appUnit, appPrice);
								(*a).right = &InsertNode(appCategory, appName, appVersion, appSize, appUnit, appPrice);
								break;
							}
							else
								a = (*a).right;
						}
					}//end of while

				}//end of else
			  }//end of outer if
		   }//end of category for
		
		int appASCIISum = ComputeASCIIEquivalent((char*)appName.c_str());
		int hashPosition = ComputeHashValue(appASCIISum, nextPrime);
		int pos = -1;

		//Make an entry in HashTable
		while (true)
		{
			if (hashPosition == pos)
			{
				cout << "\n\nHash table full, myAppstore exiting...";
				goto Exit;
			}
			if ((hashPosition == nextPrime - 1) && HashTable[hashPosition].app_node != NULL) //if we hit end of hash table, circle back to 1st position
				hashPosition = 0;
			if (HashTable[hashPosition].app_node != NULL) //keep probing linearly till we find an empty slot
			{
				pos = hashPosition;
				hashPosition++;
				continue;
			}
			else  //insert app details in hash table
			{
				strcpy(HashTable[hashPosition].app_name, (char*)appName.c_str());
				HashTable[hashPosition].app_node = &InsertNode(appCategory, appName, appVersion, appSize, appUnit, appPrice);
				break;
			}
		}//end of hash while

	}//end of app for

	while (true)
	{
		getline(cin, input);
		int strLen = input.length();
		string currentCmd = input.substr(0, 8);
		if (currentCmd == "find app")
		{
			string currentApp = input.substr(9, strLen - 1);
			int ASCIISum = ComputeASCIIEquivalent((char*)currentApp.c_str());
			int hashPosition = ComputeHashValue(ASCIISum, nextPrime);
			int pos = -1;
			struct tree appDisp;

			//Search hash table for the given app 
			while (true)
			{
				if (hashPosition == pos)
				{
					cout << "\n\nApplication not found.";
					break;
				}
				if ((hashPosition == nextPrime - 1) && HashTable[hashPosition].app_name != currentApp) //if we hit end of hash table, circle back to 1st position
					hashPosition = 0;
				if (HashTable[hashPosition].app_name != currentApp) //keep probing linearly till we find an empty slot
				{
					pos = hashPosition;
					hashPosition++;
					continue;
				}
				else  //insert app details in hash table
				{
					appDisp = *(HashTable[hashPosition].app_node);
					break;
				}
			}//end of hash while
			cout << "\n" << appDisp.app.app_name << "\nCategory: " << appDisp.app.category << "\nVersion: " << appDisp.app.version
				<< "\nSize: " << appDisp.app.size << "\nUnits: " << appDisp.app.units << "\nPrice: " << appDisp.app.price<<"\n";
					
		/*	
			while (HashTable[hashPosition].app_name != currentApp)
			{
				hashPosition++;
			}

			struct tree appDisp = *(HashTable[hashPosition].app_node);
			cout << "\n" << appDisp.app.app_name << "\nCategory: " << appDisp.app.category << "\nVersion: " << appDisp.app.version
				<< "\nSize: " << appDisp.app.size << "\nUnits: " << appDisp.app.units << "\nPrice: " << appDisp.app.price;*/

		}
		currentCmd = input.substr(0, 13);
		if (currentCmd == "find category")
		{
			string currentCat = input.substr(14, strLen - 1);
			for (int i = 0; i < n; i++)
			{
				if (CatArray[i].category == currentCat)
				{
					Inorder(CatArray[i].root);
				}
				
			}
		}

		currentCmd = input.substr(0, 9);
		if (currentCmd == "find price")
		{

		}

	}//end of while which reads commands
Exit: return 0;
}//end of main

