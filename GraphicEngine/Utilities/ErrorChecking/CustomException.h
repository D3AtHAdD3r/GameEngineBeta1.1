#pragma once
#include<Windows.h>
#include<exception>
#include<string>
#include<vector>
#include<GraphicEngine/Utilities/ErrorChecking/DxgiInfoManager.h>


//throw D3D exceptions
#define GFX_EXCEPT_NOINFO(hr) GraphicsHrException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_NOINFO(hrcall) if( FAILED( hr = (hrcall) ) ) throw GraphicsHrException( __LINE__,__FILE__,hr )

#ifndef NDEBUG
#define GFX_EXCEPT(hr) GraphicsHrException( __LINE__,__FILE__,(hr),infoManager.GetMessages() )
#define GFX_THROW_INFO(hrcall) infoManager.Set(); if( FAILED( hr = (hrcall) ) ) throw GFX_EXCEPT(hr)   //wrap all d3dinterface functions that return hr in this macro

#else
#define GFX_EXCEPT(hr) Graphics::HrException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
#endif


//throw windows exceptions
#define WND_EXCEPT( hr ) WindowsHrException( __LINE__,__FILE__,(hr) )
#define WND_LAST_EXCEPT() WindowsHrException( __LINE__,__FILE__,GetLastError() )
#define WND_NOGFX_EXCEPT() WindowsHrException( __LINE__,__FILE__ )


#define NORMAL_EXCEPT(str) NormalException(__LINE__,__FILE__, str);



//DxgiInfoManager - gets error string from output window
static DxgiInfoManager infoManager;

class CustomException : public std::exception
{
public:
	CustomException(int line, const char* file) noexcept;
	const char* what() const noexcept override;
	virtual const char* GetType() const noexcept;
	int GetLine() const noexcept;
	const std::string& GetFile() const noexcept;
	std::string GetOriginString() const noexcept;

private:
	int line;
	std::string file;
protected:
	mutable std::string whatBuffer;
};



class ExceptionX : public CustomException
{
	using CustomException::CustomException;
public:
	static std::string TranslateErrorCode(HRESULT hr) noexcept;
};


class WindowsHrException : public ExceptionX
{
public:
	WindowsHrException(int line, const char* file, HRESULT hr) noexcept;
	virtual const char* what() const noexcept override;
	virtual const char* GetType() const noexcept override;
	HRESULT GetErrorCode() const noexcept;
	std::string GetErrorDescription() const noexcept;
private:
	HRESULT hr;
};



class GraphicsHrException : public ExceptionX
{
public:
	GraphicsHrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs = {}) noexcept;
	const char* what() const noexcept override;
	const char* GetType() const noexcept override;
	HRESULT GetErrorCode() const noexcept;
	std::string GetErrorString() const noexcept;
	std::string GetErrorDescription() const noexcept;
	std::string GetErrorInfo() const noexcept;
private:
	HRESULT hr;
	std::string info;
};



class NormalException : public ExceptionX
{
public:
	NormalException(int line, const char* file, std::string msg) noexcept;
	virtual const char* what() const noexcept override;
	virtual const char* GetType() const noexcept override;
private:
	std::string message;
};