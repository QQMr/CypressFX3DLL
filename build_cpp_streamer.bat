echo off
SET BUILDENV=C:\WINDOWS\Microsoft.NET\Framework\v3.5\

echo 
echo ******************************************************
echo ***  Building Streamer sample application          ***   
echo "*****************************************************
echo


echo 
echo ****Streamer build started for Debug Win32 config...
echo

%BUILDENV%MSBuild.exe Streamer.vcproj /t:Rebuild /p:Configuration=Debug /p:Platform=Win32 >>Streamer_Debug_win32.log
if %ERRORLEVEL%==0 goto success 
if not %ERRORLEVEL%==0 goto fail 

:fail
echo ***Build failed for detail check Streamer_Debug_win32.log config. check logs.***
goto done

:success
echo
echo ****Success
echo

echo
echo ****Streamer build started for Release Win32 config...
echo

%BUILDENV%MSBuild.exe Streamer.vcproj /t:Rebuild /p:Configuration=Release /p:Platform=Win32 >>Streamer_Release_win32.log
if %ERRORLEVEL%==0 goto success 
if not %ERRORLEVEL%==0 goto fail 

:fail
echo ***Build failed for detail check  Streamer_Release_win32.log config. check logs.***
goto done

:success
echo
echo ****Success
echo

echo
echo ****Streamer build started for Debug x64 config...
echo

%BUILDENV%MSBuild.exe Streamer.vcproj /t:Rebuild /p:Configuration=Debug /p:Platform=x64 >>Streamer_Debug_x64.log
if %ERRORLEVEL%==0 goto success 
if not %ERRORLEVEL%==0 goto fail 

:fail
echo ***Build failed for detail check Streamer_Debug_x64.log config. check logs.***
goto done

:success
echo
echo ****Success
echo

echo
echo ****Streamer build started for Release x64 config...
echo

%BUILDENV%MSBuild.exe Streamer.vcproj /t:Rebuild /p:Configuration=Release /p:Platform=x64 >>Streamer_Release_x64.log
if %ERRORLEVEL%==0 goto success 
if not %ERRORLEVEL%==0 goto fail 

:fail
echo ***Build failed for detail check Streamer_Release_x64.log config. check logs.***
goto done

:success
echo
echo ****Success
echo


:done
echo Streamer application build complete