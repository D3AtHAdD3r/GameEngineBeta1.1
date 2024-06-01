#include "CustomException.h"
#include<sstream>



CustomException::CustomException(int line, const char* file) noexcept
	:
	line(line),
	file(file)
{}


const char* CustomException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< GetOriginString();

	whatBuffer = oss.str();
	return whatBuffer.c_str();
}


const char* CustomException::GetType() const noexcept
{
	return "Custom Exception";
}


int CustomException::GetLine() const noexcept
{
	return line;
}

const std::string& CustomException::GetFile() const noexcept
{
	return file;
}

//returns string with file name and line number
std::string CustomException::GetOriginString() const noexcept
{
	std::ostringstream oss;
	oss << "[File]" << file << std::endl
		<< "[Line]" << line;

	return oss.str();
}


//--------------------------------------------------------------------------------//
//Translates error code into string
std::string ExceptionX::TranslateErrorCode(HRESULT hr) noexcept
{
	char* pMsgBuf = nullptr;

	// windows will allocate memory for err string and make our pointer point to it
	const DWORD nMsgLen = FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr
	);

	// 0 string length returned indicates a failure
	if (nMsgLen == 0)
	{
		return "Unidentified error code";
	}

	// copy error string from windows-allocated buffer to std::string
	std::string errorString = pMsgBuf;

	// free windows buffer
	LocalFree(pMsgBuf);
	return errorString;
}



WindowsHrException::WindowsHrException(int line, const char* file, HRESULT hr) noexcept
	:
	ExceptionX(line, file),
	hr(hr)
{}

const char* WindowsHrException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "[Description] " << GetErrorDescription() << std::endl
		<< GetOriginString();

	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* WindowsHrException::GetType() const noexcept
{
	return "Window HRESULT Exception";
}

HRESULT WindowsHrException::GetErrorCode() const noexcept
{
	return hr;
}

std::string WindowsHrException::GetErrorDescription() const noexcept
{
	return ExceptionX::TranslateErrorCode(hr);
}

//-----------------------------------------------------------------------//

GraphicsHrException::GraphicsHrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs) noexcept
	:
	ExceptionX(line, file),
	hr(hr)
{
	// join all info messages with newlines into single string
	for (const auto& m : infoMsgs)
	{
		info += m;
		info.push_back('\n');
	}

	// remove final newline if exists
	if (!info.empty())
	{
		info.pop_back();
	}
}

const char* GraphicsHrException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "[Error String] " << GetErrorString() << std::endl
		<< "[Description] " << GetErrorDescription() << std::endl;

	if (!info.empty())
	{
		oss << "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
	}

	oss << GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* GraphicsHrException::GetType() const noexcept
{
	return "Graphics Exception";
}

HRESULT GraphicsHrException::GetErrorCode() const noexcept
{
	return hr;
}

std::string GraphicsHrException::GetErrorString() const noexcept
{
	return "Some Error:";
}

std::string GraphicsHrException::GetErrorDescription() const noexcept
{
	char* pMsgBuf = nullptr;
	// windows will allocate memory for err string and make our pointer point to it
	const DWORD nMsgLen = FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr
	);

	// 0 string length returned indicates a failure
	if (nMsgLen == 0)
	{
		return "Unidentified error code";
	}

	// copy error string from windows-allocated buffer to std::string
	std::string errorString = pMsgBuf;

	// free windows buffer
	LocalFree(pMsgBuf);
	return errorString;
}

std::string GraphicsHrException::GetErrorInfo() const noexcept
{
	return info;
}

NormalException::NormalException(int line, const char* file, std::string msg) noexcept
	:
	ExceptionX(line, file),
	message(msg)
{}

const char* NormalException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< GetOriginString() << std::endl
		<< message << std::endl;

	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* NormalException::GetType() const noexcept
{
	return "Normal Exception";
}
