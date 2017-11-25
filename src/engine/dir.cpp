/***************************************************************************
 *   Copyright (C) 2009 by Andrey Afletdinov <fheroes2@gmail.com>          *
 *                                                                         *
 *   Part of the Free Heroes2 Engine:                                      *
 *   http://sourceforge.net/projects/fheroes2                              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef WIN32
#include <dirent.h>
#else 
#include <Windows.h>
#endif

#include "system.h"
#include "tools.h"
#include "dir.h"


wstring s2ws(const std::string& str)
{
	return std::wstring(str.begin(), str.end());
}

string ws2s(const std::wstring& wstr)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.to_bytes(wstr);
}
void ListFiles::Append(const ListFiles &list)
{
    insert(end(), list.begin(), list.end());
}

void ListFiles::ReadDir(const std::string &path, const std::string &filter, bool sensitive)
{
#ifdef WIN32
	HANDLE hFind;
	WIN32_FIND_DATA FindFileData;
	auto wPath = s2ws(path);
	if ((hFind = FindFirstFile(wPath.c_str(), &FindFileData)) != INVALID_HANDLE_VALUE) {
		do {

			wstring wFileName = FindFileData.cFileName;
			string fileName = ws2s(wFileName);
			push_back(fileName);
		} while (FindNextFile(hFind, &FindFileData));
		FindClose(hFind);
	}
#else

	// read directory
	DIR *dp = opendir(path.c_str());

	if (dp)
	{
		struct dirent *ep;
		while (nullptr != (ep = readdir(dp)))
		{
			const std::string fullname = System::ConcatePath(path, ep->d_name);

			// if not regular file
			if (!System::IsFile(fullname)) continue;

			if (filter.size())
			{
				std::string filename(ep->d_name);

				if (sensitive)
				{
					if (std::string::npos == filename.find(filter)) continue;
				}
				else
				{
					if (std::string::npos == StringLower(filename).find(StringLower(filter))) continue;
				}
			}

			push_back(fullname);
		}
		closedir(dp);
	
#endif

}

void ListDirs::Append(const std::list<std::string> &dirs)
{
    insert(end(), dirs.begin(), dirs.end());
}
