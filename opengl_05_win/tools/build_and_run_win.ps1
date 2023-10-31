Push-Location $PSScriptRoot/../
.\tools\build_win.ps1; 
if ($?) { 
    Start-Process -FilePath .\dist\main.exe -WorkingDirectory .\dist\
}