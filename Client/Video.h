#pragma once

#include "Res.h"

class Video : public Res
{
public:
	Video();
	virtual ~Video() override;

	bool Load(fs::path fullPath);
	void Play(bool loop = false);
	void Stop(bool reset = false);
	void CheckEnd();

	bool IsPlay() { return _isPlay; }
	bool IsEnd() { return _isEnd; }

	wstring getFullPath() { return _fullPath.c_str(); }
	wstring GetAlias() { return _alias; }

	static void CheckMci(MCIERROR err, const WCHAR* what);
	
private:
	virtual void LoadFile(const wstring& path) {}
	virtual void SaveFile(const wstring& path) {}

private:
	fs::path _fullPath; // 영상 파일 전체 경로
	wstring  _alias; // MCI에서 부를 이름 (ResMgr 키를 그대로)
	HWND     _videoWnd = nullptr; // 영상용 자식 창
	HWND	 _parent = nullptr;
	RECT	 _rc = {};
	bool	 _isEnd = false;
	bool	 _isPlay = false;
};

