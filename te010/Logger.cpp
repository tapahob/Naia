#include "precompiled.h"
#include "time.h"
using std::string;

// the log filename
static const char* ERRORLOG_FILENAME = "error.log";

// default display flags
// default display flags
#ifdef _DEBUG
const unsigned char ERRRORFLAG_DEFAULT  = LOGFLAG_WRITE_TO_DEBUGGER;
const unsigned char WARNINGFLAG_DEFAULT = LOGFLAG_WRITE_TO_DEBUGGER;
const unsigned char LOGFLAG_DEFAULT     = LOGFLAG_WRITE_TO_DEBUGGER;
//const unsigned char ERRRORFLAG_DEFAULT  = (LOGFLAG_WRITE_TO_DEBUGGER | LOGFLAG_WRITE_TO_LOG_FILE);
//const unsigned char WARNINGFLAG_DEFAULT = (LOGFLAG_WRITE_TO_DEBUGGER | LOGFLAG_WRITE_TO_LOG_FILE);
//const unsigned char LOGFLAG_DEFAULT     = (LOGFLAG_WRITE_TO_DEBUGGER | LOGFLAG_WRITE_TO_LOG_FILE);
#else
const unsigned char ERRRORFLAG_DEFAULT  = 0;
const unsigned char WARNINGFLAG_DEFAULT = 0;
const unsigned char LOGFLAG_DEFAULT     = 0;
#endif

// singleton
class LogManager;
static LogManager* g_LogManager = NULL;

//-------------------------------------------------------------------
//	LogManager class definition
//-------------------------------------------------------------------

//-------------------------------------------------------------------
//	Constructor
//-------------------------------------------------------------------
LogManager::LogManager(void)
{
	m_messengerCriticalSection.Lock();
	for (auto it = m_errorMessengers.begin(); it != m_errorMessengers.end(); ++it)
	{
		Logger::ErrorMessenger* pMessenger = (*it);
		delete pMessenger;
	}

	m_errorMessengers.clear();
	m_messengerCriticalSection.Unlock();
}
LogManager::~LogManager(void)
{
}

