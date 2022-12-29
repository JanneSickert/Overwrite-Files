cls

SET OVERWRITE_FILES_SOURCE=C:\Overwrite-Files\app\src\test\text\txt
SET PATH_FILE_OVERWRITE_FILES=C:\Overwrite-Files\Paths.txt

SET AFA=C:\Overwrite-Files\app\src\test\text\txt1
SET AFB=C:\Overwrite-Files\app\src\test\text\txt2
SET AFC=C:\Overwrite-Files\app\src\test\text\txt3

COPY %OVERWRITE_FILES_SOURCE% %AFA%
COPY %OVERWRITE_FILES_SOURCE% %AFB%
COPY %OVERWRITE_FILES_SOURCE% %AFC%

echo %AFA% > %PATH_FILE_OVERWRITE_FILES%
echo %AFB% >> %PATH_FILE_OVERWRITE_FILES%
echo %AFC% >> %PATH_FILE_OVERWRITE_FILES%

cd..
cd..
cd..
cd..

echo Press Enter to build and execute.
pause

gradle clean build
./app/build/exe/main/debug/app

echo Press Enter to restart the test.
pause

START Test.bat