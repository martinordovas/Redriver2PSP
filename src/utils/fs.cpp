#include "fs.h"

#include <string.h>
#include <strings.h>
#include <assert.h>
#include <cstdlib>
#include <cctype>

#ifndef strcasecmp
inline int psp_strcasecmp(const char* s1, const char* s2) {
    while (*s1 && *s2 && (tolower(*(unsigned char*)s1) == tolower(*(unsigned char*)s2))) {
        s1++; s2++;
    }
    return tolower(*(unsigned char*)s1) - tolower(*(unsigned char*)s2);
}
#define strcasecmp psp_strcasecmp
#endif

#ifdef _WIN32

#include <direct.h>
#include <wtypes.h>

#define HOME_ENV "USERPROFILE"

void FS_FixPathSlashes(char* pathbuff)
{
    while (*pathbuff)
    {
        if (*pathbuff == '/') // make windows-style path
            *pathbuff = '\\';
        pathbuff++;
    }
}

#elif defined (__PSP__) || defined (PSP)

#include <sys/stat.h>
#include <dirent.h>
#include <vector>
#include <string>

void FS_FixPathSlashes(char* pathbuff)
{
    while (*pathbuff)
    {
        if (*pathbuff == '\\') // make unix-style path
            *pathbuff = '/';
        pathbuff++;
    }
}

typedef struct {
    int gl_pathc;
    char** gl_pathv;
} glob_t;

inline int glob(const char* pattern, int flags, int (*errfunc)(const char*, int), glob_t* pglob) {
    std::string pat(pattern);
    size_t slash = pat.find_last_of("/\\");
    std::string dir = (slash == std::string::npos) ? "." : pat.substr(0, slash);
    std::string ext = (slash == std::string::npos) ? pat : pat.substr(slash + 1);
    
    bool matchAll = (ext == "*" || ext == "*.*");
    std::string suffix = "";
    if (!matchAll) {
        size_t star = ext.find('*');
        if (star != std::string::npos) {
            suffix = ext.substr(star + 1);
        } else {
            suffix = ext;
        }
    }

    DIR* d = opendir(dir.c_str());
    if (!d) return -1;

    std::vector<char*> paths;
    struct dirent* de;
    while ((de = readdir(d)) != nullptr) {
        std::string name(de->d_name);
        if (name == "." || name == "..") continue;
        
        bool match = matchAll;
        if (!match && !suffix.empty()) {
            if (name.length() >= suffix.length() && 
                strcasecmp(name.c_str() + name.length() - suffix.length(), suffix.c_str()) == 0) {
                match = true;
            }
        }
        
        if (match) {
            std::string fullpath = dir + "/" + name;
            char* dup = (char*)malloc(fullpath.length() + 1);
            strcpy(dup, fullpath.c_str());
            paths.push_back(dup);
        }
    }
    closedir(d);

    pglob->gl_pathc = paths.size();
    if (pglob->gl_pathc > 0) {
        pglob->gl_pathv = (char**)malloc(sizeof(char*) * pglob->gl_pathc);
        for (size_t i = 0; i < paths.size(); i++) {
            pglob->gl_pathv[i] = paths[i];
        }
    } else {
        pglob->gl_pathv = nullptr;
    }
    return 0;
}

inline void globfree(glob_t* pglob) {
    if (pglob && pglob->gl_pathv) {
        for (int i = 0; i < pglob->gl_pathc; i++) {
            free(pglob->gl_pathv[i]);
        }
        free(pglob->gl_pathv);
        pglob->gl_pathv = nullptr;
        pglob->gl_pathc = 0;
    }
}

#elif defined (__unix__)

#include <sys/stat.h>
#include <glob.h>		// glob(), globfree()
#include <malloc.h>

void FS_FixPathSlashes(char* pathbuff)
{
    while (*pathbuff)
    {
        if (*pathbuff == '\\') // make unix-style path
            *pathbuff = '/';
        pathbuff++;
    }
}
#endif

struct FS_FINDDATA
{
    char* wildcard;

#ifdef _WIN32
    WIN32_FIND_DATAA	wfd;
    HANDLE				fileHandle;
#else
    glob_t				gl;
    int					index;
    int					pathlen;
#endif // _WIN32
};

char* strreplace(char** str, char* dst, char* replace)
{
	char* start, * tmp;
	int n;

	if ((start = strstr(*str, dst)) == 0) 
		return(0);

	n = (start - *str) + strlen(start + strlen(dst)) + strlen(replace) + 1;

	tmp = (char*)malloc(n);

	// left part
	strncpy(tmp, *str, start - *str);

	// replace (mid) part
	strcpy(tmp + (start - *str), replace);

	// right part
	strcat(tmp, start + strlen(dst));

	free(*str);

	*str = tmp;

	return tmp;
}

// opens directory for search props
const char* FS_FindFirst(const char* wildcard, FS_FINDDATA** findData)
{
	assert(findData != nullptr);

	if (findData == nullptr)
		return nullptr;

	FS_FINDDATA* newFind = new FS_FINDDATA;
	*findData = newFind;

	newFind->wildcard = (char*)malloc(strlen(wildcard) + 1);
	strcpy(newFind->wildcard, wildcard);
	FS_FixPathSlashes(newFind->wildcard);

#ifndef _WIN32
	strreplace(&newFind->wildcard, "*.*", "*");
#endif

#ifdef _WIN32
	newFind->fileHandle = ::FindFirstFileA(newFind->wildcard, &newFind->wfd);

	if (newFind->fileHandle != INVALID_HANDLE_VALUE)
		return newFind->wfd.cFileName;
#else // POSIX
	newFind->index = -1;

	if (glob(newFind->wildcard, 0, NULL, &newFind->gl) == 0 && newFind->gl.gl_pathc > 0)
	{
		newFind->pathlen = strchr(newFind->wildcard, '.') - newFind->wildcard - 1;
		newFind->index = 0;
		return newFind->gl.gl_pathv[newFind->index] + newFind->pathlen;
	}
#endif // _WIN32

	// delete if no luck
	FS_FindClose(newFind);
	*findData = NULL;

	return nullptr;
}

const char* FS_FindNext(FS_FINDDATA* findData)
{
	if (!findData)
		return nullptr;

#ifdef _WIN32
	if (!::FindNextFileA(findData->fileHandle, &findData->wfd))
		return nullptr;
#else
	findData->index++;
	if (findData->index < 0 || findData->index >= findData->gl.gl_pathc)
		return nullptr;
#endif // _WIN32

#ifdef _WIN32
	return findData->wfd.cFileName;
#else
	return findData->gl.gl_pathv[findData->index] + findData->pathlen;
#endif // _WIN32
}

void FS_FindClose(FS_FINDDATA* findData)
{
	if (!findData)
		return;

#ifdef _WIN32
	if(findData->fileHandle = INVALID_HANDLE_VALUE)
		FindClose(findData->fileHandle);
#else
	if (findData->index >= 0)
		globfree(&findData->gl);
#endif // _WIN32
	delete findData;
}

bool FS_FindIsDirectory(FS_FINDDATA* findData)
{
	if (!findData)
		return false;

#ifdef _WIN32
	return (findData->wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
#else
	struct stat st;

	if (stat(findData->gl.gl_pathv[findData->index], &st) == 0)
	{
		return (st.st_mode & S_IFDIR) > 0;
	}

	return false;
#endif // _WIN32
}