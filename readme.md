## D2DWindow-dll
UI controlをDirect2Dで作る  
UI controlとはbutton,listbox,textbox  
Frameworkとなりうるレベルを目指す
コードが煩雑にならないようにDLL化する
すべての辻褄が合致して動作するかを確認する
作り易さを優先する

## build condition
VisualStudio 2022, C++, (not xaml, not c++/cx)
use Project1/Project1.sln.

## D2DUI_1
DLL project
## D2DUI_lib
lib project (すべてをDLL側に置きたいが、開発のしやすさを配慮して一部置いておく)
## Project1
easy sample project
## Project2
出来損ないのファイルマネージャー
## Project3
テスト
## Project4
MDIフレーム