//-------------------------------------------------------------------
//	Initialize the logger
//-------------------------------------------------------------------
void LogManager::Init(const char* loggingConfigFilename)
{
	if (loggingConfigFilename)
	{
		TiXmlDocument loggingConfigFile(loggingConfigFilename);
		if (loggingConfigFile.LoadFile())
		{
			TiXmlElement* pRoot = loggingConfigFile.RootElement();
			if (!pRoot)
				return;

			// loop through each child element and load the component
			for (auto pNode = pRoot->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
			{
				unsigned char flags = 0;
				std::string tag(pNode->Attribute("tag"));

				int debugger = 0;
				pNode->Attribute("debugger", &debugger);
				if (debugger)
					flags |= LOGFLAG_WRITE_TO_DEBUGGER;

				int logFile = 0;
				pNode->Attribute("file", &logFile);
				if (logFile)
					flags |= LOGFLAG_WRITE_TO_LOG_FILE;

				SetDisplayFlags(tag, flags);
			}
		}
	}
}

//-------------------------------------------------------------------
//	Build up a log string and output it according to the display flags
//-------------------------------------------------------------------
void LogManager::Log(const std::string& tag, const std::string& message, const char* funcName, const char* sourceFile, unsigned int lineNum)
{
	m_tagCriticalSection.Lock();
	auto findIt = m_tags.find(tag);
	if (findIt != m_tags.end())
	{
		m_tagCriticalSection.Unlock();

		string buffer;
		GetOutputBuffer(buffer, tag, message, funcName, sourceFile, lineNum);
		OutputFinalBufferToLogs(buffer, findIt->second);
	}
	else
	{
		m_tagCriticalSection.Unlock();
	}
}

//-------------------------------------------------------------------
//	Sets display flags
//-------------------------------------------------------------------
void LogManager::SetDisplayFlags(const std::string & tag, unsigned char flags)
{
	m_tagCriticalSection.Lock();
	if(flags != 0)
	{
		auto findIt = m_tags.find(tag);
		if (findIt == m_tags.end())
			m_tags.insert(std::make_pair(tag, flags));
		else
			findIt->second = flags;
	}
	else
	{
		m_tags.erase(tag);
	}
	m_tagCriticalSection.Unlock();
}

//-------------------------------------------------------------------
//	Adds an error messenger to the list
//-------------------------------------------------------------------
void LogManager::AddErrorMessenger(Logger::ErrorMessenger* pMessenger)
{
	m_messengerCriticalSection.Lock();
	m_errorMessengers.push_back(pMessenger);
	m_messengerCriticalSection.Unlock();
}

//-------------------------------------------------------------------
//	Helper for ErrorMessenger
//-------------------------------------------------------------------
LogManager::ErrorDialogResult LogManager::Error(const std::string& errorMessage, bool isFatal, const char* funcName, const char* sourceFile, unsigned int lineNum)
{
	string tag = ((isFatal) ? ("FATAL") : ("ERROR"));

	// final output buffer
	string buffer;
	GetOutputBuffer(buffer, tag, errorMessage, funcName, sourceFile, lineNum);

	m_tagCriticalSection.Lock();
	auto findIt = m_tags.find(tag);
	if(findIt != m_tags.end())
		OutputFinalBufferToLogs(buffer, findIt->second);
	m_tagCriticalSection.Unlock();

	// show the dialog box
	int result = MessageBoxA(NULL, buffer.c_str(), tag.c_str(), MB_ABORTRETRYIGNORE | MB_ICONERROR | MB_DEFBUTTON3);

	switch (result)
	{
	case IDIGNORE:
		return LogManager::LOGMGR_ERROR_IGNORE;
	case IDABORT:
		return LogManager::LOGMGR_ERROR_ABORT;
	case IDRETRY:
		return LogManager::LOGMGR_ERROR_RETRY;
	default:
		return LogManager::LOGMGR_ERROR_RETRY;
	}
}

void LogManager::OutputFinalBufferToLogs(const std::string & finalBuffer, unsigned char flags)
{
	if ((flags & LOGFLAG_WRITE_TO_LOG_FILE) > 0) // log file
		WriteToLogFile(finalBuffer);
	if ((flags & LOGFLAG_WRITE_TO_DEBUGGER) > 0) // debugger
		OutputDebugStringA(finalBuffer.c_str());
}

void LogManager::WriteToLogFile(const std::string & data)
{
	FILE* pLogFile = NULL;
	fopen_s(&pLogFile, ERRORLOG_FILENAME, "a+");
	if(!pLogFile)
		return;

	fprintf_s(pLogFile, data.c_str());
	fclose(pLogFile);
}

//-------------------------------------------------------------------
//	Fills outOutputBuffer with error string
//-------------------------------------------------------------------
void LogManager::GetOutputBuffer(std::string& outOutputBuffer, const std::string& tag, const std::string& message, const char* funcName, const char* sourceFile, unsigned int lineNum)
{
	time_t rawtime;
	struct tm * timeinfo;

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );


	outOutputBuffer = ToStr(timeinfo->tm_hour) +":"+ToStr(timeinfo->tm_min)+":"+ToStr(timeinfo->tm_sec);
	if (!tag.empty())
		outOutputBuffer += " ["+tag+"] " + message;
	else
		outOutputBuffer = message;

	if(funcName != NULL)
	{
		outOutputBuffer += "\nFunction: ";
		outOutputBuffer += funcName; 
	}

	if(sourceFile != NULL)
	{
		outOutputBuffer += "\n";
		outOutputBuffer += sourceFile;
	}

	if(lineNum != NULL)
	{
		outOutputBuffer += "\nLine: ";
		char lineNumBuffer[11];
		memset(lineNumBuffer, 0, sizeof(char));
		outOutputBuffer += _itoa_s(lineNum, lineNumBuffer, 10);
	}

	outOutputBuffer += "\n";
}

Logger::ErrorMessenger::ErrorMessenger(void)
{
	g_LogManager->AddErrorMessenger(this);
	m_Enabled = true;
}

void Logger::ErrorMessenger::Show(const std::string & errorMessage, bool isFatal, const char* funcName, const char* sourceFile, unsigned int lineNum)
{
	if(m_Enabled)
	{
		if(g_LogManager->Error(errorMessage, isFatal, funcName, sourceFile, lineNum) == LogManager::LOGMGR_ERROR_IGNORE)
			m_Enabled = false;
	}
}

namespace Logger
{
	void Init(const char* loggingConfigFilename)
	{
		if(!g_LogManager)
		{
			g_LogManager = new LogManager;
			g_LogManager->Init(loggingConfigFilename);
		}

		NAIA_INFO("Logger initialized");
	}

	void Destroy(void)
	{
		delete g_LogManager;
		g_LogManager = NULL;
	}

	void Log(const std::string & tag, const std::string& message, const char* funcName, const char* sourceFile, unsigned int lineNum)
	{
		NAIA_ASSERT(g_LogManager);
		g_LogManager->Log(tag,message,funcName,sourceFile,lineNum);
	}

	void SetDisplayFlags(const std::string & tag, unsigned char flags)
	{
		NAIA_ASSERT(g_LogManager);
		g_LogManager->SetDisplayFlags(tag,flags);
	}
}