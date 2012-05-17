#!/bin/sh
MAJOR=6
MINOR=0
REVISION1=32
REVISION2=3

rm -Rf resources>/dev/null
mkdir resources>/dev/null

echo "1 VERSIONINFO">>resources/fce.rc
echo "FILEVERSION ""$MAJOR"",""$MINOR"",""$REVISION1"",""$REVISION2">>resources/fce.rc
echo "PRODUCTVERSION ""$MAJOR"",""$MINOR"",""$REVISION1"",""$REVISION2">>resources/fce.rc
echo "FILEOS 0x40004
FILETYPE 0x2
{
	BLOCK \"StringFileInfo\"
	{
		BLOCK \"040904B0\"
		{
			VALUE \"CompanyName\", \"XiaoJSoft Studio\"
			VALUE \"FileDescription\", \"File-based shell for BCE\"
			VALUE \"FileVersion\", \"""$MAJOR"".""$MINOR"".""$REVISION1"".""$REVISION2"" (shell)\"
			VALUE \"InternalName\", \"fce.exe\"
			VALUE \"LegalCopyright\", \"(C) XiaoJSoft Studio. All rights reserved.\"
			VALUE \"OriginalFilename\", \"fce.exe\"
			VALUE \"ProductName\", \"XiaoJSoft Chemical Equation Balancer\"
			VALUE \"ProductVersion\", \"""$MAJOR"".""$MINOR"".""$REVISION1"".""$REVISION2""\"
		}
	}

	BLOCK \"VarFileInfo\"
	{
		VALUE \"Translation\", 0x0409, 0x04B0
	}
}">>resources/fce.rc

exit 0

