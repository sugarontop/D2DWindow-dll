## D2DWindow-dll
UI controlをDirect2Dで作る  
UI controlとはbutton,listbox,textbox  
コードが煩雑にならないようにDLL化する
すべての辻褄が合致して動作するかを確認する
作り易さを優先する

## build condition
VisualStudio 2019, C++, (not xaml, not c++/cx)

## D2DUI_1
dll project
## D2DUI_lib
lib project
## Project1
main, window project
## Project2
Project1にプラスアルファ
## Project3
さらにプラスアルファ

