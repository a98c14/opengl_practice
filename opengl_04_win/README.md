## Build & Run
```ps1
.\tools\build_win.ps1; if($?) { .\dist\main.exe }
```

## Building GLAD
```ps1
# run at workspace root
mkdir ./temp/ && pushd ./temp/ && cl /LD ../glad/src/gl.c -I../glad/include && popd;
```