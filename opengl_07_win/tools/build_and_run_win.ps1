Push-Location $PSScriptRoot/../
.\tools\build_win.ps1; 
if ($?) { 
    Push-Location .\dist\
    .\main.exe
    Pop-Location
}