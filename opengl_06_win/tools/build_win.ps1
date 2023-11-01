$app_name = "main";
$output_path = ".\dist";
$source_file_path = ".\src\main.c";

$compile_args = @(
    "/Zi",
    "/nologo",
    "/Fd:""$output_path\$app_name.pdb""",
    "/Fo:""$output_path""\",
    "/I"".\include""",
    "/I"".\src\opus""",
    "/I"".\src\fonts""",
    "/DSTB_IMAGE_IMPLEMENTATION",
    ".\lib\glfw3dll.lib",
    ".\lib\glfw3.lib",
    ".\lib\gl.obj",
    "opengl32.lib");

$link_args = @(
    "/debug:full",
    "/out:""$output_path\$app_name.exe""",
    "/pdb:""$output_path\$app_name.pdb""");

Push-Location $PSScriptRoot/../
New-Item -ItemType Directory -Force -Path $output_path;
cl $compile_args $source_file_path /link $link_args;
Pop-Location