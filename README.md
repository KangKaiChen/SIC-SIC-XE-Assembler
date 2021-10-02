# SIC-SIC-XE-Assembler




1.	使用說明:
輸入要讀的Input檔 (Ex:input2…)，程式結束時會有Output_+ Input檔名+.txt的檔案出現即為輸出檔。

2. 說明你的程式設計:
輸入一個檔案，讀檔方式為一次讀一個字元後，直到讀到換行\n就把整個字串放在vector中等待切token, 進入到切token 換一整行讀近來切如果讀到空白或tab 就繼續看他是屬於table 3 還是5還是6 挼果是delimeter就看他C還是X 是table 4.5.6.7 等等前面做的事大致與作業1的方式一樣,token切好就創造一個opcode table裡面存放以下資訊	
string instrName = "" ; //指令名稱   int format ; //指令型別  string opcode = "no"; //指令opcode code
之後進入本次程式主要的部份轉譯機器碼, 首先要先判斷是否有語法錯誤

1 判斷指令開頭是否正確
2. 斷instruction後是否還有東西若沒有，直接結束 
    3.判斷operand的開頭或結尾是否是','
4. 判斷此token使否不是最後一個且也不是','
5. 若operand的開頭或結尾是'.'，syntax error  
6.若lable後沒有東西syntax error

如果非以上狀況之後就印出資訊,做pass1 設定起始位址, 判斷指令是否有lable是否存在cross reference若已存在duplicate symbol
沒有判斷EQU後面接得是LABEL還是integer, 若沒有syntax error && 指令'EQU，看指令是屬於type1.2.3.4 .pseudo instruction將它翻成machinecode 結束pass1 pass2 找指令未被翻譯的將它翻成machinecode  重複執行pass1 若為execution instruction 一樣去判斷指令屬於type1.2.3.4 pseudo instruction 將結果output出來
SIC 與 SIC XE 的差別在於 SIC XE要比XIC多了nixbpe
![image](https://user-images.githubusercontent.com/55120331/135712382-c7a4800a-0a85-4f0c-8734-7599a7b139f1.png)
![image](https://user-images.githubusercontent.com/55120331/135712386-8c04c34c-8bc7-4e66-96ff-d5214b5098f0.png)
