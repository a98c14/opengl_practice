Push-Location $PSScriptRoot/../
.\tools\build_win.ps1; 
if ($?) { 
    .\dist\main.exe 
}