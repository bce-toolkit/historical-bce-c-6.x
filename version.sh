#!/bin/sh
MAJOR=6
MINOR=2
REVISION1=26
REVISION2=1

rm -Rf resources>/dev/null
mkdir resources>/dev/null

echo "1 VERSIONINFO">>resources/version.rc
echo "FILEVERSION ""$MAJOR"",""$MINOR"",""$REVISION1"",""$REVISION2">>resources/version.rc
echo "PRODUCTVERSION ""$MAJOR"",""$MINOR"",""$REVISION1"",""$REVISION2">>resources/version.rc
echo "FILEOS 0x40004
FILETYPE 0x2
{
	BLOCK \"StringFileInfo\"
	{
		BLOCK \"040904B0\"
		{
			VALUE \"CompanyName\", \"XiaoJSoft Studio.\"
			VALUE \"FileDescription\", \"BCE Shell\"
			VALUE \"FileVersion\", \"""$MAJOR"".""$MINOR"".""$REVISION1"".""$REVISION2"" (kernel-only)\"
			VALUE \"InternalName\", \"bce.exe\"
			VALUE \"LegalCopyright\", \"(C) XiaoJSoft Studio. All rights reserved.\"
			VALUE \"OriginalFilename\", \"bce.exe\"
			VALUE \"ProductName\", \"XiaoJSoft Chemical Equation Balancer\"
			VALUE \"ProductVersion\", \"""$MAJOR"".""$MINOR"".""$REVISION1"".""$REVISION2""\"
		}
	}

	BLOCK \"VarFileInfo\"
	{
		VALUE \"Translation\", 0x0409, 0x04B0
	}
}">>resources/version.rc

exit 0

