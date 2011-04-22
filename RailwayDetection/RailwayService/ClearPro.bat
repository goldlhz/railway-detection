attrib -R -A -S -H *.ncb /S /D
attrib -R -A -S -H *.aps /S /D
attrib -R -A -S -H *.user /S /D
attrib -R -A -S -H *.cd /S /D
attrib -R -A -S -H *.exp /S /D
attrib -R -A -S -H *.~ml /S /D
attrib -R -A -S -H *.pdb /S /D
del /S /Q *.ncb
del /S /Q *.suo
del /S /Q *.aps
del /S /Q *.user
del /S /Q *.cd
del /S /Q *.exp
del /S /Q *.~ml
del /S /Q *.pdb
for /r . %%a in (.) do @if exist "%%a\Debug" rd /s /q "%%a\Debug"
for /r . %%a in (.) do @if exist "%%a\Release" rd /s /q "%%a\Release"
for /r . %%a in (.) do @if exist "%%a\.svn" rd /s /q "%%a\.svn"    
pause
