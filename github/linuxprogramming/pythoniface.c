#include<stdio.h>
#include<stdlib.h>
#include<Python.h>

void test()
{
    Py_Initialize();       
    FILE * fp = fopen("hello.py", "r");       
    if (fp == NULL)
        return;       
    PyRun_SimpleFile(fp, "hello.py");       
    Py_Finalize();       
}

int main()
{
	PyObject *pName, *pModule, *pDict, *pFunc, *pValue, *pArgs,*pList,*pRet;

	//Initialize the python Interpreter
	Py_Initialize();

	//Check the IsInitialized
	if(!Py_IsInitialized()){
			printf("is initialized no\n");
			return -1;
	}
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append('./')");
	//Build the name object, python filename
	pName = PyString_FromString("hello");
	if (!pName){
			printf("pname --test no\n");
	}
	//Load the module object
	pName = Py_BuildValue("s", "hello");
	pModule = PyImport_Import(pName);
	//Check pModule
	if(!pModule){
			printf("test.py no\n");
			return -1;
	}
	//pDict is a borrowed reference
	pDict = PyModule_GetDict(pModule);
	if(!pDict){
			printf("pDict no\n");
	}

	//pFunc is also a borrowed refernce, find the function
	pFunc = PyDict_GetItemString(pDict, "test");
	if(pFunc == NULL)
	{
			printf("pfunc == null\n");
	}
	//pFunc = PyObject_GetAttrString(pModule, "test");
	if(!pFunc ||!PyCallable_Check(pFunc)){
			printf("pFunc no \n");
			return -1;
	}
	pList = PyList_New(2);
	PyList_SetItem(pList,0,Py_BuildValue("i",1));
	PyList_SetItem(pList,1,Py_BuildValue("s","hello"));
	pArgs = PyTuple_New(1);
	//PyTuple_SetItem(pArgs,0,Py_BuildValue("s","hello"));
	PyTuple_SetItem(pArgs,0,pList);

	pRet = PyObject_CallObject(pFunc,pArgs);
	//get return value
	if(pRet == NULL){
		printf("pret is null\n");
		return -1;
	}
	int i = 0, s = PyList_Size(pRet);
	printf("pret size=  %d\n",s);
	char * t = PyString_AsString(PyList_GetItem(pRet,0));
	long a = PyInt_AsLong(PyList_GetItem(pRet,1));
	printf("t= %s,a= %ld",t,a);

	//release resource
	Py_DECREF(pModule);
	Py_DECREF(pName);
	Py_DECREF(pDict);
	Py_DECREF(pFunc);
	Py_DECREF(pArgs);
	Py_DECREF(pRet);
	Py_DECREF(pList);
	Py_Finalize();
	return 0;
}

