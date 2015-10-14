@echo off

set debug=1
set fast=0
set release=0

set commonCompilerFlags= -nologo -Gm- -GR- -EHa- -fp:fast -Oi -WX -W4 -DWIN32=1
set commonLinkerFlags= -incremental:no -opt:ref
set win32Libraries= user32.lib gdi32.lib winmm.lib opengl32.lib

set ignoredWarnings= -wd4505 -wd4201 -wd4100 -wd4189
set debugCompilerFlags= %commonCompilerFlags% -MTd -Od %ignoredWarnings% -DINTERNAL=1 -DSLOW=1 -FC -Z7
set fastCompilerFlags= %commonCompilerFlags% -O2 %ignoredWarnings% -DINTERNAL=1 -DSLOW=0 -FC -Z7
set releaseCompilerFlags= %commonCompilerFlags% -O2 -DINTERNAL=0 -DSLOW=0

IF %debug%==1 (set compilerFlags= %debugCompilerFlags%)
IF %fast%==1 (set compilerFlags= %fastCompilerFlags%)
IF %release%==1 (set compilerFlags= %releaseCompilerFlags%)

IF NOT EXIST bin mkdir bin
pushd bin

cl %compilerFlags% /I w:\slib ..\src\win32.cpp /link %commonLinkerFlags% %win32Libraries%

popd
