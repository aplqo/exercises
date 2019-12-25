@echo off
set /p di="Enter destintion diectory "
set /p ty="Enter configure type (1: devc++ 2: MSVC/MinGW) "

mkdir %di%
mkdir "%di%\.vscode"
mkdir "%di%\debug_tools"
mkdir "%di%\scripts"
if %ty% EQU 1 copy ".\tools\c_cpp_properties.json" "%di%\.vscode\"
if %ty% EQU 2 copy ".\tools\vscode\*" "%di%\.vscode\"
copy ".\tools\.clang-format" "%di%"
copy ".\tools\debug_tools\*" "%di%\debug_tools\"
copy ".\tools\scripts\*" "%di%\scripts\"