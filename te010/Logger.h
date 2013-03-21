#pragma once
// display flags
const unsigned char LOGFLAG_WRITE_TO_LOG_FILE =	1 << 0;
const unsigned char LOGFLAG_WRITE_TO_DEBUGGER =	1 << 1;

namespace Logger
{
	class ErrorMessenger
	{
		bool m_Enabled;

	public:
		ErrorMessenger(void);
		void Show(const std::string & errorMessage, bool isFatal, const char* funcName, const char* sourceFile, unsigned int lineNum);
	};

	void Init(const char* loggingConfigFilename);
	void Destroy(void);

	// logging functions
	void Log(const std::string & tag, const std::string& message, const char* funcName, const char* sourceFile, unsigned int lineNum);
	void SetDisplayFlags(const std::string & tag, unsigned char flags);
};

//-------------------------------------------------------------------
//	Debug Macros
//-------------------------------------------------------------------

// fatal errors always presented to the user
#define NAIA_FATAL(str) \
	do \
	{ \
		static Logger::ErrorMessenger* pErrorMessenger = new Logger::ErrorMessenger; \
		std::string s((str)); \
		pErrorMessenger->Show(s, true, __FUNCTION__, __FILE__, __LINE__); \
	} \
	while (0) \

#define NAIA_ASSERT(expr) \
	do \
	{ \
		if (!(expr)) \
		{ \
			static Logger::ErrorMessenger* pErrorMessenger = new Logger::ErrorMessenger; \
			pErrorMessenger->Show(#expr, false, __FUNCTION__, __FILE__, __LINE__); \
		} \
	} \
	while (0) \

// errors as a dialog with abort, retry and ignore
#define NAIA_ERROR(str) \
	do \
	{ \
		static Logger::ErrorMessenger* pErrorMessenger = new Logger::ErrorMessenger; \
		std::string s((str)); \
		pErrorMessenger->Show(s, false, __FUNCTION__, __FILE__, __LINE__); \
	} \
	while(0) \

// just a log with "WARNING" tag
#define NAIA_WARNING(str) \
	do \
	{ \
		std::string s((str)); \
		Logger::Log("WARNING",s,__FUNCTION__,__FILE__, __LINE__); \
	} \
	while(0) \

// just a log with "INFO" tag
#define NAIA_INFO(str) \
	do \
	{ \
		std::string s((str)); \
		Logger::Log("INFO", s, NULL, NULL, 0); \
	} \
	while(0) \

#define NAIA_LOG(tag, str) \
	do \
	{ \
		std::string s((str)); \
		Logger::Log(tag,s,NULL,NULL, 0); \
	} \
	while(0) \

//-------------------------------------------------------------------
//	Log manager class declaration
//-------------------------------------------------------------------
class LogManager
{
public:
	enum ErrorDialogResult
	{
		LOGMGR_ERROR_ABORT,
		LOGMGR_ERROR_RETRY,
		LOGMGR_ERROR_IGNORE
	};

	typedef std::map<std::string, unsigned char> Tags;
	typedef std::list<Logger::ErrorMessenger*> ErrorMessengerList;

	Tags m_tags;
	ErrorMessengerList m_errorMessengers;

	// thread safety
	CriticalSection m_tagCriticalSection;
	CriticalSection m_messengerCriticalSection;

public:
	LogManager(void);
	~LogManager(void);

	void Init(const char* loggingConfigFilename);

	// logs
	void Log(const std::string& tag, const std::string& message, const char* funcName, const char* sourceFile, unsigned int lineNum);
	void SetDisplayFlags(const std::string & tag, unsigned char flags);

	// error messengers
	void AddErrorMessenger(Logger::ErrorMessenger* pMessenger);
	LogManager::ErrorDialogResult Error(const std::string& errorMessage, bool isFatal, const char* funcName, const char* sourceFile, unsigned int lineNum);

private:
	// log helpers
	void OutputFinalBufferToLogs(const std::string & finalBuffer, unsigned char flags);
	void WriteToLogFile(const std::string& data);
	void GetOutputBuffer(std::string& outOutputBuffer, const std::string& tag, const std::string& message, const char* funcName, const char* sourceFile, unsigned int lineNum);
};

