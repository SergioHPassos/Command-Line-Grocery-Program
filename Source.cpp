#include <Python.h>
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <vector>
#include <string>
#undef max

using namespace std;

/*
Description:
	To call this function, simply pass the function name in Python that you wish to call.
Example:
	callProcedure("printsomething");
Output:
	Python will print on the screen: Hello from python!
Return:
	None
*/
void CallProcedure(string pName)
{
	char* procname = new char[pName.length() + 1];
	strcpy(procname, pName.c_str());

	Py_Initialize();
	PyObject* my_module = PyImport_ImportModule("PythonCode");
	PyErr_Print();
	PyObject* my_function = PyObject_GetAttrString(my_module, procname);
	PyObject* my_result = PyObject_CallObject(my_function, NULL);
	Py_Finalize();

	delete[] procname;
}

/*
Description:
	To call this function, pass the name of the Python functino you wish to call and the string parameter you want to send
Example:
	int x = callIntFunc("PrintMe","Test");
Output:
	Python will print on the screen:
		You sent me: Test
Return:
	100 is returned to the C++
*/
int callIntFunc(string proc, string param)
{
	char* procname = new char[proc.length() + 1];
	strcpy(procname, proc.c_str());

	char* paramval = new char[param.length() + 1];
	strcpy(paramval, param.c_str());


	PyObject* pName, * pModule, * pDict, * pFunc, * pValue = nullptr, * presult = nullptr;
	// Initialize the Python Interpreter
	Py_Initialize();
	// Build the name object
	pName = PyUnicode_FromString((char*)"PythonCode");
	// Load the module object
	pModule = PyImport_Import(pName);
	// pDict is a borrowed reference 
	pDict = PyModule_GetDict(pModule);
	// pFunc is also a borrowed reference 
	pFunc = PyDict_GetItemString(pDict, procname);
	if (PyCallable_Check(pFunc))
	{
		pValue = Py_BuildValue("(z)", paramval);
		PyErr_Print();
		presult = PyObject_CallObject(pFunc, pValue);
		PyErr_Print();
	}
	else
	{
		PyErr_Print();
	}
	//printf("Result is %d\n", _PyLong_AsInt(presult));
	Py_DECREF(pValue);
	// Clean up
	Py_DECREF(pModule);
	Py_DECREF(pName);
	// Finish the Python Interpreter
	Py_Finalize();

	// clean 
	delete[] procname;
	delete[] paramval;


	return _PyLong_AsInt(presult);
}

/*
Description:
	To call this function, pass the name of the Python functino you wish to call and the string parameter you want to send
Example:
	int x = callIntFunc("doublevalue",5);
Return:
	25 is returned to the C++
*/
int callIntFunc(string proc, int param)
{
	char* procname = new char[proc.length() + 1];
	strcpy(procname, proc.c_str());

	PyObject* pName, * pModule, * pDict, * pFunc, * pValue = nullptr, * presult = nullptr;
	// Initialize the Python Interpreter
	Py_Initialize();
	// Build the name object
	pName = PyUnicode_FromString((char*)"PythonCode");
	// Load the module object
	pModule = PyImport_Import(pName);
	// pDict is a borrowed reference 
	pDict = PyModule_GetDict(pModule);
	// pFunc is also a borrowed reference 
	pFunc = PyDict_GetItemString(pDict, procname);
	if (PyCallable_Check(pFunc))
	{
		pValue = Py_BuildValue("(i)", param);
		PyErr_Print();
		presult = PyObject_CallObject(pFunc, pValue);
		PyErr_Print();
	}
	else
	{
		PyErr_Print();
	}
	//printf("Result is %d\n", _PyLong_AsInt(presult));
	Py_DECREF(pValue);
	// Clean up
	Py_DECREF(pModule);
	Py_DECREF(pName);
	// Finish the Python Interpreter
	Py_Finalize();

	// clean 
	delete[] procname;

	return _PyLong_AsInt(presult);
}


// display menu to console
void displayMenu()
{
	cout << "1: Frequency Of Items" << endl;
	cout << "2: Frequency Of A Specific Item" << endl;
	cout << "3: Display Histogram " << endl;
	cout << "4: Exit" << endl;
	cout << "Enter your selection as a number 1, 2, 3 or 4: " << endl;

}


// get user input
int getUserInput(int& userInput)
{
	// read input
	cin >> userInput;

	// validate user input
	if (cin.fail())
	{
		// reset strema's internal error state flags
		cin.clear();

		// extract characters from the input sequence and discards them
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		// clear console
		system("cls");

		// display error
		cout << "Invalid, Please try again..." << endl;

		return -1;
	}

	//
	cout << endl;
	return 1;
}


// get user item
int getUserItem(string& userItem) 
{
	// read input
	cin >> userItem;

	// cross check with list
	if (callIntFunc("crossCheck", userItem) == -1) 
	{
		// reset strema's internal error state flags
		cin.clear();

		// extract characters from the input sequence and discards them
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		// clear console
		system("cls");

		// display error
		cout << "Invalid item, try again....\n";
		return -1;
	}

	//
	cout << endl;
	return  1;
}


// entry point
int main()
{
	/*
	* convert string to const wchar_t
	* pythonhome was not set, so I set it manually
	*/
	//string pyHome = "C:\\Users\\NewUser\\AppData\\Local\\Programs\\Python\\Python39";
	//wstring widestr = wstring(pyHome.begin(), pyHome.end());
	//Py_SetPythonHome(widestr.c_str());

	
	// stores users input
	int userInput;
	string userItem;

	// main loop
	while (true) 
	{
		// get valid input for menu
		do
		{
			// display menu
			displayMenu();
		} while (getUserInput(userInput) == -1);

		// clear console
		system("cls");

		// handles input execution
		if (userInput == 4) 
		{ 
			// exit program
			break;  
		}
		else if (userInput == 3) 
		{
			// save items to .dat with freuqnecy
			int maxLength = callIntFunc("saveDataForHistogram", 0);

			// read and display histogram
			ifstream file("frequency.dat");
			if (file.is_open())
			{
				int value;
				int index;
				string item;
				string line;
				// read line by line
				while (getline(file, line))
				{
					// split data
					index = line.find(" ");
					item = line.substr(0, index);
					value = stoi(line.substr(index, line.length()));

					// add spaces for items shorter than the longest item
					// for making it pretty
					for (int i = 0; i < (maxLength - item.length()); i++)
					{
						cout << ' ';
					}

					// than display the item with the the total count
					cout << item;
					for (int i = 0; i < value; i++) 
					{
						cout << '*';
					}
					cout << endl;

				}
			}

			// newline
			cout << endl;
		}
		else if (userInput == 2) 
		{
			// get valid user input
			do
			{
				// ask for item
				cout << "Please enter the item name: \n";
			} while (getUserItem(userItem) == -1);
			
			cout << userItem << " x" << callIntFunc("returnFrequencyOfItem", userItem) << "\n\n";
		}
		else if (userInput == 1) 
		{
			CallProcedure("frequencyOfItems");
		}
	}





	return 0;

}