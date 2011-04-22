// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the DATABASEINTERFACE_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// DATABASEINTERFACE_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef DATABASEINTERFACE_EXPORTS
#define DATABASEINTERFACE_API __declspec(dllexport)
#else
#define DATABASEINTERFACE_API __declspec(dllimport)
#endif

// This class is exported from the DataBaseInterface.dll
class DATABASEINTERFACE_API CDataBaseInterface {
public:
	CDataBaseInterface(void);
	// TODO: add your methods here.
};

extern DATABASEINTERFACE_API int nDataBaseInterface;

DATABASEINTERFACE_API int fnDataBaseInterface(void);
