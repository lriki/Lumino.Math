#pragma once

// ソースファイルと同じフォルダにあるファイルまでのパスを取得する
#define LOCAL_RESOURCE(fileName) Lumino_Sample_GetLocalResource(__FILE__, fileName)

inline const char* Lumino_Sample_GetLocalResource(const char* sourceFilePath, const char* fileName)
{
	static char output[_MAX_PATH];
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char name[_MAX_FNAME];
	char ext[_MAX_EXT];
	_splitpath_s(sourceFilePath, drive, _MAX_DRIVE, dir, _MAX_DIR, name, _MAX_FNAME, ext, _MAX_EXT);
	sprintf_s(output, _MAX_PATH, "%s%s%s", drive, dir, fileName);
	return output;
}
