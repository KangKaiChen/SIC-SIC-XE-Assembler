#include <iostream>
#include <fstream> // open, is_open, close, ignore
#include <string>
#include <string.h>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <stdio.h>
#include <iomanip>

using namespace std;
 
struct hashTable{
	int hashValue = -1 ; //hash value 
    string token = "" ; //token名稱 
};

struct outTable{
	int tableNum = -1 ; //token對應table編號 
	int tableNo = -1 ;	//token對應 table的位置 
};

struct OPTAB{
	string instrName = "" ; //指令名稱 
	int format ; //指令型別 
	string opcode = "no"; //指令opcode code 
};

struct OPERAND_GROUP{
	vector<outTable> outList ; //每個operand的所有token 
	int operand_type ; //型別 
};

struct TOKEN_GROUP{
	outTable label ; //statement label 
	outTable instr ; //statement 指令 
	bool isType4 = false ;	//指令是否是type4 
	vector<OPERAND_GROUP> operandList ;	//指令所有operand 
};

//symbol table
struct SYMTAB{
	int label_tableNo = -1; //label在table的位置 
	unsigned int address = 0 ; //label address 
};

struct LiTTAB{
	int hashValue = -1 ; //hash value 
	int tableNum = -1 ;
	int tableNo = -1 ;
	int type = -1 ;
	unsigned int address = 0 ; //literal address
	bool hasAddress = false ;
	string machine_code = "" ;
};

struct OUT_LITTAB{
	vector<LiTTAB> literalList ;
	unsigned int pc = 0 ;
};

struct BASE{
	unsigned int address = 0 ; //紀錄在base reg的addess 
};


struct tokenTable{
	string cmd = "" ; //statement
	vector<outTable> outList ; //token列表
	TOKEN_GROUP token_group ; //指令資訊 
	bool isOnlyComment = false ; //是否指令純註解 
	bool syntaxError = false ; //是否語意錯誤 
	bool isTranslate = false ; //是否翻譯完成 	
	string machine_code = "" ; //machine code ;
    unsigned int loc ; //指令位址 
};

#define TYPE1 1
#define TYPE2 2
#define TYPE3or4 34

#define MEMORY 0
#define REG 1
#define VALUE 2
#define IMME_VALUE 3
#define LITERAL 4
#define PSEUDO_VALUE 5
#define PSEUDO_STRING 6


class Token{
public:
	string inputFileName = "";  	
	string outFileName = "";
	ifstream inFile; // 讀檔 
    ofstream outFile; // 寫檔
    
    vector<string> table1 ;
  	vector<string> table2 ;
  	vector<string> table3 ;
  	vector<string> table4 ;		
  	
  	hashTable table5[100] ;
  	hashTable table6[100] ;
  	hashTable table7[100] ;	
  	  	
  	vector<tokenTable> tokenTableList ; //存每個指令資訊 
  	vector<OPTAB> OPTABlist ; //指令對應表 
  	vector<SYMTAB> crossRef ; //label、地址對照表 
  	vector<OUT_LITTAB> out_literal_list ;
  	LiTTAB LITTAB[100] ; //literal table
	BASE base_reg ; //base reg
	bool isPass2 = false ;
	int literal_index = 0 ;
  	
  	unsigned int LOCCTR = 0 ; //指向目前指令位址 
  	
  	void createTable(){
		OPTAB optab ;
		optab.instrName = "ADD";
		optab.format = TYPE3or4 ;
		optab.opcode = "18";
		OPTABlist.push_back(optab) ;
		
		optab.instrName = "ADDF";
		optab.format = TYPE3or4;
		optab.opcode = "58";
		OPTABlist.push_back(optab) ;
		
		optab.instrName = "ADDR";
		optab.format = TYPE2;
		optab.opcode = "90";
		OPTABlist.push_back(optab) ;
		
		optab.instrName = "AND";
		optab.format = TYPE3or4;
		optab.opcode = "40";
		OPTABlist.push_back(optab) ;
		
		optab.instrName = "CLEAR";
		optab.format = TYPE2;
		optab.opcode = "B4";
		OPTABlist.push_back(optab) ;

        optab.instrName = "COMP";
		optab.format = TYPE3or4;
		optab.opcode = "28";
		OPTABlist.push_back(optab) ;
		
		optab.instrName = "COMPF";
		optab.format = TYPE3or4;
		optab.opcode = "88";
		OPTABlist.push_back(optab) ;		
		
		optab.instrName = "COMPR";
		optab.format = TYPE2;
		optab.opcode = "A0";
		OPTABlist.push_back(optab) ;
		
		optab.instrName = "DIV";
		optab.format = TYPE3or4;
		optab.opcode = "24";
		OPTABlist.push_back(optab) ;
		
		optab.instrName = "DIVF";
		optab.format = TYPE3or4;
		optab.opcode = "64";
		OPTABlist.push_back(optab) ;
		
		optab.instrName = "DIVR";
		optab.format = TYPE2;
		optab.opcode = "9C";
		OPTABlist.push_back(optab) ;
		
		optab.instrName = "FIX";
		optab.format = TYPE1;
		optab.opcode = "C4";
		OPTABlist.push_back(optab) ;
		
		optab.instrName = "FLOAT";
		optab.format = TYPE1;
		optab.opcode = "CO";
		OPTABlist.push_back(optab) ;
		
		optab.instrName = "HIO";
		optab.format = TYPE1;
		optab.opcode = "F4";
		OPTABlist.push_back(optab) ;
		
		optab.instrName = "J";
		optab.format = TYPE3or4;
		optab.opcode = "3C";
		OPTABlist.push_back(optab) ;
		
		optab.instrName = "JEQ";
		optab.format = TYPE3or4;
		optab.opcode = "30";
		OPTABlist.push_back(optab) ;
		
		optab.instrName = "JGT";
		optab.format = TYPE3or4;
		optab.opcode = "34";
		OPTABlist.push_back(optab) ;
		
		optab.instrName = "JLT";
		optab.format = TYPE3or4;
		optab.opcode = "38";
		OPTABlist.push_back(optab) ;
		
		optab.instrName = "JSUB";
		optab.format = TYPE3or4;
		optab.opcode = "48";
		OPTABlist.push_back(optab) ;
		
		optab.instrName = "LDA";
		optab.format = TYPE3or4;
		optab.opcode = "00";
		OPTABlist.push_back(optab) ;
		
		optab.instrName = "LDB";
		optab.format = TYPE3or4;
		optab.opcode = "68";
		OPTABlist.push_back(optab) ;
		
		optab.instrName = "LDCH";
		optab.format = TYPE3or4;
		optab.opcode = "50";
		OPTABlist.push_back(optab) ;
		
		optab.instrName = "LDF";
		optab.format = TYPE3or4;
		optab.opcode = "70";
		OPTABlist.push_back(optab) ;
		
		optab.instrName = "LDL";
		optab.format = TYPE3or4;
		optab.opcode = "08";
		OPTABlist.push_back(optab) ;
		
		optab.instrName = "LDS";
		optab.format = TYPE3or4;
		optab.opcode = "6C";
		OPTABlist.push_back(optab) ;
		
		optab.instrName = "LDT";
		optab.format = TYPE3or4;
		optab.opcode = "74";
		OPTABlist.push_back(optab) ;
		
		optab.instrName = "LDX";
		optab.format = TYPE3or4;
		optab.opcode = "04";
		OPTABlist.push_back(optab) ;
		
		optab.instrName = "LPS";
		optab.format = TYPE3or4;
		optab.opcode = "D0";
		OPTABlist.push_back(optab) ;
		
		optab.instrName = "MUL";
		optab.format = TYPE3or4;
		optab.opcode = "20";
		OPTABlist.push_back(optab) ;
		
		optab.instrName = "MULF";
		optab.format = TYPE3or4;
		optab.opcode = "60";
		OPTABlist.push_back(optab) ;
		
		optab.instrName = "MULR";
		optab.format = TYPE2;
		optab.opcode = "98";
		OPTABlist.push_back(optab) ;
		
		optab.instrName = "NORM";
		optab.format = TYPE1;
		optab.opcode = "C8";
		OPTABlist.push_back(optab) ;
		
		optab.instrName = "OR";
		optab.format = TYPE3or4;
		optab.opcode = "44";
		OPTABlist.push_back(optab) ;
		
		optab.instrName = "RD";
		optab.format = TYPE3or4;
		optab.opcode = "D8";
		OPTABlist.push_back(optab) ;
		
		optab.instrName = "RMO";
		optab.format = TYPE2;
		optab.opcode = "AC";
		OPTABlist.push_back(optab) ;
		
		optab.instrName = "RSUB";
		optab.format = TYPE3or4;
		optab.opcode = "4C";
		OPTABlist.push_back(optab) ;
		
		optab.instrName = "SHIFTL";
		optab.format = TYPE2;
		optab.opcode = "A4";
		OPTABlist.push_back(optab) ;
		
		optab.instrName = "SHIFTR";
		optab.format = TYPE2;
		optab.opcode = "A8";
		OPTABlist.push_back(optab) ;
		
		optab.instrName = "SIO";
		optab.format = TYPE1;
		optab.opcode = "F0";
		OPTABlist.push_back(optab) ;
		
		optab.instrName = "SSK";
		optab.format = TYPE3or4;
		optab.opcode = "EC";
		OPTABlist.push_back(optab) ;
		
		optab.instrName = "STA";
		optab.format = TYPE3or4;
		optab.opcode = "0C";
		OPTABlist.push_back(optab) ;
		
		optab.instrName = "STB";
		optab.format = TYPE3or4;
		optab.opcode = "78";
		OPTABlist.push_back(optab) ;
		
		optab.instrName = "STCH";
		optab.format = TYPE3or4;
		optab.opcode = "54";
		OPTABlist.push_back(optab) ;
		
		optab.instrName = "STF";
		optab.format = TYPE3or4;
		optab.opcode = "80";
		OPTABlist.push_back(optab) ;
		
		optab.instrName = "STI";
		optab.format = TYPE3or4;
		optab.opcode = "D4";
		OPTABlist.push_back(optab) ;
		
		optab.instrName = "STL";
		optab.format = TYPE3or4;
		optab.opcode = "14";
		OPTABlist.push_back(optab) ;
		
		optab.instrName = "STS";
		optab.format = TYPE3or4;
		optab.opcode = "7C";
		OPTABlist.push_back(optab) ;
		
		optab.instrName = "STSW";
		optab.format = TYPE3or4;
		optab.opcode = "E8";
		OPTABlist.push_back(optab) ;
		
		optab.instrName = "STT";
		optab.format = TYPE3or4;
		optab.opcode = "84";
		OPTABlist.push_back(optab) ;
		
		optab.instrName = "STX";
		optab.format = TYPE3or4;
		optab.opcode = "10";
		OPTABlist.push_back(optab) ;
		
		optab.instrName = "SUB";
		optab.format = TYPE3or4;
		optab.opcode = "1C";
		OPTABlist.push_back(optab) ;
		
		optab.instrName = "SUBF";
		optab.format = TYPE3or4;
		optab.opcode = "5C";
		OPTABlist.push_back(optab) ;
		
		optab.instrName = "SUBR";
		optab.format = TYPE2;
		optab.opcode = "94";
		OPTABlist.push_back(optab) ;
		
		optab.instrName = "SVC";
		optab.format = TYPE2;
		optab.opcode = "B0";
		OPTABlist.push_back(optab) ;
		
		optab.instrName = "TD";
		optab.format = TYPE3or4;
		optab.opcode = "E0";
		OPTABlist.push_back(optab) ;
		
		optab.instrName = "TIO";
		optab.format = TYPE1;
		optab.opcode = "F8";
		OPTABlist.push_back(optab) ;
		
		optab.instrName = "TIX";
		optab.format = TYPE3or4;
		optab.opcode = "2C";
		OPTABlist.push_back(optab) ;
		
		optab.instrName = "TIXR";
		optab.format = TYPE2;
		optab.opcode = "B8";
		OPTABlist.push_back(optab) ;
		
		optab.instrName = "WD";
		optab.format = TYPE3or4;
		optab.opcode = "DC" ;
		OPTABlist.push_back(optab) ;
		
	}
  	
  	void readTable(vector<string> &table, string tableNum){
  		string fileName = "" ;
  		string str;
  		char ch ='\0' ;
  		fileName = "Table"+ tableNum + ".table" ;
  		
  		inFile.open(fileName.c_str()) ;
  		inFile.get(ch) ; //讀下一個字元	
  		
  		while (!inFile.eof()) {
  			while ( ch != '\n') { //讀到\n代表完整的字串已全部放入str 
  			    if(ch != ' ')
					str = str + ch ;
				inFile.get(ch) ;
				if(inFile.eof()) break ;
			}//while
			
			table.push_back(str) ; 	
			str = "" ;	//str重置
			inFile.get(ch) ;					
		}//while
		
       inFile.close() ;  		
	}//readTable()
	
	void readAllTable(){
		readTable(table1, "1") ;
		 
		/*cout << "table1: " << endl ;
		for ( int i = 0 ; i < table1.size() ; i++)
		    cout << table1[i] << endl ; 
		cout << table1.size() << endl ;*/
		     
		readTable(table2, "2") ;
		/*cout << "table2: " << endl ;
		for ( int i = 0 ; i < table2.size() ; i++)
		    cout << table2[i] << endl ;
		cout << table2.size() << endl ;*/
			 
		readTable(table3, "3") ;
		/*cout << "table3: " << endl ;
		for ( int i = 0 ; i < table3.size() ; i++)
		    cout << table3[i] << endl ;
		cout << table3.size() << endl ;*/
			 
		readTable(table4, "4") ;
		/*cout << "table4: " << endl ;
		for ( int i = 0 ; i < table4.size() ; i++)
		      cout << table4[i] << endl ;
	    cout << table4.size() << endl ;*/
		      
	}//readAllTable()
	
	void readFile(){
			
		do {
			cout << "請輸入檔名：";
			cin >> inputFileName ;
			inFile.open( inputFileName.c_str()); // 開檔 
			if ( inFile.fail() ) { 
      			cout << "NO FILE " << inputFileName << " PLEASE ENTER AGAIN!!" << endl; // 不能開 
    		} // if ( 找不到檔案 )
		}while(inFile.fail()) ;
		
		cout << inputFileName << " open success!!!" << endl;
		inFile.close() ;
	}//readFile()
	
	long long countAscii(string token){
		long long num = 0 ;
	
		for ( int i = 0 ; i < token.size() ; i++){
			num = num + int(token[i]) ;			  
		}//for
		
		return num ;		
	}//countAscii()
	
	int hashValue(long long num){
		int hashValue = 0 ;			
		hashValue = num % 100 ;	
		return hashValue ;		
	}//hashValue()
	
	void createLinearHash(int hashNum, string token, hashTable table[100], int &tableNo){
		int curIndex = 0 ;//目前hashTable的index位置 
		bool isDone = false ; //判斷是否找到空格 
		
		if(table[hashNum].hashValue == -1){
			table[hashNum].hashValue = hashNum ;
			table[hashNum].token = copyString(token) ;
			tableNo = hashNum ;
		}//if
		
		else{
			curIndex = hashNum ;
			while(!isDone){ //當還沒有找到空格，執行以下動作 
				curIndex++ ;				
				if(curIndex > 99){
					curIndex = 0 ;
				}//if
										
				if(table[curIndex].hashValue == -1){
				    isDone = true ;						
				}//if					
			}//while
			
			table[curIndex].hashValue = hashNum ;
			table[curIndex].token = copyString(token) ;
			tableNo = curIndex ;
			
		}//else
	}//createLinearHash()
	
	void createLiteralHash(int hashNum, int tableNum, int tableNo, int literal_type){
		int curIndex = 0 ;//目前hashTable的index位置 
		bool isDone = false ; //判斷是否找到空格 
		
		if(LITTAB[hashNum].hashValue == -1){
			LITTAB[hashNum].hashValue = hashNum ;
			LITTAB[hashNum].tableNum = tableNum ;
			LITTAB[hashNum].tableNo = tableNo ;
			LITTAB[hashNum].type = literal_type ;
		}//if
		
		else{
			curIndex = hashNum ;
			while(!isDone){ //當還沒有找到空格，執行以下動作 
				curIndex++ ;				
				if(curIndex > 99){
					curIndex = 0 ;
				}//if
										
				if(LITTAB[curIndex].hashValue == -1){
				    isDone = true ;						
				}//if					
			}//while
			
			LITTAB[hashNum].hashValue = hashNum ;
			LITTAB[hashNum].tableNum = tableNum ;
			LITTAB[hashNum].tableNo = tableNo ;	
			LITTAB[hashNum].type = literal_type ;	
		}//else
	}//createLinearHash()
	
	bool isInLiteralTable(int tableNum, int tableNo){
		for ( int i = 0 ; i < 100 ; i++){
			if(LITTAB[i].hashValue != -1){
				if(LITTAB[i].tableNum == tableNum && LITTAB[i].tableNo == tableNo){
					return true ;
				}//if
			}//if
		}//for
		return false ;
	}//isInLiteralTable()
		
	string copyString(string token){
		string str = "" ;
		for ( int i = 0 ; i < token.size() ; i++){
		   	str = str + token[i] ;
		}//for
		return str ;
	}//copyString()
		
	string LowToUpCase(string token){
		string upperCase = "" ;
		for ( int i = 0 ; i < token.size() ; i++){
			if(token[i] == 'a') upperCase = upperCase + 'A' ;
			else if(token[i] == 'b') upperCase = upperCase + 'B' ;
			else if(token[i] == 'c') upperCase = upperCase + 'C' ;
			else if(token[i] == 'd') upperCase = upperCase + 'D' ;
			else if(token[i] == 'e') upperCase = upperCase + 'E' ;
			else if(token[i] == 'f') upperCase = upperCase + 'F' ;
			else if(token[i] == 'g') upperCase = upperCase + 'G' ;
			else if(token[i] == 'h') upperCase = upperCase + 'H' ;
			else if(token[i] == 'i') upperCase = upperCase + 'I' ;
			else if(token[i] == 'j') upperCase = upperCase + 'J' ;
			else if(token[i] == 'k') upperCase = upperCase + 'K' ;
			else if(token[i] == 'l') upperCase = upperCase + 'L' ;
			else if(token[i] == 'm') upperCase = upperCase + 'M' ;
			else if(token[i] == 'n') upperCase = upperCase + 'N' ;
			else if(token[i] == 'o') upperCase = upperCase + 'O' ;
			else if(token[i] == 'p') upperCase = upperCase + 'P' ;
			else if(token[i] == 'q') upperCase = upperCase + 'Q' ;
			else if(token[i] == 'r') upperCase = upperCase + 'R' ;
			else if(token[i] == 's') upperCase = upperCase + 'S' ;
			else if(token[i] == 't') upperCase = upperCase + 'T' ;
			else if(token[i] == 'u') upperCase = upperCase + 'U' ;
			else if(token[i] == 'v') upperCase = upperCase + 'V' ;
			else if(token[i] == 'w') upperCase = upperCase + 'W' ;
			else if(token[i] == 'x') upperCase = upperCase + 'X' ;
			else if(token[i] == 'y') upperCase = upperCase + 'Y' ;
			else if(token[i] == 'z') upperCase = upperCase + 'Z' ;					
			else  upperCase = upperCase + token[i] ;			 
		}//for
		
		return upperCase ;
	}//upToLowCase()
	
	bool isTable1(string token, int &tableNo){
		string upCase = "" ;
		for ( int i = 0 ; i < table1.size() ; i++){
			upCase = LowToUpCase(table1[i]) ;
			if(token.compare(upCase) == 0) {
				tableNo = i + 1 ;
				return true ;
			}//if			
		}//for		
		return false ;
	}//isTable1()
	
	bool isTable2(string token, int &tableNo ){
		string upCase = "" ;
		for ( int i = 0 ; i < table2.size() ; i++){
			upCase = LowToUpCase(table2[i]) ;
			if(token.compare(upCase) == 0) {
				tableNo = i + 1 ;
				return true ;
			}//if			
		}//for		
		return false ;
	}//isTable2()
	
	bool isTable3(string token, int &tableNo ){
		string upCase = "" ;
		for ( int i = 0 ; i < table3.size() ; i++){
			upCase = LowToUpCase(table3[i]) ;
			if(token.compare(upCase) == 0) {
				tableNo = i + 1 ;
				return true ;
			}//if			
		}//for		
		return false ;
	}//isTable3()
	
	
	bool isTable1To3(string token, int &tableNum, int &tableNo){
		string upCase = LowToUpCase(token) ;
		if(isTable1(upCase, tableNo)) {
			tableNum = 1 ;
			return true ;
		}//if
		
		else if(isTable2(upCase, tableNo)){
			tableNum = 2 ;
			return true ;
		}//if
		
		else if(isTable3(upCase, tableNo)){
			tableNum = 3 ;
			return true ;
		}//if
		
		return false ;				
	}//isTable1To3()
	
	bool isDelimiter(char ch, int&tableNo){
		 string str = "" ;
		 str = str + ch ;
		 for ( int i = 0 ; i < table4.size() ; i++){		
			if(str.compare(table4[i]) == 0) {
				tableNo = i + 1 ;
				return true ;
			}//if				
		 }//for		 
		 return false ;
	}//isDelimiter()
	
	bool isInteger(string token){
		for ( int i = 0 ; i < token.size() ; i++){
			if( token[i] < '0' || token[i] >'9')
			   return false ;
		}//for		
		return true ;
	}//isInteger()
	
	bool isInTable5(string token, int &tableNo){
        for ( int i = 0 ; i < 100 ; i++){
            if (table5[i].hashValue != -1){
            	if(table5[i].token.compare(token) == 0){
            		tableNo = i ;
            		return true ;
				}//if
			}//if
		}//for
		
		return false ;
	}//isTable5()
	
	bool isInTable6(string token, int &tableNo){
		string upCase = "" ;
		upCase = LowToUpCase(token) ;
        for ( int i = 0 ; i < 100 ; i++){
            if (table6[i].hashValue != -1){
            	if(LowToUpCase(table6[i].token).compare(upCase) == 0){
            		tableNo = i ;
            		return true ;
				}//if
			}//if
		}//for
		
		return false ;
	}//isTable6()
	
	bool isInTable7(string token, int &tableNo){
        for ( int i = 0 ; i < 100 ; i++){
            if (table7[i].hashValue != -1){
            	if(table7[i].token.compare(token) == 0){
            		tableNo = i ;
            		return true ;
				}//if
			}//if
		}//for
		
		return false ;
	}//isTable7()
		
	void cutCmdToken(string cmdLine){
		string token = "" ;
		int tableNum = -1 ;
		int tableNo = -1 ;
		int delNo = -1 ;
		outTable tmp ;
		tokenTable tokenTable ;
		vector<outTable> outList ;
		
		for ( int i = 0 ; i < cmdLine.size() ; i++){
		    if (cmdLine[i] == ' ' || cmdLine[i] == '\t' ){		    	
				if(!token.empty()){
				  	//cout << "token: " << token << endl ;
					if(isTable1To3(token, tableNum, tableNo)){
						tmp.tableNum = tableNum ;
						tmp.tableNo = tableNo ;
						outList.push_back(tmp) ;
					}//if
					
					else if(isInteger(token)) {
						if(!isInTable6(token, tableNo)){
							createLinearHash(hashValue(countAscii(token)), token, table6, tableNo) ;
						}//if
						
						tmp.tableNum = 6 ;
						tmp.tableNo = tableNo ;
						outList.push_back(tmp) ;
					}//if	
				
					else {
						if(!isInTable5(token, tableNo)){
							createLinearHash(hashValue(countAscii(token)), token, table5, tableNo) ;
						}//if
						
						tmp.tableNum = 5 ;
						tmp.tableNo = tableNo ;
						outList.push_back(tmp) ;
					}//else  
					token = "" ;			   
			    }//if					
				
			}//if
			
			else if(isDelimiter(cmdLine[i], tableNo)){ //table4
			    //cout << "token: " << cmdLine[i] << endl ;
			    delNo = tableNo ;
			    if((token.compare("C") == 0 || token.compare("c") == 0) && cmdLine[i] =='\''){
			    	token = "" ;
			    	
			    	tmp.tableNum = 4 ;
					tmp.tableNo = tableNo ;
					outList.push_back(tmp) ;
					//cout << "token: " << cmdLine[i] << endl ;
					
					i++ ;
					while(cmdLine[i]!= '\''){
						token = token + cmdLine[i] ;
						i++ ;
					}//while
					
					if(!isInTable7(token, tableNo)){
						createLinearHash(hashValue(countAscii(token)), token, table7, tableNo) ;
					}//if
					
					tmp.tableNum = 7 ;
					tmp.tableNo = tableNo ;
					outList.push_back(tmp) ;
					//cout << "token: " << token << endl ;
					
					tmp.tableNum = 4 ;
					tmp.tableNo = delNo ;
					outList.push_back(tmp) ;
					
					
					
				}//if
				
				else if((token.compare("X") == 0 || token.compare("x") == 0) && cmdLine[i] =='\''){
			    	token = "" ;
			    	tmp.tableNum = 4 ;
					tmp.tableNo = tableNo ;
					outList.push_back(tmp) ;
					
					i++ ;
					while(cmdLine[i]!= '\''){
						token = token + cmdLine[i] ;
						i++ ;
					}//while
					
					if(!isInTable6(LowToUpCase(token), tableNo)){
						createLinearHash(hashValue(countAscii(LowToUpCase(token))), token, table6, tableNo) ;
					}//if
					//cout << "token: " << token << endl ;
					
					tmp.tableNum = 6 ;
					tmp.tableNo = tableNo ;
					outList.push_back(tmp) ;
					
					tmp.tableNum = 4 ;
					tmp.tableNo = delNo ;
					outList.push_back(tmp) ;
				}//if
				
			    else {
			    	delNo = tableNo ;
			    	if(!token.empty()){
					  	//cout << "token: " << token << endl ;
						if(isTable1To3(token, tableNum, tableNo)){
							tmp.tableNum = tableNum ;
							tmp.tableNo = tableNo ;
							outList.push_back(tmp) ;
						}//if
						
						else if(isInteger(token)) {
							if(!isInTable6(token, tableNo)){
								createLinearHash(hashValue(countAscii(token)), token, table6, tableNo) ;
							}//if
							
							tmp.tableNum = 6 ;
							tmp.tableNo = tableNo ;
							outList.push_back(tmp) ;
						}//if	
					
						else {
							if(!isInTable5(token, tableNo)){
								createLinearHash(hashValue(countAscii(token)), token, table5, tableNo) ;
							}//if
							
							tmp.tableNum = 5 ;
							tmp.tableNo = tableNo ;
							outList.push_back(tmp) ;
						}//else  			   
			       }//if
				   
				   if(cmdLine[i]== '.'){				   	    
					    i = cmdLine.size() ;
				   }//if	
				   
				    tmp.tableNum = 4 ;
					tmp.tableNo = delNo ;
					outList.push_back(tmp) ;		
				}//else		
				
				token = "" ;	    				
			}//if
			
			else{
				token = token + cmdLine[i] ;
			}//else
						
		}//for
		
			if(!token.empty()){
			  	//cout << "token: " << token << endl ;
				if(isTable1To3(token, tableNum, tableNo)){
					tmp.tableNum = tableNum ;
					tmp.tableNo = tableNo ;
					outList.push_back(tmp) ;
				}//if
				
				else if(isInteger(token)) {
					if(!isInTable6(token, tableNo)){
						createLinearHash(hashValue(countAscii(token)), token, table6, tableNo) ;
					}//if
					
					tmp.tableNum = 6 ;
					tmp.tableNo = tableNo ;
					outList.push_back(tmp) ;
				}//if	
			
				else {
					if(!isInTable5(token, tableNo)){
						createLinearHash(hashValue(countAscii(token)), token, table5, tableNo) ;
					}//if
					
					tmp.tableNum = 5 ;
					tmp.tableNo = tableNo ;
					outList.push_back(tmp) ;
				}//else  			   
			}//if					
		
		token = "" ;
		
		/*cout << cmdLine << endl ;
		for ( int i = 0 ; i < outList.size() ; i++){
			cout << "(" << outList[i].tableNum << "," << outList[i].tableNo << ")" ;
		}//for*/
		
		//將cut token後結果放置在 tokenTableList
		if(!outList.empty()){			
			tokenTable.cmd = cmdLine ;
			tokenTable.outList = outList ;
			tokenTableList.push_back(tokenTable) ;
		}//if
		
		outList.clear() ;
	}//cutCmdToken()
	
	void cutFileToken(){
		string cmdLine = "" ;
		char ch = '\0' ;		
		inFile.open(inputFileName.c_str()) ; //開檔 
		
		inFile.get(ch) ;
		while(!inFile.eof()){
			while ( ch != '\n'){
				cmdLine = cmdLine + ch ;
				inFile.get(ch) ;
				if(inFile.eof()) break ;
			}//while
								
			cutCmdToken(cmdLine) ; //切指令token 
									
			cmdLine = "" ;
			inFile.get(ch) ;
		}//while
		
		inFile.close() ;
	}//cutFileToken()
	
	string findToken(hashTable table[100], int tableNo){
		return table[tableNo].token ;
	}//for
	
	int stringToInt(string str){
		return atoi(str.c_str()) ;
	}//stringToInt(string str)
	
	string intToString(int num){
		stringstream ss;
        ss << num;
        return ss.str() ;
	}//intToString(int num)
	
	string change10T016(int loc){
		stringstream ss;
		ss << hex << loc ;
		string str = ss.str() ;
		return str ;
	}//(change10T016)
		
	bool isFirstTokenRight(outTable firstTab){
		if(firstTab.tableNum == 5 || firstTab.tableNum == 2 || firstTab.tableNum == 1 || 
		    (firstTab.tableNum == 4 && firstTab.tableNo == 2 ) ||(firstTab.tableNum == 4 && firstTab.tableNo == 10 ))
			  return true ;
	    else
	    	return false ;		    
	} //isFirstTokenRight()
	
	bool define_operand(vector<OPERAND_GROUP> &operandList, outTable outTable){
		string str = "" ;
		
		for (int i = 0 ; i < operandList.size() ; i++){
			//先判斷token開頭 
		    if(operandList[i].outList[0].tableNum == 3){ //開頭為reg 
		        //operand只有一個token 
		     	if(operandList[i].outList.size() == 1){
		     		operandList[i].operand_type = REG ;					     		
				}//if		    	
				else return false ;
			}//if
			
			else if(operandList[i].outList[0].tableNum == 6){ //開頭為integer
				//operand只有一個token 
		     	if(operandList[i].outList.size() == 1){
		     		str = findToken(table6, operandList[i].outList[0].tableNo) ;
					//判斷數值是否是10進位 
					if(isInteger(str)){		
					    //cout << "i: " << i << endl ;				
						operandList[i].operand_type = VALUE ;
					}//if		     									     		
				}//if	
				else return false ;	    			    	
			}//if
			
			else if(operandList[i].outList[0].tableNum == 5){ //開頭為symbal(label)
			    if(operandList[i].outList.size() == 1)
		     		operandList[i].operand_type = MEMORY ;	
			    else return false ;    				
			}//if
			
			else if(operandList[i].outList[0].tableNum == 4 && operandList[i].outList[0].tableNo == 12){ //開頭為'#'
				//operand有兩個token 
		     	if(operandList[i].outList.size() == 2){
		     		//第二個token為symbol
		     		if(operandList[i].outList[1].tableNum == 5 ){
		     			operandList[i].operand_type = IMME_VALUE ;
					}//if
					
					//第二個token為integer
					else if(operandList[i].outList[1].tableNum == 6){
						str = findToken(table6, operandList[i].outList[1].tableNo) ;
						//判斷數值是否是10進位 
						if(isInteger(str)){
							operandList[i].operand_type = IMME_VALUE ;
						}//if
						else return false ;
					}//if	
					else return false ;	     				     		
				}//if	
				else return false ;			
			}//if
			
			else if(operandList[i].outList[0].tableNum == 4 && operandList[i].outList[0].tableNo == 11){ //開頭為'='
			    // =x'f1', =c'eof'，operand有4個token 
				if(operandList[i].outList.size() == 4){
					//第二個和第4個token為'\'' 
					if((operandList[i].outList[1].tableNum == 4 && operandList[i].outList[1].tableNo == 9) && 
						(operandList[i].outList[3].tableNum == 4 && operandList[i].outList[3].tableNo == 9)){
						//第三個token為integer or string 
						if(operandList[i].outList[2].tableNum == 6){
							if(findToken(table6, operandList[i].outList[2].tableNo).size() <= 0 || findToken(table6, operandList[i].outList[2].tableNo).size() > 6  )
								return false;
							else
								operandList[i].operand_type = LITERAL ;									
						}//if
						else if(operandList[i].outList[2].tableNum == 7){							
							operandList[i].operand_type = LITERAL ;									        								
						}//if				
						else return false ;			
					}//if
					else return false ;
				}//if
				
				// =3277，operand有2個token
				else if(operandList[i].outList.size() == 2){
					//第二個token為 integer
					if(operandList[i].outList[1].tableNum == 6){
						str = findToken(table6, operandList[i].outList[1].tableNo) ;
						//判斷數值是否是10進位 
						if(isInteger(str)){
							operandList[i].operand_type = LITERAL ;
						}//if
						else return false ;
					}//if
					else return false ;
				}//if	
				else return false ;			
			}//if
			
			else if(operandList[i].outList[0].tableNum == 4 && operandList[i].outList[0].tableNo == 13){ //開頭為'@'
			    //operand有兩個token 
				if(operandList[i].outList.size() == 2){
					//第二個token為label
					if(operandList[i].outList[1].tableNum == 5 ){
						operandList[i].operand_type = MEMORY ;
					}//if
					
				    /*//第二個token為integer
					else if(operandList[i].outList[1].tableNum == 6){
						str = findToken(table6, operandList[i].outList[1].tableNo) ;
						//判斷數值是否是10進位 
						if(isInteger(str)){
							operandList[i].operand_type = MEMORY ;
						}//if
						else return false ;
					}//if*/
					else return false ;
				}//if
				else return false ;
			}//if	
			
			else if(operandList[i].outList[0].tableNum == 4 && operandList[i].outList[0].tableNo == 9){ //開頭為''/'
			    //operand有三個token 
				if(operandList[i].outList.size() == 3){
					//第三個token為'\''
					if(operandList[i].outList[2].tableNum == 4 && operandList[i].outList[2].tableNo == 9) {
						//第二個token為string、integer 
						if(operandList[i].outList[1].tableNum == 6 ){
							operandList[i].operand_type = PSEUDO_VALUE ;
							return true ;
						}//if
						
						else if(operandList[i].outList[1].tableNum == 7){
							operandList[i].operand_type = PSEUDO_STRING ;
							return true ;
						}//if
						else return false ;
					}//if
					else return false ;
				}//if		
				else return false ;		
			}//if
			
			else
				return false ;							
		}//for
		
		return true ;
		
	}//define_operand()
	
	bool isInstrSyntaxRight(tokenTable &tokenTable){
		int inStr_tableNum = tokenTable.token_group.instr.tableNum ;
		int inStr_tableIndex = tokenTable.token_group.instr.tableNo-1 ;	
		string str = "" ;
		
		//先判斷operand是否合乎文法，若合乎文法，再判斷指令是否合乎文法 
		if(define_operand(tokenTable.token_group.operandList, tokenTable.token_group.instr )){
			
			//指令為execution instruction 		
			if(inStr_tableNum == 1){
				cout << "format:" << OPTABlist[inStr_tableIndex].format << endl ;
				//指令為type1 
				if(OPTABlist[inStr_tableIndex].format == TYPE1){
					//沒有任何operand 
					if(tokenTable.token_group.operandList.empty())
						return true ;				
				}//if
				
				//指令為type2
				else if(OPTABlist[inStr_tableIndex].format == TYPE2){
					
					
					if(inStr_tableIndex == 4 ){ //clear、tixr(1 reg) 
					    //operand只有一個 
						if(tokenTable.token_group.operandList.size() == 1){
							//operand為REG 
							if(tokenTable.token_group.operandList[0].operand_type == REG)
								return true ;
						}//if					   		
					}//if
					
					//addr、compr、divr、mulr、rmo、subr (2 reg)
					if(inStr_tableIndex == 2 || inStr_tableIndex == 7 || inStr_tableIndex == 10 || inStr_tableIndex ==30 || inStr_tableIndex ==34 || inStr_tableIndex == 52 || inStr_tableIndex == 57 ){ 
						//operand有兩個 
						if(tokenTable.token_group.operandList.size() == 2){
							//operand皆為REG 
							if(tokenTable.token_group.operandList[0].operand_type == REG && tokenTable.token_group.operandList[1].operand_type == REG)
								return true ;
						}//if				
					}//if
					
					else if(inStr_tableIndex == 36 || inStr_tableIndex == 37 ){ //shiftl、shiftr
						//operand有兩個 
						if(tokenTable.token_group.operandList.size() == 2){
							//第一個opernad為reg 
							if(tokenTable.token_group.operandList[0].operand_type == REG ){
								//第二個opernad為integer
								if(tokenTable.token_group.operandList[1].operand_type == VALUE){
									str = findToken(table6, tokenTable.token_group.operandList[1].outList[0].tableNo) ;
									//若REG為'F' 
									if(tokenTable.token_group.operandList[0].outList[0].tableNo == 6){										
										if(stringToInt(str) > 0 && stringToInt(str) <= 47 ){
											return true ;
										}//if
									}//if
									
									//若REG為'A','X','L','B','S','T','PC','SW' 
									else{										
										if(stringToInt(str) > 0 && stringToInt(str) <= 24 ){
											return true ;
										}//if										
									}//else									
								}//if
							}//if
						}//if 						
					}//if	
					
					else if(inStr_tableIndex == 53 ){ //svc
					    //operand只有一個 
						if(tokenTable.token_group.operandList.size() == 1){
							if(tokenTable.token_group.operandList[0].operand_type == VALUE){
								return true ;
							}//if							
						}//if					    
					}//if							
				}//if
				
				//指令為type3、type4 
				else if(OPTABlist[inStr_tableIndex].format == TYPE3or4){
					if(inStr_tableIndex == 35){
						if(tokenTable.token_group.operandList.size() == 0) return true ;
					}//if
					else{
						//operand有兩個 				
						if(tokenTable.token_group.operandList.size() == 2){
							//第二個operand為reg 'x' 
							if(tokenTable.token_group.operandList[1].outList[0].tableNum == 3 && tokenTable.token_group.operandList[1].outList[0].tableNo == 2){
								//第一個operand為symbol、literal、integer 
								if(tokenTable.token_group.operandList[0].operand_type == VALUE || tokenTable.token_group.operandList[0].operand_type == LITERAL || 
								       tokenTable.token_group.operandList[0].operand_type == MEMORY ){
								    return true ;
								}//if
							}//if						
						}//if
						
						//operand只有一個 
						else if(tokenTable.token_group.operandList.size() == 1){
							//第一個operand為symbol、literal、integer 、immediate value 
							if(tokenTable.token_group.operandList[0].operand_type == VALUE || tokenTable.token_group.operandList[0].operand_type == LITERAL || 
								tokenTable.token_group.operandList[0].operand_type == MEMORY || tokenTable.token_group.operandList[0].operand_type == IMME_VALUE ){
								return true ;
							}//if
						}//if
						
					}//else													
				}//if(tpye 3 4)									
			}//if
			
			//指令為pseudo instruction 
			else if(inStr_tableNum  == 2){		
			    cout << "format: Pseudo"  << endl ;	
				if(inStr_tableIndex == 0){ //START
					//operand只有一個 				
					if(tokenTable.token_group.operandList.size() == 1){						
						if(tokenTable.token_group.operandList[0].operand_type == VALUE){							
							return true ;
						}//if
					}//if
				}//if
				
				else if(inStr_tableIndex == 1){ //END
					//operand只有一個 				
					if(tokenTable.token_group.operandList.size() == 1){
						if(tokenTable.token_group.operandList[0].operand_type == MEMORY){
							return true ;
						}//if
					}//if						
				}//else
				
				else if(inStr_tableIndex == 2){ //BYTE
				    //operand只有一個 	
					if(tokenTable.token_group.operandList.size() == 1){
						if(tokenTable.token_group.operandList[0].operand_type == PSEUDO_VALUE){
							str = findToken(table6, tokenTable.token_group.operandList[0].outList[1].tableNo) ; 
							//只佔一個byte 
							if(str.size() > 0 && str.size() <= 2){
								return true ;
							}//if
						}//if
						
						else if(tokenTable.token_group.operandList[0].operand_type == PSEUDO_STRING)
							return true ;
						else if(tokenTable.token_group.operandList[0].operand_type == VALUE)
							return true ;
					}//if																			
				}//if
				
				else if(inStr_tableIndex == 3){ //WORD
					//operand只有一個 	
					if(tokenTable.token_group.operandList.size() == 1){
						if(tokenTable.token_group.operandList[0].operand_type == PSEUDO_VALUE){
							str = findToken(table6, tokenTable.token_group.operandList[0].outList[1].tableNo) ; 
							//只佔一個WORD
							if(str.size() > 0 && str.size() <= 6){
								return true ;
							}//if
						}//if
						
						else if(tokenTable.token_group.operandList[0].operand_type == PSEUDO_STRING)
							return true ;
						else if(tokenTable.token_group.operandList[0].operand_type == VALUE)
							return true ;
					}//if						   
				}//if
				
				else if(inStr_tableIndex == 4){ //RESB
					//operand只有一個 				
					if(tokenTable.token_group.operandList.size() == 1){
						if(tokenTable.token_group.operandList[0].operand_type == VALUE){
							return true ;
						}//if
					}//if					
				}//if
				
				else if(inStr_tableIndex == 5){ //RESW
					//operand只有一個 				
					if(tokenTable.token_group.operandList.size() == 1){
						if(tokenTable.token_group.operandList[0].operand_type == VALUE){
							return true ;
						}//if
					}//if						
				}//if
													
				else if(inStr_tableIndex == 6){ //EQU
				   //operand只有一個 
					if(tokenTable.token_group.operandList.size() == 1){
						if(tokenTable.token_group.operandList[0].operand_type == VALUE)
							return true ;
						else if(tokenTable.token_group.operandList[0].operand_type == MEMORY)
							return true ;
					}//if
					else if(tokenTable.token_group.operandList.empty())
						return true ;
				}//if
				
				else if(inStr_tableIndex == 7){ //BASE
				    //operand只有一個 
					if(tokenTable.token_group.operandList.size() == 1){
						//第一個operand為symbol、literal、integer 
						if(tokenTable.token_group.operandList[0].operand_type == VALUE || tokenTable.token_group.operandList[0].operand_type == MEMORY || 
						    tokenTable.token_group.operandList[0].operand_type == LITERAL )
						    return true ;
					}//if
				}//if
				
				else if(inStr_tableIndex == 8){ //LTORG
				    //不存在operand 
				   	if(tokenTable.token_group.operandList.empty()){
				   		return true ;
					}//if	
				}//if											
			}//if			
		}//if
		
		return false ;						
	}//isInstrSyntaxRight(tokenTable &tokenTable)
		
	void checkSyntax(tokenTable &tokenTable){
		int i = 0 ; //目前判斷的token index 
		int outlistSize = tokenTable.outList.size() ;
		OPERAND_GROUP tmp ;
		outTable outtab ;
		
		//先判斷指令開頭是否正確
		if(isFirstTokenRight(tokenTable.outList[0])){ 
		    		    		    
			//指令開頭為label 
	       if(tokenTable.outList[0].tableNum == 5){
	       	    //更新指令token_group的label 
	       	    tokenTable.token_group.label.tableNum = tokenTable.outList[0].tableNum ;
		        tokenTable.token_group.label.tableNo = tokenTable.outList[0].tableNo ;
		        
	       	    i++ ; //i = 1
	       		if(outlistSize > i){ //判斷label後是否還有東西 	       		    
	       			if(tokenTable.outList[i].tableNum == 4 && tokenTable.outList[i].tableNo == 2 ){ //第二個token為'+'
	       			    cout << "case1!!!!!!!" << endl ;
					    //設定指令token_group為type4 
                        tokenTable.token_group.isType4 = true ; //指令為type4 
                        
	       				i++ ; //i = 2
	       				if(outlistSize > i){ //判斷'+'後是否還有東西
	       					if(tokenTable.outList[i].tableNum == 1){ //判斷'+'後是否為execution instruction 
	       					     //更新指令token_group的instr 
	       						tokenTable.token_group.instr.tableNum = tokenTable.outList[i].tableNum ;
		    					tokenTable.token_group.instr.tableNo = tokenTable.outList[i].tableNo ;
		    					i++ ; //i = 3 ;
		    					if(outlistSize > i){ //判斷instruction後是否還有東西//若沒有，直接結束 
		    						
		    					    //判斷operand的開頭或結尾是否是',' 
		    						if(!(tokenTable.outList[i].tableNum == 4 && tokenTable.outList[i].tableNo == 1) && !(tokenTable.outList[outlistSize-1].tableNum == 4 && tokenTable.outList[outlistSize-1].tableNo == 1) &&
					                	!((tokenTable.outList[outlistSize-1].tableNum == 4 && tokenTable.outList[outlistSize-1].tableNo == 10) &&(tokenTable.outList[outlistSize-2].tableNum == 4 && tokenTable.outList[outlistSize-2].tableNo == 1))){
			
										for(int j = i ; j < outlistSize ; j++){
		    								while((j < outlistSize) && !(tokenTable.outList[j].tableNum == 4 && tokenTable.outList[j].tableNo == 1 )){
		    									//判斷此token使否不是最後一個且也不是'.' 
		    									if( !(j == outlistSize-1 && (tokenTable.outList[j].tableNum == 4 && tokenTable.outList[j].tableNo == 10 ))){
		    										outtab.tableNum = tokenTable.outList[j].tableNum ;
		    										outtab.tableNo = tokenTable.outList[j].tableNo ;
			    									tmp.outList.push_back(outtab) ;
												}//if 
												j++ ;
											}//while
											
											if(!tmp.outList.empty()) {
												tokenTable.token_group.operandList.push_back(tmp) ;
												
											}//if  								
												
											tmp.outList.clear() ;		    								
										}//for
										
										if(!isInstrSyntaxRight(tokenTable)){
											cout << "syntax error" << endl ;
											tokenTable.syntaxError = true ;
											tokenTable.token_group.operandList.clear() ;
										}//if
										
									}//if
									else{ //若operand的開頭或結尾是'.'，syntax error  
										cout << "syntax error" << endl ;
										tokenTable.syntaxError = true ;
									}//else
								}//if
							}//if
							else{ //若'+'後非execution instruction，syntax error 
								cout << "syntax error" << endl ;
								tokenTable.syntaxError = true ;
							}//else
						}//if
						else{ //若'+'後沒有東西syntax error 
							cout << "syntax error" << endl ;
							tokenTable.syntaxError = true ;
						}//else
	       				
					}//if
					
					else if(tokenTable.outList[i].tableNum == 1 || tokenTable.outList[i].tableNum == 2){ //第二個token為instruction 
					    cout << "case2!!!!!!!" << endl ;
					    tokenTable.token_group.instr.tableNum = tokenTable.outList[i].tableNum ;
		    			tokenTable.token_group.instr.tableNo = tokenTable.outList[i].tableNo ;
		    			
						i++ ; //i = 2
						if(outlistSize > i){ //判斷instruction後是否還有東西//若沒有，直接結束 
    					    //判斷operand的開頭或結尾是否是',' 
    						if(!(tokenTable.outList[i].tableNum == 4 && tokenTable.outList[i].tableNo == 1) &&  !(tokenTable.outList[outlistSize-1].tableNum == 4 && tokenTable.outList[outlistSize-1].tableNo == 1) &&
					                	!((tokenTable.outList[outlistSize-1].tableNum == 4 && tokenTable.outList[outlistSize-1].tableNo == 10) &&(tokenTable.outList[outlistSize-2].tableNum == 4 && tokenTable.outList[outlistSize-2].tableNo == 1)) ){
    							for(int j = i ; j < outlistSize ; j++){
    								while((j < outlistSize) && !(tokenTable.outList[j].tableNum == 4 && tokenTable.outList[j].tableNo == 1 )){
    									//判斷此token使否不是最後一個且也不是',' 
    									if( !(j == outlistSize-1 && (tokenTable.outList[j].tableNum == 4 && tokenTable.outList[j].tableNo == 10 ))){
    										outtab.tableNum = tokenTable.outList[j].tableNum ;
    										outtab.tableNo = tokenTable.outList[j].tableNo ;
	    									tmp.outList.push_back(outtab) ;
										}//if 
										j++ ;
									}//while
									
									if(!tmp.outList.empty()) {
										tokenTable.token_group.operandList.push_back(tmp) ;									
									}//if  	
									tmp.outList.clear() ;	    								
								}//for
								
								
								if(!isInstrSyntaxRight(tokenTable)){
									cout << "syntax error" << endl ;
									tokenTable.syntaxError = true ;
									tokenTable.token_group.operandList.clear() ;
								}//if
								
							}//if
							else{ //若operand的開頭或結尾是'.'，syntax error  
								cout << "syntax error" << endl ;
								tokenTable.syntaxError = true ;
							}//else
						}//if
					}//if
					
					
					else{ //非上述兩個，syntax error 
						cout << "syntax error" << endl ;
						tokenTable.syntaxError = true ;
					}//else
					
					
				}//if
				else { //若lable後沒有東西syntax error 
					cout << "syntax error" << endl ;
					tokenTable.syntaxError = true ;
				}//else
		   }//if
		   
		   //指令開頭為instruction
		   else if(tokenTable.outList[0].tableNum == 1 || tokenTable.outList[0].tableNum == 2){
		   	    cout << "case3!!!!!!!" << endl ;
		   	    tokenTable.token_group.instr.tableNum = tokenTable.outList[i].tableNum ;
    			tokenTable.token_group.instr.tableNo = tokenTable.outList[i].tableNo ;
    			
				i++ ; //i = 1
				if(outlistSize > i){ //判斷instruction後是否還有東西//若沒有，直接結束 
				    //判斷operand的開頭或結尾是否是',' 
					if(!(tokenTable.outList[i].tableNum == 4 && tokenTable.outList[i].tableNo == 1) &&  !(tokenTable.outList[outlistSize-1].tableNum == 4 && tokenTable.outList[outlistSize-1].tableNo == 1) &&
					      !((tokenTable.outList[outlistSize-1].tableNum == 4 && tokenTable.outList[outlistSize-1].tableNo == 10) &&(tokenTable.outList[outlistSize-2].tableNum == 4 && tokenTable.outList[outlistSize-2].tableNo == 1))){
						for(int j = i ; j < outlistSize ; j++){
							while((j < outlistSize) && !(tokenTable.outList[j].tableNum == 4 && tokenTable.outList[j].tableNo == 1 )){
								//判斷此token使否不是最後一個且也不是'.' 
								if( !(j == outlistSize-1 && (tokenTable.outList[j].tableNum == 4 && tokenTable.outList[j].tableNo == 10 ))){
									outtab.tableNum = tokenTable.outList[j].tableNum ;
									outtab.tableNo = tokenTable.outList[j].tableNo ;
									tmp.outList.push_back(outtab) ;
								}//if 
								j++ ;
							}//while

							if(!tmp.outList.empty()) {
								tokenTable.token_group.operandList.push_back(tmp) ;								
							}//if  		
							tmp.outList.clear() ;	    								
						}//for
						
						if(!isInstrSyntaxRight(tokenTable)){
							cout << "syntax error" << endl ;
							tokenTable.syntaxError = true ;
							tokenTable.token_group.operandList.clear() ;
						}//if
					}//if
					else{ //若operand的開頭或結尾是','，syntax error  
						cout << "syntax error" << endl ;
						tokenTable.syntaxError = true ;
					}//else
				}//if
		   	
		   }//if
		   
		   //指令開頭為'+'
		   else if(tokenTable.outList[0].tableNum == 4 && tokenTable.outList[0].tableNo == 2){
		   	    cout << "case4!!!!!!!" << endl ;
		   		//設定指令token_group為type4 
                tokenTable.token_group.isType4 = true ; //指令為type4 
                
   				i++ ; //i = 1
   				if(outlistSize > i){ //判斷'+'後是否還有東西
   					if(tokenTable.outList[i].tableNum == 1){ //判斷'+'後是否為execution instruction 
 					    
   					     //更新指令token_group的instr 
   						tokenTable.token_group.instr.tableNum = tokenTable.outList[i].tableNum ;
    					tokenTable.token_group.instr.tableNo = tokenTable.outList[i].tableNo ;
    					i++ ; //i = 2 ;
    					if(outlistSize > i){ //判斷instruction後是否還有東西//若沒有，直接結束 
    					    //判斷operand的開頭或結尾是否是',' 
    						if(!(tokenTable.outList[i].tableNum == 4 && tokenTable.outList[i].tableNo == 1) &&  !(tokenTable.outList[outlistSize-1].tableNum == 4 && tokenTable.outList[outlistSize-1].tableNo == 1) &&
					             !((tokenTable.outList[outlistSize-1].tableNum == 4 && tokenTable.outList[outlistSize-1].tableNo == 10) &&(tokenTable.outList[outlistSize-2].tableNum == 4 && tokenTable.outList[outlistSize-2].tableNo == 1)) ){
    							for(int j = i ; j < outlistSize ; j++){
    								while((j < outlistSize) && !(tokenTable.outList[j].tableNum == 4 && tokenTable.outList[j].tableNo == 1 )){
    									//判斷此token使否不是最後一個且也不是'.' 
    									if( !(j == outlistSize-1 && (tokenTable.outList[j].tableNum == 4 && tokenTable.outList[j].tableNo == 10 ))){
    										outtab.tableNum = tokenTable.outList[j].tableNum ;
    										outtab.tableNo = tokenTable.outList[j].tableNo ;
	    									tmp.outList.push_back(outtab) ;
										}//if 
										j++ ;
									}//while
									
									if(!tmp.outList.empty()) {
										tokenTable.token_group.operandList.push_back(tmp) ;
										
									}//if  	
									tmp.outList.clear() ;		    								
								}//for
								
								if(!isInstrSyntaxRight(tokenTable)){
									cout << "syntax error" << endl ;
									tokenTable.syntaxError = true ;
									tokenTable.token_group.operandList.clear() ;
								}//if
							}//if
							else{ //若operand的開頭或結尾是','，syntax error  
								cout << "syntax error" << endl ;
								tokenTable.syntaxError = true ;
							}//else
						}//if
					}//if
					else{ //若'+'後非execution instruction，syntax error 
						cout << "syntax error" << endl ;
						tokenTable.syntaxError = true ;
					}//else
				}//if
				else{ //若'+'後沒有東西syntax error 
					cout << "syntax error" << endl ;
					tokenTable.syntaxError = true ;
				}//else
		   	
		   }//if
		   
		   //指令開頭為'.'
		   else{
		   	    cout << "case5!!!!!!!" << endl ;
		   		tokenTable.isOnlyComment = true ;
		   }//else
		}//if
		
		else{
			cout << "syntax error" << endl ;
			tokenTable.syntaxError = true ;
		}//else		
	}//checkSyntax()
	
	//integer value 16進位轉10進位，不考慮A~F 
	unsigned int change16To10(string str){
		unsigned int sixteen = 1 ;
		unsigned int total = 0 ;
		unsigned int ch = 0 ;
		
		for(int i = 0 ; i < str.size()-1 ; i++){
			sixteen = sixteen * 16 ;
		}//for

		for ( int i= 0; i < str.size() ; i++ ){
			ch = int(str.at(i)) - 48 ;
			total = total + ch * sixteen ;
			sixteen = sixteen / 16 ;
		}//for
		
		return total ;		
	}//int
	
	
	//查是否指令的label已存在在cross referfence 
	bool isInSymbolTable(int symbol_table5_no, unsigned int &address){
		for( int i = 0 ; i < crossRef.size() ; i++ ){
			if(crossRef[i].label_tableNo == symbol_table5_no ){
				address = crossRef[i].address ;
				return true ;
			}//if
			  
		}//for		
	   return false ;		
	}//isInSymbolTable()
	
	bool isInLiteralTable(int tableNum, int tableNo, int &return_literalTable_No){
		for( int i = 0 ; i < 100 ; i++){
			if(LITTAB[i].tableNum == tableNum && LITTAB[i].tableNo == tableNo){
				return_literalTable_No = i;
				return true ;
			}//if
		}//for
		return false ;
	}//isInLiteralTable()
	
	string numChange_minus1(string str){
		if(str.compare("1") == 0) return "0" ;
		else if(str.compare("2") == 0) return "1" ;
		else if(str.compare("3") == 0) return "2" ;
		else if(str.compare("4") == 0) return "3" ;
		else if(str.compare("5") == 0) return "4" ;
		else if(str.compare("6") == 0) return "5" ;
		else if(str.compare("7") == 0) return "6" ;
		else if(str.compare("8") == 0) return "7" ;
		else if(str.compare("9") == 0) return "8" ;
		else if(str.compare("10") == 0) return "9" ;
		else if(str.compare("11") == 0) return "A" ;
		else if(str.compare("12") == 0) return "B" ;
		else if(str.compare("13") == 0) return "C" ;
		else if(str.compare("14") == 0) return "D" ;
		else if(str.compare("15") == 0) return "E" ;
		else if(str.compare("16") == 0) return "F" ;
		else return "0" ;
	}//str
	
	string numChange(string str){
		if(str.compare("1") == 0) return "1" ;
		else if(str.compare("2") == 0) return "2" ;
		else if(str.compare("3") == 0) return "3" ;
		else if(str.compare("4") == 0) return "4" ;
		else if(str.compare("5") == 0) return "5" ;
		else if(str.compare("6") == 0) return "6" ;
		else if(str.compare("7") == 0) return "7" ;
		else if(str.compare("8") == 0) return "8" ;
		else if(str.compare("9") == 0) return "9" ;
		else if(str.compare("10") == 0) return "A" ;
		else if(str.compare("11") == 0) return "B" ;
		else if(str.compare("12") == 0) return "C" ;
		else if(str.compare("13") == 0) return "D" ;
		else if(str.compare("14") == 0) return "E" ;
		else if(str.compare("15") == 0) return "F" ;
		else if(str.compare("0") == 0) return "0" ;
		else return "0" ;
	}//str
	
	void process_TYPE1(tokenTable &tokenTable){
		int instr_tableNo =  tokenTable.token_group.instr.tableNo -1 ;
		if(instr_tableNo == 11){ //FIX
		    if(!tokenTable.isTranslate){
		    	tokenTable.loc = LOCCTR ;
				tokenTable.machine_code = "C4" ;
				tokenTable.isTranslate = true ;
			}//if		    
			LOCCTR = LOCCTR + 1 ;
		}//if
		else if(instr_tableNo == 12){ //FLOAT
			if(!tokenTable.isTranslate){
				tokenTable.loc = LOCCTR ;
				tokenTable.machine_code = "C0" ;
				tokenTable.isTranslate = true ;
			}//if			
			LOCCTR = LOCCTR + 1 ;
		}//if
		else if(instr_tableNo == 13){ //HIO
			if(!tokenTable.isTranslate){
				tokenTable.loc = LOCCTR ;
				tokenTable.machine_code = "F4" ;
				tokenTable.isTranslate = true ;
			}//if			
			LOCCTR = LOCCTR + 1 ;
		}//if
		else if(instr_tableNo == 31){ //NORM
			if(!tokenTable.isTranslate){
				tokenTable.loc = LOCCTR ;
				tokenTable.machine_code = "C8" ;
				tokenTable.isTranslate = true ;
			}//if			
			LOCCTR = LOCCTR + 1 ;
		}//if
		else if(instr_tableNo == 38){ //SIO
			if(!tokenTable.isTranslate){
				tokenTable.loc = LOCCTR ;
				tokenTable.machine_code = "F0" ;
				tokenTable.isTranslate = true ;
			}//if			
			LOCCTR = LOCCTR + 1 ;
		}//if
		else if(instr_tableNo == 55){ //TIO
			if(!tokenTable.isTranslate){
				tokenTable.loc = LOCCTR ;
				tokenTable.machine_code = "F8" ;
				tokenTable.isTranslate = true ;
			}//if			
			LOCCTR = LOCCTR + 1 ;
		}//if
		else{
			if(!tokenTable.isTranslate){
				tokenTable.loc = LOCCTR ;
				tokenTable.machine_code = "00" ;
				tokenTable.isTranslate = true ;
			}//if			
			LOCCTR = LOCCTR + 1 ;
		}//else
	}//process_TYPE1()
	
	
	void process_TYPE2(tokenTable &tokenTable){
		int instr_tableNo =  tokenTable.token_group.instr.tableNo -1 ;
		int op1_tableNo = 0 ;
		int op2_tableNo = 0 ;
			
		string reg1 = "" ;
		string reg2 = "" ;
		string opcode = "" ;
		
		if(instr_tableNo == 2) { //ADDR
		
		    if(!tokenTable.isTranslate){
		    	op1_tableNo = tokenTable.token_group.operandList[0].outList[0].tableNo -1 ;
			    op2_tableNo = tokenTable.token_group.operandList[1].outList[0].tableNo -1 ;
				opcode = "90" ;
				reg1 = intToString(op1_tableNo) ;
				reg2 = intToString(op2_tableNo) ;
				
				tokenTable.machine_code = opcode+reg1+reg2 ;
				tokenTable.loc = LOCCTR ;
				tokenTable.isTranslate = true ;
			}//if		    
			LOCCTR = LOCCTR+ 2 ;
		}//if
		
		else if(instr_tableNo == 4){ //CLEAR
			if(!tokenTable.isTranslate){
				op1_tableNo = tokenTable.token_group.operandList[0].outList[0].tableNo -1 ;
				opcode = "B4" ;
				reg1 = intToString(op1_tableNo) ;
				reg2 = "0" ;
				
				tokenTable.machine_code = opcode+reg1+reg2 ;
				tokenTable.loc = LOCCTR ;
				tokenTable.isTranslate = true ;
			}//if		    
			LOCCTR = LOCCTR+ 2 ;
		}//if
		
		else if(instr_tableNo == 7){ //COMPPR
			if(!tokenTable.isTranslate){
				op1_tableNo = tokenTable.token_group.operandList[0].outList[0].tableNo -1 ;
			    op2_tableNo = tokenTable.token_group.operandList[1].outList[0].tableNo -1 ;
				opcode = "A0" ;
				reg1 = intToString(op1_tableNo) ;
				reg2 = intToString(op2_tableNo) ;
				
				tokenTable.machine_code = opcode+reg1+reg2 ;
				tokenTable.loc = LOCCTR ;
				tokenTable.isTranslate = true ;
			}//if
		
			LOCCTR = LOCCTR+ 2 ;
		}//if
		else if(instr_tableNo == 10){ //DIVR
			if(!tokenTable.isTranslate){
				op1_tableNo = tokenTable.token_group.operandList[0].outList[0].tableNo -1 ;
			    op2_tableNo = tokenTable.token_group.operandList[1].outList[0].tableNo -1 ;
				opcode = "9C" ;
				reg1 = intToString(op1_tableNo) ;
				reg2 = intToString(op2_tableNo) ;
				
				tokenTable.machine_code = opcode+reg1+reg2 ;
				tokenTable.loc = LOCCTR ;
				tokenTable.isTranslate = true ;
			}//if			
			LOCCTR = LOCCTR+ 2 ;
		}//if
		else if(instr_tableNo == 30){ //MULR
			if(!tokenTable.isTranslate){
				op1_tableNo = tokenTable.token_group.operandList[0].outList[0].tableNo -1 ;
			    op2_tableNo = tokenTable.token_group.operandList[1].outList[0].tableNo -1 ;
				opcode = "98" ;
				reg1 = intToString(op1_tableNo) ;
				reg2 = intToString(op2_tableNo) ;
				
				tokenTable.machine_code = opcode+reg1+reg2 ;
				tokenTable.loc = LOCCTR ;
				tokenTable.isTranslate = true ;
			}//if			
			LOCCTR = LOCCTR+ 2 ;
		}//if
		else if(instr_tableNo == 34){ //RMO
			if(!tokenTable.isTranslate){
				op1_tableNo = tokenTable.token_group.operandList[0].outList[0].tableNo -1 ;
			    op2_tableNo = tokenTable.token_group.operandList[1].outList[0].tableNo -1 ;
				opcode = "AC" ;
				reg1 = intToString(op1_tableNo) ;
				reg2 = intToString(op2_tableNo) ;
				
				tokenTable.machine_code = opcode+reg1+reg2 ;
				tokenTable.loc = LOCCTR ;
				tokenTable.isTranslate = true ;
			}//if			
			LOCCTR = LOCCTR+ 2 ;
		}//if
		else if(instr_tableNo == 36){ //SHIFTL
			if(!tokenTable.isTranslate){
				op1_tableNo = tokenTable.token_group.operandList[0].outList[0].tableNo -1 ;	    
				opcode = "A4" ;
				reg1 = intToString(op1_tableNo) ;
				reg2 = numChange_minus1(findToken(table6, tokenTable.token_group.operandList[1].outList[0].tableNo)) ;
							
				tokenTable.machine_code = opcode+reg1+reg2 ;
				tokenTable.loc = LOCCTR ;
				tokenTable.isTranslate = true ;
			}//if		    
			LOCCTR = LOCCTR+ 2 ;
		}//if
		else if(instr_tableNo == 37){ //SHIFTR
			if(!tokenTable.isTranslate){
				op1_tableNo = tokenTable.token_group.operandList[0].outList[0].tableNo -1 ;	 
				opcode = "A8" ;
				reg1 = intToString(op1_tableNo) ;
				reg2 = numChange_minus1(findToken(table6, tokenTable.token_group.operandList[1].outList[0].tableNo)) ;
				
				tokenTable.machine_code = opcode+reg1+reg2 ;
				tokenTable.loc = LOCCTR ;
				tokenTable.isTranslate = true ;
			}//if			
			LOCCTR = LOCCTR+ 2 ;
		}//if
		else if(instr_tableNo == 52){ //SUBR
			if(!tokenTable.isTranslate){
				op1_tableNo = tokenTable.token_group.operandList[0].outList[0].tableNo -1 ;
			    op2_tableNo = tokenTable.token_group.operandList[1].outList[0].tableNo -1 ;
				opcode = "94" ;
				reg1 = intToString(op1_tableNo) ;
				reg2 = intToString(op2_tableNo) ;
				
				tokenTable.machine_code = opcode+reg1+reg2 ;
				tokenTable.loc = LOCCTR ;
				tokenTable.isTranslate = true ;
			}//if			
			LOCCTR = LOCCTR+ 2 ;
		}//if
		else if(instr_tableNo == 53){ //SVC
 			if(!tokenTable.isTranslate){
				opcode = "B0" ;
				reg1 = findToken(table6, tokenTable.token_group.operandList[0].outList[0].tableNo) ;
				reg2 = "0" ;
				
				tokenTable.machine_code = opcode+reg1+reg2 ;
				tokenTable.loc = LOCCTR ;
				tokenTable.isTranslate = true ;
			}//if		
			LOCCTR = LOCCTR+ 2 ;
		}//if
		else if(instr_tableNo == 57){ //TIXR
			if(!tokenTable.isTranslate){
				op1_tableNo = tokenTable.token_group.operandList[0].outList[0].tableNo -1 ;
				op2_tableNo = tokenTable.token_group.operandList[1].outList[0].tableNo -1 ;
				opcode = "B8" ;
				reg1 = intToString(op1_tableNo) ;
				reg2 = intToString(op2_tableNo) ;
				
				tokenTable.machine_code = opcode+reg1+reg2 ;
				tokenTable.loc = LOCCTR ;
				tokenTable.isTranslate = true ;
			}//if		
			LOCCTR = LOCCTR+ 2 ;
		}//if		
	}//process_TYPE2()
	
	string find_n_i_x_b_p_e(tokenTable tokenTable, unsigned int &disp){
		string n = "" ;
		string i = "" ;
		string x = "" ;
		string b = "" ;
		string p = "" ;
		string e = "" ;
		
		int operand_type = tokenTable.token_group.operandList[0].operand_type ;
		unsigned int pc  = 0 ;
		unsigned int address = 0 ;
	   
	    int tableNo = 0 ;
	    string str = "" ;
		
		if(!tokenTable.token_group.isType4) pc = LOCCTR + 3 ;
		else pc = LOCCTR + 4 ;
		
		if(operand_type == IMME_VALUE){
			n = "0" ;
			i = "1" ;
			x = "0" ;						
			if(!tokenTable.token_group.isType4) e = "0" ;
			else e = "1" ;
			
			if(tokenTable.token_group.operandList[0].outList[1].tableNum == 5){
				if(isInSymbolTable(tokenTable.token_group.operandList[0].outList[1].tableNo, address)){
					if(!tokenTable.token_group.isType4){
						if(int(address - pc) >= -2048 && int(address - pc) <= 2047){
							b = "0" ;
							p = "1" ;
							disp = address - pc ;
						}//if
						else{
							b = "1" ;
							p = "0" ;
							disp = address-base_reg.address ;
						}//else
					}//if
					else{
						b = "0" ;
						p = "0" ;
						disp = address ;
					}//else
				}//if
			}//if
			
			else{ 
				disp = stringToInt(findToken(table6, tokenTable.token_group.operandList[0].outList[1].tableNo)) ;
				b = "0" ;
				p = "0" ;					             
			}//else
		}//if
				
		else if(operand_type == VALUE){
			n = "1" ;
			i = "1" ;
			
			if(tokenTable.token_group.operandList.size() == 2) x = "1" ;
			else x = "0" ;
			
			str = findToken(table6, tokenTable.token_group.operandList[0].outList[0].tableNo ) ; 
			disp = stringToInt(str) ;
			b = "0" ;
			p = "0" ;
			if(!tokenTable.token_group.isType4){								
				e = "0" ;
			}//if
			
			else{
				e = "1" ;
			}//else						
		}//if
		
		else if(operand_type == MEMORY){
			n = "1" ;
			i = "1" ;
			
			if(tokenTable.token_group.operandList.size() == 2) x = "1" ;
			else x = "0" ;
			
			//indirect
			if(tokenTable.token_group.operandList[0].outList[0].tableNum == 4){
				isInSymbolTable(tokenTable.token_group.operandList[0].outList[1].tableNo, address ) ;
				i = "0" ;
			}
			else {
				isInSymbolTable(tokenTable.token_group.operandList[0].outList[0].tableNo, address ) ;				
			}//else
			
			if(!tokenTable.token_group.isType4){
				
				if(int(address - pc) >= -2048 && int(address - pc) <= 2047){
					disp = address - pc;
					b = "0" ;
					p = "1" ;
				}//if
				else{
					if(base_reg.address != -1){
						//cout << "address: " << hex << address << endl ;
						//cout << "base_add: " << hex << base_reg.address << endl ;
						disp = address-base_reg.address ;
						b = "1" ;
						p = "0" ;
					}//if
					
					else return "no translate" ;					
				}//else
				e = "0" ;
			}//if
			else{			
				disp = address ;
				b = "0" ;
				p = "0" ;
				e = "1" ;
		   }//else
		}//if
		
		else {
			disp = 0 ;
			return "000000" ;	
		}
		
		return n+i+x+b+p+e ;
	}//find_n_i_x_b_p_e()
	
	string findLiteral_n_i_x_b_p_e(tokenTable tokenTable, int tableNum, int tableNo, unsigned int &disp){
		string n = "" ;
		string i = "" ;
		string x = "" ;
		string b = "" ;
		string p = "" ;
		string e = "" ;
		
		unsigned int pc  = 0 ;
		unsigned int address = 0 ;	    
		int return_literalTable_No = 0 ;
		
		if(!tokenTable.token_group.isType4) pc = LOCCTR + 3 ;
		else pc = LOCCTR + 4 ;
		
		n = "1" ;
		i = "1" ;
			
		if(tokenTable.token_group.operandList.size() == 2) x = "1" ;
		else x = "0" ;
			
		isInLiteralTable(tableNum, tableNo, return_literalTable_No) ;
		address = LITTAB[return_literalTable_No].address ;
							
		if(!tokenTable.token_group.isType4){
			
			if(int(address - pc) >= -2048 && int(address - pc) <= 2047){
				disp = address - pc;
				b = "0" ;
				p = "1" ;
			}//if
			else{
				if(base_reg.address != -1){
					disp = address-base_reg.address ;
					b = "1" ;
					p = "0" ;
				}//if
				
				else return "no translate" ;					
			}//else
			e = "0" ;
		}//if
		else{			
			disp = address ;
			b = "0" ;
			p = "0" ;
			e = "1" ;
	   }//else		
		return n+i+x+b+p+e ;
	}//find_n_i_x_b_p_e()
	
	//判斷是否operand已被定義 
	bool isOperandDefine(tokenTable tokenTable){
	    unsigned int address ;	
		int tableNo = 0 ;	
		string str = "" ;
		if(tokenTable.token_group.operandList[0].operand_type == IMME_VALUE){ //第一個opernad為immediate value 
		    //label作為immediate value 
			if(tokenTable.token_group.operandList[0].outList[1].tableNum == 5){
				if(isInSymbolTable(tokenTable.token_group.operandList[0].outList[1].tableNo, address)) 
					return true ;										
			}//if
			
			//integer作為immediate value 
			else if(tokenTable.token_group.operandList[0].outList[1].tableNum == 6){
				return true ;
			}//if
			
		}//if
		
		else if(tokenTable.token_group.operandList[0].operand_type == VALUE) { //第一個opernad為value
			return true	;		
		}//if
		
		else if(tokenTable.token_group.operandList[0].operand_type == MEMORY) { //第一個opernad為memory
		    //indirect
			if(tokenTable.token_group.operandList[0].outList[0].tableNum == 4){
				if(isInSymbolTable(tokenTable.token_group.operandList[0].outList[1].tableNo, address)){
					return true ;
				}//if
			}//if
			
			//dirict
			else{
				if(isInSymbolTable(tokenTable.token_group.operandList[0].outList[0].tableNo, address)){
					return true ;
				}//if
			}//else						
		}//if
		return false;
	}//isOperandDefine()
	
    string change_disp_to_string( char e, int disp){
    	stringstream ss;
    	string str_result = "" ;
    	string str = "" ;
    	ss << hex << disp ;
    	str = ss.str();
    	
    	if(e == '0'){
    		if(str.size() > 3 ){
	    		str_result = str.substr(str.size()-3, 3) ;
			}//if
			else if(str.size() == 3){
				str_result = str ;
			}//if
			else if(str.size() == 2){
				str_result = "0"+str ;
			}//if
			else{
				str_result = "00"+str ;
			}//else
		}//if
		
		else{
			if(str.size() > 5 ){
	    		str_result = str.substr(str.size()-5, 3) ;
			}//if
			else if(str.size() == 5){
				str_result = str ;
			}//if
			else if(str.size() == 4){
				str_result = "0"+str ;
			}//if
			else if(str.size() == 3){
				str_result = "00"+str ;
			}//if
			else if(str.size() == 2){
				str_result = "000"+str ;
			}//if
			else{
				str_result = "0000"+str ;
			}//else
		}//else
    	
    	return str_result ;
	}//change_disp_to_string()
	
	string change16To2(string str){
		string str_result = "" ;
		for ( int i = 0 ; i < str.size() ; i++){
			if(str.at(i) == '0') str_result = str_result + "0000" ;
			else if(str.at(i) == '1') str_result = str_result + "0001" ;
			else if(str.at(i) == '2') str_result = str_result + "0010" ;
			else if(str.at(i) == '3') str_result = str_result + "0011" ;
			else if(str.at(i) == '4') str_result = str_result + "0100" ;
			else if(str.at(i) == '5') str_result = str_result + "0101" ;
			else if(str.at(i) == '6') str_result = str_result + "0110" ;
			else if(str.at(i) == '7') str_result = str_result + "0111" ;
			else if(str.at(i) == '8') str_result = str_result + "1000" ;
			else if(str.at(i) == '9') str_result = str_result + "1001" ;
			else if(str.at(i) == 'A' || str.at(i) == 'a') str_result = str_result + "1010" ;
			else if(str.at(i) == 'B' || str.at(i) == 'b') str_result = str_result + "1011" ;
			else if(str.at(i) == 'C' || str.at(i) == 'c') str_result = str_result + "1100" ;
			else if(str.at(i) == 'D' || str.at(i) == 'd') str_result = str_result + "1101" ;
			else if(str.at(i) == 'E' || str.at(i) == 'e') str_result = str_result + "1110" ;
			else if(str.at(i) == 'F' || str.at(i) == 'f') str_result = str_result + "1111" ;
		}//for
		
		return str_result ;
	}//change16To2()
	
	
	string change2To16(string str){
		if(str.compare("0000") == 0) return "0" ;
		else if(str.compare("0001") == 0) return "1" ;
		else if(str.compare("0010") == 0) return "2" ;
		else if(str.compare("0011") == 0) return "3" ;
		else if(str.compare("0100") == 0) return "4" ;
		else if(str.compare("0101") == 0) return "5" ;
		else if(str.compare("0110") == 0) return "6" ;
		else if(str.compare("0111") == 0) return "7" ;
		else if(str.compare("1000") == 0) return "8" ;
		else if(str.compare("1001") == 0) return "9" ;
		else if(str.compare("1010") == 0) return "A" ;
		else if(str.compare("1011") == 0) return "B" ;
		else if(str.compare("1100") == 0) return "C" ;
		else if(str.compare("1101") == 0) return "D" ;
		else if(str.compare("1110") == 0) return "E" ;
		else if(str.compare("1111") == 0) return "F" ;
	
	}//change2To16()
	
	string change_opcode_bits6(string str){
		string str1 = str.substr(0,4) ;		
		string str2 = str.substr(4,4) ;	
		string str3 = str.substr(8,4);				
		return change2To16(str1) + change2To16(str2) + change2To16(str3) ;	
	}//change_opcode_bits6()
	
	void process_TPYE3or4(tokenTable &tokenTable){
		
		int instr_tableNo =  tokenTable.token_group.instr.tableNo -1 ;
		
		int literal_tableNum = 0;
		int literal_tableNo = 0;
		int return_literalTable_No = 0 ;
	    string literal_str = "" ; 
		
		string opcode = "" ;
		unsigned int disp = 0 ;
		string disp_str = "" ;
		string bits_6 = "" ;
		
		int operand_type1 = 0 ;
		int literal_type = 0 ;
		
	    if(!tokenTable.token_group.operandList.empty()){
	    	operand_type1 = tokenTable.token_group.operandList[0].operand_type ;
	    	 /////////////////////////
	    	if(operand_type1 == LITERAL){
	    		if(tokenTable.token_group.operandList[0].outList[1].tableNum == 4){
	    			if(tokenTable.token_group.operandList[0].outList[2].tableNum == 6) literal_type = 1 ;
	    			else literal_type = 2 ;
	    			literal_tableNum = tokenTable.token_group.operandList[0].outList[2].tableNum ;
	    			literal_tableNo = tokenTable.token_group.operandList[0].outList[2].tableNo ;
				}//if
				else{
					literal_type = 3 ;
					literal_tableNum = tokenTable.token_group.operandList[0].outList[1].tableNum ;
	    			literal_tableNo = tokenTable.token_group.operandList[0].outList[1].tableNo ;
				}//else
			}//if
			//////////////////////////////////
	    	if(instr_tableNo == 0){//ADD		   
				opcode = "000110" ;	
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "180000";
					else tokenTable.machine_code = "18000000";
					tokenTable.isTranslate = true ;
				}//if	*/	
					if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					//若literal存在在LITTAB 
					if(isInLiteralTable(literal_tableNum, literal_tableNo, return_literalTable_No)){
						if(LITTAB[return_literalTable_No].hasAddress){
							bits_6 = findLiteral_n_i_x_b_p_e(tokenTable, literal_tableNum, literal_tableNo, disp) ;
							if(bits_6.compare("no translate") != 0){
								disp_str = change_disp_to_string( bits_6.at(5), disp) ;
								tokenTable.machine_code = change_opcode_bits6(opcode+bits_6) + disp_str ;
								tokenTable.isTranslate = true ;
							}//if							
						}//if	
					}//if					
					//若不存在，hash至LITTAB 
					else{
						if(literal_tableNum == 6) literal_str = findToken(table6,literal_tableNo) ;
						else literal_str = findToken(table7,literal_tableNo) ;						
						createLiteralHash(hashValue(countAscii(literal_str)), literal_tableNum, literal_tableNo, literal_type);
					}//else
				}//if															
			}//if
			else if(instr_tableNo == 1){//ADDF
				opcode = "010110" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "580000";
					else tokenTable.machine_code = "58000000";
					tokenTable.isTranslate = true ;
				}//if*/
					if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					//若literal存在在LITTAB 
					if(isInLiteralTable(literal_tableNum, literal_tableNo, return_literalTable_No)){
						if(LITTAB[return_literalTable_No].hasAddress){
							bits_6 = findLiteral_n_i_x_b_p_e(tokenTable, literal_tableNum, literal_tableNo, disp) ;
							if(bits_6.compare("no translate") != 0){
								disp_str = change_disp_to_string( bits_6.at(5), disp) ;
								tokenTable.machine_code = change_opcode_bits6(opcode+bits_6) + disp_str ;
								tokenTable.isTranslate = true ;
							}//if							
						}//if	
					}//if					
					//若不存在，hash至LITTAB 
					else{
						if(literal_tableNum == 6) literal_str = findToken(table6,literal_tableNo) ;
						else literal_str = findToken(table7,literal_tableNo) ;						
						createLiteralHash(hashValue(countAscii(literal_str)), literal_tableNum, literal_tableNo, literal_type);
					}//else
				}//if
			}//if
			else if(instr_tableNo == 3){//AND
				opcode = "010000" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "400000";
					else tokenTable.machine_code = "40000000";
					tokenTable.isTranslate = true ;
				}//if*/
					if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					//若literal存在在LITTAB 
					if(isInLiteralTable(literal_tableNum, literal_tableNo, return_literalTable_No)){
						if(LITTAB[return_literalTable_No].hasAddress){
							bits_6 = findLiteral_n_i_x_b_p_e(tokenTable, literal_tableNum, literal_tableNo, disp) ;
							if(bits_6.compare("no translate") != 0){
								disp_str = change_disp_to_string( bits_6.at(5), disp) ;
								tokenTable.machine_code = change_opcode_bits6(opcode+bits_6) + disp_str ;
								tokenTable.isTranslate = true ;
							}//if							
						}//if	
					}//if					
					//若不存在，hash至LITTAB 
					else{
						if(literal_tableNum == 6) literal_str = findToken(table6,literal_tableNo) ;
						else literal_str = findToken(table7,literal_tableNo) ;						
						createLiteralHash(hashValue(countAscii(literal_str)), literal_tableNum, literal_tableNo, literal_type);
					}//else
				}//if
			}//if
			else if(instr_tableNo == 5){//COMP
				opcode = "001010" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "280000";
					else tokenTable.machine_code = "28000000";
					tokenTable.isTranslate = true ;
				}//if*/
					if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					//若literal存在在LITTAB 
					if(isInLiteralTable(literal_tableNum, literal_tableNo, return_literalTable_No)){
						if(LITTAB[return_literalTable_No].hasAddress){
							bits_6 = findLiteral_n_i_x_b_p_e(tokenTable, literal_tableNum, literal_tableNo, disp) ;
							if(bits_6.compare("no translate") != 0){
								disp_str = change_disp_to_string( bits_6.at(5), disp) ;
								tokenTable.machine_code = change_opcode_bits6(opcode+bits_6) + disp_str ;
								tokenTable.isTranslate = true ;
							}//if							
						}//if	
					}//if					
					//若不存在，hash至LITTAB 
					else{
						if(literal_tableNum == 6) literal_str = findToken(table6,literal_tableNo) ;
						else literal_str = findToken(table7,literal_tableNo) ;						
						createLiteralHash(hashValue(countAscii(literal_str)), literal_tableNum, literal_tableNo, literal_type);
					}//else
				}//if
			}//if
			else if(instr_tableNo == 6){//COMPF
				opcode = "100010" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "880000";
					else tokenTable.machine_code = "88000000";
					tokenTable.isTranslate = true ;
				}//if*/
					if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					//若literal存在在LITTAB 
					if(isInLiteralTable(literal_tableNum, literal_tableNo, return_literalTable_No)){
						if(LITTAB[return_literalTable_No].hasAddress){
							bits_6 = findLiteral_n_i_x_b_p_e(tokenTable, literal_tableNum, literal_tableNo, disp) ;
							if(bits_6.compare("no translate") != 0){
								disp_str = change_disp_to_string( bits_6.at(5), disp) ;
								tokenTable.machine_code = change_opcode_bits6(opcode+bits_6) + disp_str ;
								tokenTable.isTranslate = true ;
							}//if							
						}//if	
					}//if					
					//若不存在，hash至LITTAB 
					else{
						if(literal_tableNum == 6) literal_str = findToken(table6,literal_tableNo) ;
						else literal_str = findToken(table7,literal_tableNo) ;						
						createLiteralHash(hashValue(countAscii(literal_str)), literal_tableNum, literal_tableNo, literal_type);
					}//else
				}//if
			}//if
			else if(instr_tableNo == 8){//DIV
				opcode = "001001" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "240000";
					else tokenTable.machine_code = "24000000";
					tokenTable.isTranslate = true ;
				}//if*/
					if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					//若literal存在在LITTAB 
					if(isInLiteralTable(literal_tableNum, literal_tableNo, return_literalTable_No)){
						if(LITTAB[return_literalTable_No].hasAddress){
							bits_6 = findLiteral_n_i_x_b_p_e(tokenTable, literal_tableNum, literal_tableNo, disp) ;
							if(bits_6.compare("no translate") != 0){
								disp_str = change_disp_to_string( bits_6.at(5), disp) ;
								tokenTable.machine_code = change_opcode_bits6(opcode+bits_6) + disp_str ;
								tokenTable.isTranslate = true ;
							}//if							
						}//if	
					}//if					
					//若不存在，hash至LITTAB 
					else{
						if(literal_tableNum == 6) literal_str = findToken(table6,literal_tableNo) ;
						else literal_str = findToken(table7,literal_tableNo) ;						
						createLiteralHash(hashValue(countAscii(literal_str)), literal_tableNum, literal_tableNo, literal_type);
					}//else
				}//if
			}//if
			else if(instr_tableNo == 9){//DIVF
				opcode = "011001" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "640000";
					else tokenTable.machine_code = "64000000";
					tokenTable.isTranslate = true ;
				}//if*/
					if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					//若literal存在在LITTAB 
					if(isInLiteralTable(literal_tableNum, literal_tableNo, return_literalTable_No)){
						if(LITTAB[return_literalTable_No].hasAddress){
							bits_6 = findLiteral_n_i_x_b_p_e(tokenTable, literal_tableNum, literal_tableNo, disp) ;
							if(bits_6.compare("no translate") != 0){
								disp_str = change_disp_to_string( bits_6.at(5), disp) ;
								tokenTable.machine_code = change_opcode_bits6(opcode+bits_6) + disp_str ;
								tokenTable.isTranslate = true ;
							}//if							
						}//if	
					}//if					
					//若不存在，hash至LITTAB 
					else{
						if(literal_tableNum == 6) literal_str = findToken(table6,literal_tableNo) ;
						else literal_str = findToken(table7,literal_tableNo) ;						
						createLiteralHash(hashValue(countAscii(literal_str)), literal_tableNum, literal_tableNo, literal_type);
					}//else
				}//if
			}//if
			else if(instr_tableNo == 14){//J
				opcode = "001111" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "3C0000";
					else tokenTable.machine_code = "3C000000";
					tokenTable.isTranslate = true ;
				}//if*/
					if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					//若literal存在在LITTAB 
					if(isInLiteralTable(literal_tableNum, literal_tableNo, return_literalTable_No)){
						if(LITTAB[return_literalTable_No].hasAddress){
							bits_6 = findLiteral_n_i_x_b_p_e(tokenTable, literal_tableNum, literal_tableNo, disp) ;
							if(bits_6.compare("no translate") != 0){
								disp_str = change_disp_to_string( bits_6.at(5), disp) ;
								tokenTable.machine_code = change_opcode_bits6(opcode+bits_6) + disp_str ;
								tokenTable.isTranslate = true ;
							}//if							
						}//if	
					}//if					
					//若不存在，hash至LITTAB 
					else{
						if(literal_tableNum == 6) literal_str = findToken(table6,literal_tableNo) ;
						else literal_str = findToken(table7,literal_tableNo) ;						
						createLiteralHash(hashValue(countAscii(literal_str)), literal_tableNum, literal_tableNo, literal_type);
					}//else
				}//if
			}//if
			else if(instr_tableNo == 15){//JEQ
				opcode = "001100" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "300000";
					else tokenTable.machine_code = "30000000";
					tokenTable.isTranslate = true ;
				}//if*/
					if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					//若literal存在在LITTAB 
					if(isInLiteralTable(literal_tableNum, literal_tableNo, return_literalTable_No)){
						if(LITTAB[return_literalTable_No].hasAddress){
							bits_6 = findLiteral_n_i_x_b_p_e(tokenTable, literal_tableNum, literal_tableNo, disp) ;
							if(bits_6.compare("no translate") != 0){
								disp_str = change_disp_to_string( bits_6.at(5), disp) ;
								tokenTable.machine_code = change_opcode_bits6(opcode+bits_6) + disp_str ;
								tokenTable.isTranslate = true ;
							}//if							
						}//if	
					}//if					
					//若不存在，hash至LITTAB 
					else{
						if(literal_tableNum == 6) literal_str = findToken(table6,literal_tableNo) ;
						else literal_str = findToken(table7,literal_tableNo) ;						
						createLiteralHash(hashValue(countAscii(literal_str)), literal_tableNum, literal_tableNo, literal_type);
					}//else
				}//if
			}//if
			else if(instr_tableNo == 16){//JGT
				opcode = "001101" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "340000";
					else tokenTable.machine_code = "34000000";
					tokenTable.isTranslate = true ;
				}//if*/
					if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					//若literal存在在LITTAB 
					if(isInLiteralTable(literal_tableNum, literal_tableNo, return_literalTable_No)){
						if(LITTAB[return_literalTable_No].hasAddress){
							bits_6 = findLiteral_n_i_x_b_p_e(tokenTable, literal_tableNum, literal_tableNo, disp) ;
							if(bits_6.compare("no translate") != 0){
								disp_str = change_disp_to_string( bits_6.at(5), disp) ;
								tokenTable.machine_code = change_opcode_bits6(opcode+bits_6) + disp_str ;
								tokenTable.isTranslate = true ;
							}//if							
						}//if	
					}//if					
					//若不存在，hash至LITTAB 
					else{
						if(literal_tableNum == 6) literal_str = findToken(table6,literal_tableNo) ;
						else literal_str = findToken(table7,literal_tableNo) ;						
						createLiteralHash(hashValue(countAscii(literal_str)), literal_tableNum, literal_tableNo, literal_type);
					}//else
				}//if
			}//if
			else if(instr_tableNo == 17){//JLT
				opcode = "001110" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "380000";
					else tokenTable.machine_code = "38000000";
					tokenTable.isTranslate = true ;
				}//if*/
					if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					//若literal存在在LITTAB 
					if(isInLiteralTable(literal_tableNum, literal_tableNo, return_literalTable_No)){
						if(LITTAB[return_literalTable_No].hasAddress){
							bits_6 = findLiteral_n_i_x_b_p_e(tokenTable, literal_tableNum, literal_tableNo, disp) ;
							if(bits_6.compare("no translate") != 0){
								disp_str = change_disp_to_string( bits_6.at(5), disp) ;
								tokenTable.machine_code = change_opcode_bits6(opcode+bits_6) + disp_str ;
								tokenTable.isTranslate = true ;
							}//if							
						}//if	
					}//if					
					//若不存在，hash至LITTAB 
					else{
						if(literal_tableNum == 6) literal_str = findToken(table6,literal_tableNo) ;
						else literal_str = findToken(table7,literal_tableNo) ;						
						createLiteralHash(hashValue(countAscii(literal_str)), literal_tableNum, literal_tableNo, literal_type);
					}//else
				}//if
			}//if
			else if(instr_tableNo == 18){//JSUB
				opcode = "010010" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "480000";
					else tokenTable.machine_code = "48000000";
					tokenTable.isTranslate = true ;
				}//if*/
					if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					//若literal存在在LITTAB 
					if(isInLiteralTable(literal_tableNum, literal_tableNo, return_literalTable_No)){
						if(LITTAB[return_literalTable_No].hasAddress){
							bits_6 = findLiteral_n_i_x_b_p_e(tokenTable, literal_tableNum, literal_tableNo, disp) ;
							if(bits_6.compare("no translate") != 0){
								disp_str = change_disp_to_string( bits_6.at(5), disp) ;
								tokenTable.machine_code = change_opcode_bits6(opcode+bits_6) + disp_str ;
								tokenTable.isTranslate = true ;
							}//if							
						}//if	
					}//if					
					//若不存在，hash至LITTAB 
					else{
						if(literal_tableNum == 6) literal_str = findToken(table6,literal_tableNo) ;
						else literal_str = findToken(table7,literal_tableNo) ;						
						createLiteralHash(hashValue(countAscii(literal_str)), literal_tableNum, literal_tableNo, literal_type);
					}//else
				}//if
			}//if
			else if(instr_tableNo == 19){//LDA
				opcode = "000000" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "000000";
					else tokenTable.machine_code = "00000000";
					tokenTable.isTranslate = true ;
				}//if*/
				if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					//若literal存在在LITTAB 
					if(isInLiteralTable(literal_tableNum, literal_tableNo, return_literalTable_No)){
						if(LITTAB[return_literalTable_No].hasAddress){
							bits_6 = findLiteral_n_i_x_b_p_e(tokenTable, literal_tableNum, literal_tableNo, disp) ;
							if(bits_6.compare("no translate") != 0){
								disp_str = change_disp_to_string( bits_6.at(5), disp) ;
								tokenTable.machine_code = change_opcode_bits6(opcode+bits_6) + disp_str ;
								tokenTable.isTranslate = true ;
							}//if							
						}//if	
					}//if					
					//若不存在，hash至LITTAB 
					else{
						
						if(literal_tableNum == 6) literal_str = findToken(table6,literal_tableNo) ;
						else literal_str = findToken(table7,literal_tableNo) ;		
						//cout << "literal_str : " << literal_str << endl ;				
						createLiteralHash(hashValue(countAscii(literal_str)), literal_tableNum, literal_tableNo, literal_type);
					}//else
				}//if
			}//if
			else if(instr_tableNo == 20){//LDB
				opcode = "011010" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "680000";
					else tokenTable.machine_code = "68000000";
					tokenTable.isTranslate = true ;
				}//if*/
					if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					//若literal存在在LITTAB 
					if(isInLiteralTable(literal_tableNum, literal_tableNo, return_literalTable_No)){
						if(LITTAB[return_literalTable_No].hasAddress){
							bits_6 = findLiteral_n_i_x_b_p_e(tokenTable, literal_tableNum, literal_tableNo, disp) ;
							if(bits_6.compare("no translate") != 0){
								disp_str = change_disp_to_string( bits_6.at(5), disp) ;
								tokenTable.machine_code = change_opcode_bits6(opcode+bits_6) + disp_str ;
								tokenTable.isTranslate = true ;
							}//if							
						}//if	
					}//if					
					//若不存在，hash至LITTAB 
					else{
						if(literal_tableNum == 6) literal_str = findToken(table6,literal_tableNo) ;
						else literal_str = findToken(table7,literal_tableNo) ;						
						createLiteralHash(hashValue(countAscii(literal_str)), literal_tableNum, literal_tableNo, literal_type);
					}//else
				}//if
			}//if
			else if(instr_tableNo == 21){//LDCH
				opcode = "010100" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "500000";
					else tokenTable.machine_code = "50000000";
					tokenTable.isTranslate = true ;
				}//if*/
					if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					//若literal存在在LITTAB 
					if(isInLiteralTable(literal_tableNum, literal_tableNo, return_literalTable_No)){
						if(LITTAB[return_literalTable_No].hasAddress){
							bits_6 = findLiteral_n_i_x_b_p_e(tokenTable, literal_tableNum, literal_tableNo, disp) ;
							if(bits_6.compare("no translate") != 0){
								disp_str = change_disp_to_string( bits_6.at(5), disp) ;
								tokenTable.machine_code = change_opcode_bits6(opcode+bits_6) + disp_str ;
								tokenTable.isTranslate = true ;
							}//if							
						}//if	
					}//if					
					//若不存在，hash至LITTAB 
					else{
						if(literal_tableNum == 6) literal_str = findToken(table6,literal_tableNo) ;
						else literal_str = findToken(table7,literal_tableNo) ;						
						createLiteralHash(hashValue(countAscii(literal_str)), literal_tableNum, literal_tableNo, literal_type);
					}//else
				}//if
			}//if
			else if(instr_tableNo == 22){//LDF
				opcode = "011100" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "700000";
					else tokenTable.machine_code = "70000000";
					tokenTable.isTranslate = true ;
				}//if*/
					if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					//若literal存在在LITTAB 
					if(isInLiteralTable(literal_tableNum, literal_tableNo, return_literalTable_No)){
						if(LITTAB[return_literalTable_No].hasAddress){
							bits_6 = findLiteral_n_i_x_b_p_e(tokenTable, literal_tableNum, literal_tableNo, disp) ;
							if(bits_6.compare("no translate") != 0){
								disp_str = change_disp_to_string( bits_6.at(5), disp) ;
								tokenTable.machine_code = change_opcode_bits6(opcode+bits_6) + disp_str ;
								tokenTable.isTranslate = true ;
							}//if							
						}//if	
					}//if					
					//若不存在，hash至LITTAB 
					else{
						if(literal_tableNum == 6) literal_str = findToken(table6,literal_tableNo) ;
						else literal_str = findToken(table7,literal_tableNo) ;						
						createLiteralHash(hashValue(countAscii(literal_str)), literal_tableNum, literal_tableNo, literal_type);
					}//else
				}//if
			}//if
			else if(instr_tableNo == 23){//LDL
				opcode = "000010" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "080000";
					else tokenTable.machine_code = "08000000";
					tokenTable.isTranslate = true ;
				}//if*/
					if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					//若literal存在在LITTAB 
					if(isInLiteralTable(literal_tableNum, literal_tableNo, return_literalTable_No)){
						if(LITTAB[return_literalTable_No].hasAddress){
							bits_6 = findLiteral_n_i_x_b_p_e(tokenTable, literal_tableNum, literal_tableNo, disp) ;
							if(bits_6.compare("no translate") != 0){
								disp_str = change_disp_to_string( bits_6.at(5), disp) ;
								tokenTable.machine_code = change_opcode_bits6(opcode+bits_6) + disp_str ;
								tokenTable.isTranslate = true ;
							}//if							
						}//if	
					}//if					
					//若不存在，hash至LITTAB 
					else{
						if(literal_tableNum == 6) literal_str = findToken(table6,literal_tableNo) ;
						else literal_str = findToken(table7,literal_tableNo) ;						
						createLiteralHash(hashValue(countAscii(literal_str)), literal_tableNum, literal_tableNo, literal_type);
					}//else
				}//if
			}//if
			else if(instr_tableNo == 24){//LDS
				opcode = "011011" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "6C0000";
					else tokenTable.machine_code = "6C000000";
					tokenTable.isTranslate = true ;
				}//if*/
					if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					//若literal存在在LITTAB 
					if(isInLiteralTable(literal_tableNum, literal_tableNo, return_literalTable_No)){
						if(LITTAB[return_literalTable_No].hasAddress){
							bits_6 = findLiteral_n_i_x_b_p_e(tokenTable, literal_tableNum, literal_tableNo, disp) ;
							if(bits_6.compare("no translate") != 0){
								disp_str = change_disp_to_string( bits_6.at(5), disp) ;
								tokenTable.machine_code = change_opcode_bits6(opcode+bits_6) + disp_str ;
								tokenTable.isTranslate = true ;
							}//if							
						}//if	
					}//if					
					//若不存在，hash至LITTAB 
					else{
						if(literal_tableNum == 6) literal_str = findToken(table6,literal_tableNo) ;
						else literal_str = findToken(table7,literal_tableNo) ;						
						createLiteralHash(hashValue(countAscii(literal_str)), literal_tableNum, literal_tableNo, literal_type);
					}//else
				}//if
			}//if
			else if(instr_tableNo == 25){//LDT
				opcode = "011101" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "740000";
					else tokenTable.machine_code = "74000000";
					tokenTable.isTranslate = true ;
				}//if*/
					if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					//若literal存在在LITTAB 
					if(isInLiteralTable(literal_tableNum, literal_tableNo, return_literalTable_No)){
						if(LITTAB[return_literalTable_No].hasAddress){
							bits_6 = findLiteral_n_i_x_b_p_e(tokenTable, literal_tableNum, literal_tableNo, disp) ;
							if(bits_6.compare("no translate") != 0){
								disp_str = change_disp_to_string( bits_6.at(5), disp) ;
								tokenTable.machine_code = change_opcode_bits6(opcode+bits_6) + disp_str ;
								tokenTable.isTranslate = true ;
							}//if							
						}//if	
					}//if					
					//若不存在，hash至LITTAB 
					else{
						if(literal_tableNum == 6) literal_str = findToken(table6,literal_tableNo) ;
						else literal_str = findToken(table7,literal_tableNo) ;						
						createLiteralHash(hashValue(countAscii(literal_str)), literal_tableNum, literal_tableNo, literal_type);
					}//else
				}//if
			}//if
			else if(instr_tableNo == 26){//LDX
				opcode = "000001" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "040000";
					else tokenTable.machine_code = "04000000";
					tokenTable.isTranslate = true ;
				}//if*/
					if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					//若literal存在在LITTAB 
					if(isInLiteralTable(literal_tableNum, literal_tableNo, return_literalTable_No)){
						if(LITTAB[return_literalTable_No].hasAddress){
							bits_6 = findLiteral_n_i_x_b_p_e(tokenTable, literal_tableNum, literal_tableNo, disp) ;
							if(bits_6.compare("no translate") != 0){
								disp_str = change_disp_to_string( bits_6.at(5), disp) ;
								tokenTable.machine_code = change_opcode_bits6(opcode+bits_6) + disp_str ;
								tokenTable.isTranslate = true ;
							}//if							
						}//if	
					}//if					
					//若不存在，hash至LITTAB 
					else{
						if(literal_tableNum == 6) literal_str = findToken(table6,literal_tableNo) ;
						else literal_str = findToken(table7,literal_tableNo) ;						
						createLiteralHash(hashValue(countAscii(literal_str)), literal_tableNum, literal_tableNo, literal_type);
					}//else
				}//if
			}//if
			else if(instr_tableNo == 27){//LPS
				opcode = "110100" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "D00000";
					else tokenTable.machine_code = "D0000000";
					tokenTable.isTranslate = true ;
				}//if*/
					if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					//若literal存在在LITTAB 
					if(isInLiteralTable(literal_tableNum, literal_tableNo, return_literalTable_No)){
						if(LITTAB[return_literalTable_No].hasAddress){
							bits_6 = findLiteral_n_i_x_b_p_e(tokenTable, literal_tableNum, literal_tableNo, disp) ;
							if(bits_6.compare("no translate") != 0){
								disp_str = change_disp_to_string( bits_6.at(5), disp) ;
								tokenTable.machine_code = change_opcode_bits6(opcode+bits_6) + disp_str ;
								tokenTable.isTranslate = true ;
							}//if							
						}//if	
					}//if					
					//若不存在，hash至LITTAB 
					else{
						if(literal_tableNum == 6) literal_str = findToken(table6,literal_tableNo) ;
						else literal_str = findToken(table7,literal_tableNo) ;						
						createLiteralHash(hashValue(countAscii(literal_str)), literal_tableNum, literal_tableNo, literal_type);
					}//else
				}//if
			}//if
			else if(instr_tableNo == 28){//MUL
				opcode = "001000" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "200000";
					else tokenTable.machine_code = "20000000";
					tokenTable.isTranslate = true ;
				}//if*/
					if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					//若literal存在在LITTAB 
					if(isInLiteralTable(literal_tableNum, literal_tableNo, return_literalTable_No)){
						if(LITTAB[return_literalTable_No].hasAddress){
							bits_6 = findLiteral_n_i_x_b_p_e(tokenTable, literal_tableNum, literal_tableNo, disp) ;
							if(bits_6.compare("no translate") != 0){
								disp_str = change_disp_to_string( bits_6.at(5), disp) ;
								tokenTable.machine_code = change_opcode_bits6(opcode+bits_6) + disp_str ;
								tokenTable.isTranslate = true ;
							}//if							
						}//if	
					}//if					
					//若不存在，hash至LITTAB 
					else{
						if(literal_tableNum == 6) literal_str = findToken(table6,literal_tableNo) ;
						else literal_str = findToken(table7,literal_tableNo) ;						
						createLiteralHash(hashValue(countAscii(literal_str)), literal_tableNum, literal_tableNo, literal_type);
					}//else
				}//if
			}//if
			else if(instr_tableNo == 29){//MULF
				opcode = "011000" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "600000";
					else tokenTable.machine_code = "60000000";
					tokenTable.isTranslate = true ;
				}//if*/
					if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					//若literal存在在LITTAB 
					if(isInLiteralTable(literal_tableNum, literal_tableNo, return_literalTable_No)){
						if(LITTAB[return_literalTable_No].hasAddress){
							bits_6 = findLiteral_n_i_x_b_p_e(tokenTable, literal_tableNum, literal_tableNo, disp) ;
							if(bits_6.compare("no translate") != 0){
								disp_str = change_disp_to_string( bits_6.at(5), disp) ;
								tokenTable.machine_code = change_opcode_bits6(opcode+bits_6) + disp_str ;
								tokenTable.isTranslate = true ;
							}//if							
						}//if	
					}//if					
					//若不存在，hash至LITTAB 
					else{
						if(literal_tableNum == 6) literal_str = findToken(table6,literal_tableNo) ;
						else literal_str = findToken(table7,literal_tableNo) ;						
						createLiteralHash(hashValue(countAscii(literal_str)), literal_tableNum, literal_tableNo, literal_type);
					}//else
				}//if
			}//if
			else if(instr_tableNo == 32){//OR
				opcode = "010001" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "440000";
					else tokenTable.machine_code = "44000000";
					tokenTable.isTranslate = true ;
				}//if*/
					if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					//若literal存在在LITTAB 
					if(isInLiteralTable(literal_tableNum, literal_tableNo, return_literalTable_No)){
						if(LITTAB[return_literalTable_No].hasAddress){
							bits_6 = findLiteral_n_i_x_b_p_e(tokenTable, literal_tableNum, literal_tableNo, disp) ;
							if(bits_6.compare("no translate") != 0){
								disp_str = change_disp_to_string( bits_6.at(5), disp) ;
								tokenTable.machine_code = change_opcode_bits6(opcode+bits_6) + disp_str ;
								tokenTable.isTranslate = true ;
							}//if							
						}//if	
					}//if					
					//若不存在，hash至LITTAB 
					else{
						if(literal_tableNum == 6) literal_str = findToken(table6,literal_tableNo) ;
						else literal_str = findToken(table7,literal_tableNo) ;						
						createLiteralHash(hashValue(countAscii(literal_str)), literal_tableNum, literal_tableNo, literal_type);
					}//else
				}//if
			}//if
			else if(instr_tableNo == 33){//RD
				opcode = "110110" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "D80000";
					else tokenTable.machine_code = "D8000000";
					tokenTable.isTranslate = true ;
				}//if*/
					if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					//若literal存在在LITTAB 
					if(isInLiteralTable(literal_tableNum, literal_tableNo, return_literalTable_No)){
						if(LITTAB[return_literalTable_No].hasAddress){
							bits_6 = findLiteral_n_i_x_b_p_e(tokenTable, literal_tableNum, literal_tableNo, disp) ;
							if(bits_6.compare("no translate") != 0){
								disp_str = change_disp_to_string( bits_6.at(5), disp) ;
								tokenTable.machine_code = change_opcode_bits6(opcode+bits_6) + disp_str ;
								tokenTable.isTranslate = true ;
							}//if							
						}//if	
					}//if					
					//若不存在，hash至LITTAB 
					else{
						if(literal_tableNum == 6) literal_str = findToken(table6,literal_tableNo) ;
						else literal_str = findToken(table7,literal_tableNo) ;						
						createLiteralHash(hashValue(countAscii(literal_str)), literal_tableNum, literal_tableNo, literal_type);
					}//else
				}//if
			}//if
			else if(instr_tableNo == 35){//RSUB
				opcode = "010011" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "4C0000";
					else tokenTable.machine_code = "4C000000";
					tokenTable.isTranslate = true ;
				}//if*/
					if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					//若literal存在在LITTAB 
					if(isInLiteralTable(literal_tableNum, literal_tableNo, return_literalTable_No)){
						if(LITTAB[return_literalTable_No].hasAddress){
							bits_6 = findLiteral_n_i_x_b_p_e(tokenTable, literal_tableNum, literal_tableNo, disp) ;
							if(bits_6.compare("no translate") != 0){
								disp_str = change_disp_to_string( bits_6.at(5), disp) ;
								tokenTable.machine_code = change_opcode_bits6(opcode+bits_6) + disp_str ;
								tokenTable.isTranslate = true ;
							}//if							
						}//if	
					}//if					
					//若不存在，hash至LITTAB 
					else{
						if(literal_tableNum == 6) literal_str = findToken(table6,literal_tableNo) ;
						else literal_str = findToken(table7,literal_tableNo) ;						
						createLiteralHash(hashValue(countAscii(literal_str)), literal_tableNum, literal_tableNo, literal_type);
					}//else
				}//if
			}//if
			else if(instr_tableNo == 39){//SSK
				opcode = "111011" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "EC0000";
					else tokenTable.machine_code = "EC000000";
					tokenTable.isTranslate = true ;
				}//if*/
					if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					//若literal存在在LITTAB 
					if(isInLiteralTable(literal_tableNum, literal_tableNo, return_literalTable_No)){
						if(LITTAB[return_literalTable_No].hasAddress){
							bits_6 = findLiteral_n_i_x_b_p_e(tokenTable, literal_tableNum, literal_tableNo, disp) ;
							if(bits_6.compare("no translate") != 0){
								disp_str = change_disp_to_string( bits_6.at(5), disp) ;
								tokenTable.machine_code = change_opcode_bits6(opcode+bits_6) + disp_str ;
								tokenTable.isTranslate = true ;
							}//if							
						}//if	
					}//if					
					//若不存在，hash至LITTAB 
					else{
						if(literal_tableNum == 6) literal_str = findToken(table6,literal_tableNo) ;
						else literal_str = findToken(table7,literal_tableNo) ;						
						createLiteralHash(hashValue(countAscii(literal_str)), literal_tableNum, literal_tableNo, literal_type);
					}//else
				}//if
			}//if
			else if(instr_tableNo == 40){//STA
				opcode = "000011" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "0C0000";
					else tokenTable.machine_code = "0C000000";
					tokenTable.isTranslate = true ;
				}//if*/
					if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					//若literal存在在LITTAB 
					if(isInLiteralTable(literal_tableNum, literal_tableNo, return_literalTable_No)){
						if(LITTAB[return_literalTable_No].hasAddress){
							bits_6 = findLiteral_n_i_x_b_p_e(tokenTable, literal_tableNum, literal_tableNo, disp) ;
							if(bits_6.compare("no translate") != 0){
								disp_str = change_disp_to_string( bits_6.at(5), disp) ;
								tokenTable.machine_code = change_opcode_bits6(opcode+bits_6) + disp_str ;
								tokenTable.isTranslate = true ;
							}//if							
						}//if	
					}//if					
					//若不存在，hash至LITTAB 
					else{
						if(literal_tableNum == 6) literal_str = findToken(table6,literal_tableNo) ;
						else literal_str = findToken(table7,literal_tableNo) ;						
						createLiteralHash(hashValue(countAscii(literal_str)), literal_tableNum, literal_tableNo, literal_type);
					}//else
				}//if
			}//if
			else if(instr_tableNo == 41){//STB
				opcode = "011110" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "780000";
					else tokenTable.machine_code = "78000000";
					tokenTable.isTranslate = true ;
				}//if*/
					if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					//若literal存在在LITTAB 
					if(isInLiteralTable(literal_tableNum, literal_tableNo, return_literalTable_No)){
						if(LITTAB[return_literalTable_No].hasAddress){
							bits_6 = findLiteral_n_i_x_b_p_e(tokenTable, literal_tableNum, literal_tableNo, disp) ;
							if(bits_6.compare("no translate") != 0){
								disp_str = change_disp_to_string( bits_6.at(5), disp) ;
								tokenTable.machine_code = change_opcode_bits6(opcode+bits_6) + disp_str ;
								tokenTable.isTranslate = true ;
							}//if							
						}//if	
					}//if					
					//若不存在，hash至LITTAB 
					else{
						if(literal_tableNum == 6) literal_str = findToken(table6,literal_tableNo) ;
						else literal_str = findToken(table7,literal_tableNo) ;						
						createLiteralHash(hashValue(countAscii(literal_str)), literal_tableNum, literal_tableNo, literal_type);
					}//else
				}//if
			}//if
			else if(instr_tableNo == 42){//STCH
				opcode = "010101" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "540000";
					else tokenTable.machine_code = "54000000";
					tokenTable.isTranslate = true ;
				}//if*/
					if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					//若literal存在在LITTAB 
					if(isInLiteralTable(literal_tableNum, literal_tableNo, return_literalTable_No)){
						if(LITTAB[return_literalTable_No].hasAddress){
							bits_6 = findLiteral_n_i_x_b_p_e(tokenTable, literal_tableNum, literal_tableNo, disp) ;
							if(bits_6.compare("no translate") != 0){
								disp_str = change_disp_to_string( bits_6.at(5), disp) ;
								tokenTable.machine_code = change_opcode_bits6(opcode+bits_6) + disp_str ;
								tokenTable.isTranslate = true ;
							}//if							
						}//if	
					}//if					
					//若不存在，hash至LITTAB 
					else{
						if(literal_tableNum == 6) literal_str = findToken(table6,literal_tableNo) ;
						else literal_str = findToken(table7,literal_tableNo) ;						
						createLiteralHash(hashValue(countAscii(literal_str)), literal_tableNum, literal_tableNo, literal_type);
					}//else
				}//if
			}//if
			else if(instr_tableNo == 43){//STF
				opcode = "100000" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "800000";
					else tokenTable.machine_code = "80000000";
					tokenTable.isTranslate = true ;
				}//if*/
					if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					//若literal存在在LITTAB 
					if(isInLiteralTable(literal_tableNum, literal_tableNo, return_literalTable_No)){
						if(LITTAB[return_literalTable_No].hasAddress){
							bits_6 = findLiteral_n_i_x_b_p_e(tokenTable, literal_tableNum, literal_tableNo, disp) ;
							if(bits_6.compare("no translate") != 0){
								disp_str = change_disp_to_string( bits_6.at(5), disp) ;
								tokenTable.machine_code = change_opcode_bits6(opcode+bits_6) + disp_str ;
								tokenTable.isTranslate = true ;
							}//if							
						}//if	
					}//if					
					//若不存在，hash至LITTAB 
					else{
						if(literal_tableNum == 6) literal_str = findToken(table6,literal_tableNo) ;
						else literal_str = findToken(table7,literal_tableNo) ;						
						createLiteralHash(hashValue(countAscii(literal_str)), literal_tableNum, literal_tableNo, literal_type);
					}//else
				}//if
			}//if
			else if(instr_tableNo == 44){//STI
				opcode = "110101" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "E40000";
					else tokenTable.machine_code = "E4000000";
					tokenTable.isTranslate = true ;
				}//if*/
					if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					//若literal存在在LITTAB 
					if(isInLiteralTable(literal_tableNum, literal_tableNo, return_literalTable_No)){
						if(LITTAB[return_literalTable_No].hasAddress){
							bits_6 = findLiteral_n_i_x_b_p_e(tokenTable, literal_tableNum, literal_tableNo, disp) ;
							if(bits_6.compare("no translate") != 0){
								disp_str = change_disp_to_string( bits_6.at(5), disp) ;
								tokenTable.machine_code = change_opcode_bits6(opcode+bits_6) + disp_str ;
								tokenTable.isTranslate = true ;
							}//if							
						}//if	
					}//if					
					//若不存在，hash至LITTAB 
					else{
						if(literal_tableNum == 6) literal_str = findToken(table6,literal_tableNo) ;
						else literal_str = findToken(table7,literal_tableNo) ;						
						createLiteralHash(hashValue(countAscii(literal_str)), literal_tableNum, literal_tableNo, literal_type);
					}//else
				}//if
			}//if
			else if(instr_tableNo == 45){//STL
				opcode = "000101" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "140000";
					else tokenTable.machine_code = "14000000";
					tokenTable.isTranslate = true ;
				}//if*/
					if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					//若literal存在在LITTAB 
					if(isInLiteralTable(literal_tableNum, literal_tableNo, return_literalTable_No)){
						if(LITTAB[return_literalTable_No].hasAddress){
							bits_6 = findLiteral_n_i_x_b_p_e(tokenTable, literal_tableNum, literal_tableNo, disp) ;
							if(bits_6.compare("no translate") != 0){
								disp_str = change_disp_to_string( bits_6.at(5), disp) ;
								tokenTable.machine_code = change_opcode_bits6(opcode+bits_6) + disp_str ;
								tokenTable.isTranslate = true ;
							}//if							
						}//if	
					}//if					
					//若不存在，hash至LITTAB 
					else{
						if(literal_tableNum == 6) literal_str = findToken(table6,literal_tableNo) ;
						else literal_str = findToken(table7,literal_tableNo) ;						
						createLiteralHash(hashValue(countAscii(literal_str)), literal_tableNum, literal_tableNo, literal_type);
					}//else
				}//if
			}//if
			else if(instr_tableNo == 46){//STS
				opcode = "011111" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "7C0000";
					else tokenTable.machine_code = "7C000000";
					tokenTable.isTranslate = true ;
				}//if*/
					if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					//若literal存在在LITTAB 
					if(isInLiteralTable(literal_tableNum, literal_tableNo, return_literalTable_No)){
						if(LITTAB[return_literalTable_No].hasAddress){
							bits_6 = findLiteral_n_i_x_b_p_e(tokenTable, literal_tableNum, literal_tableNo, disp) ;
							if(bits_6.compare("no translate") != 0){
								disp_str = change_disp_to_string( bits_6.at(5), disp) ;
								tokenTable.machine_code = change_opcode_bits6(opcode+bits_6) + disp_str ;
								tokenTable.isTranslate = true ;
							}//if							
						}//if	
					}//if					
					//若不存在，hash至LITTAB 
					else{
						if(literal_tableNum == 6) literal_str = findToken(table6,literal_tableNo) ;
						else literal_str = findToken(table7,literal_tableNo) ;						
						createLiteralHash(hashValue(countAscii(literal_str)), literal_tableNum, literal_tableNo, literal_type);
					}//else
				}//if
			}//if
			else if(instr_tableNo == 47){//STSW
				opcode = "111010" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "E80000";
					else tokenTable.machine_code = "E8000000";
					tokenTable.isTranslate = true ;
				}//if*/
					if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					//若literal存在在LITTAB 
					if(isInLiteralTable(literal_tableNum, literal_tableNo, return_literalTable_No)){
						if(LITTAB[return_literalTable_No].hasAddress){
							bits_6 = findLiteral_n_i_x_b_p_e(tokenTable, literal_tableNum, literal_tableNo, disp) ;
							if(bits_6.compare("no translate") != 0){
								disp_str = change_disp_to_string( bits_6.at(5), disp) ;
								tokenTable.machine_code = change_opcode_bits6(opcode+bits_6) + disp_str ;
								tokenTable.isTranslate = true ;
							}//if							
						}//if	
					}//if					
					//若不存在，hash至LITTAB 
					else{
						if(literal_tableNum == 6) literal_str = findToken(table6,literal_tableNo) ;
						else literal_str = findToken(table7,literal_tableNo) ;						
						createLiteralHash(hashValue(countAscii(literal_str)), literal_tableNum, literal_tableNo, literal_type);
					}//else
				}//if
			}//if
			else if(instr_tableNo == 48){//STT
				opcode = "100001" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "840000";
					else tokenTable.machine_code = "84000000";
					tokenTable.isTranslate = true ;
				}//if*/
					if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					//若literal存在在LITTAB 
					if(isInLiteralTable(literal_tableNum, literal_tableNo, return_literalTable_No)){
						if(LITTAB[return_literalTable_No].hasAddress){
							bits_6 = findLiteral_n_i_x_b_p_e(tokenTable, literal_tableNum, literal_tableNo, disp) ;
							if(bits_6.compare("no translate") != 0){
								disp_str = change_disp_to_string( bits_6.at(5), disp) ;
								tokenTable.machine_code = change_opcode_bits6(opcode+bits_6) + disp_str ;
								tokenTable.isTranslate = true ;
							}//if							
						}//if	
					}//if					
					//若不存在，hash至LITTAB 
					else{
						if(literal_tableNum == 6) literal_str = findToken(table6,literal_tableNo) ;
						else literal_str = findToken(table7,literal_tableNo) ;						
						createLiteralHash(hashValue(countAscii(LowToUpCase(literal_str))), literal_tableNum, literal_tableNo, literal_type);
					}//else
				}//if
			}//if
			else if(instr_tableNo == 49){//STX
				
				opcode = "000100" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "100000";
					else tokenTable.machine_code = "10000000";
					tokenTable.isTranslate = true ;
				}//if*/
					if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					//若literal存在在LITTAB 
					if(isInLiteralTable(literal_tableNum, literal_tableNo, return_literalTable_No)){
						if(LITTAB[return_literalTable_No].hasAddress){
							bits_6 = findLiteral_n_i_x_b_p_e(tokenTable, literal_tableNum, literal_tableNo, disp) ;
							if(bits_6.compare("no translate") != 0){
								disp_str = change_disp_to_string( bits_6.at(5), disp) ;
								tokenTable.machine_code = change_opcode_bits6(opcode+bits_6) + disp_str ;
								tokenTable.isTranslate = true ;
							}//if							
						}//if	
					}//if					
					//若不存在，hash至LITTAB 
					else{
						if(literal_tableNum == 6) literal_str = findToken(table6,literal_tableNo) ;
						else literal_str = findToken(table7,literal_tableNo) ;						
						createLiteralHash(hashValue(countAscii(literal_str)), literal_tableNum, literal_tableNo, literal_type);
					}//else
				}//if
			}//if
			else if(instr_tableNo == 50){//SUB
				opcode = "000111" ;
				/*if(!tokenTable.isTranslate &&operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "1C0000";
					else tokenTable.machine_code = "1C000000";
					tokenTable.isTranslate = true ;
				}//if*/
					if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					//若literal存在在LITTAB 
					if(isInLiteralTable(literal_tableNum, literal_tableNo, return_literalTable_No)){
						if(LITTAB[return_literalTable_No].hasAddress){
							bits_6 = findLiteral_n_i_x_b_p_e(tokenTable, literal_tableNum, literal_tableNo, disp) ;
							if(bits_6.compare("no translate") != 0){
								disp_str = change_disp_to_string( bits_6.at(5), disp) ;
								tokenTable.machine_code = change_opcode_bits6(opcode+bits_6) + disp_str ;
								tokenTable.isTranslate = true ;
							}//if							
						}//if	
					}//if					
					//若不存在，hash至LITTAB 
					else{
						if(literal_tableNum == 6) literal_str = findToken(table6,literal_tableNo) ;
						else literal_str = findToken(table7,literal_tableNo) ;						
						createLiteralHash(hashValue(countAscii(literal_str)), literal_tableNum, literal_tableNo, literal_type);
					}//else
				}//if
			}//if
			else if(instr_tableNo == 51){//SUBF
				opcode = "010111" ;
				/*if(!tokenTable.isTranslate &&operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "5C0000";
					else tokenTable.machine_code = "5C000000";
					tokenTable.isTranslate = true ;
				}//if*/
					if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					//若literal存在在LITTAB 
					if(isInLiteralTable(literal_tableNum, literal_tableNo, return_literalTable_No)){
						if(LITTAB[return_literalTable_No].hasAddress){
							bits_6 = findLiteral_n_i_x_b_p_e(tokenTable, literal_tableNum, literal_tableNo, disp) ;
							if(bits_6.compare("no translate") != 0){
								disp_str = change_disp_to_string( bits_6.at(5), disp) ;
								tokenTable.machine_code = change_opcode_bits6(opcode+bits_6) + disp_str ;
								tokenTable.isTranslate = true ;
							}//if							
						}//if	
					}//if					
					//若不存在，hash至LITTAB 
					else{
						if(literal_tableNum == 6) literal_str = findToken(table6,literal_tableNo) ;
						else literal_str = findToken(table7,literal_tableNo) ;						
						createLiteralHash(hashValue(countAscii(LowToUpCase(literal_str))), literal_tableNum, literal_tableNo, literal_type);
					}//else
				}//if
			}//if
			else if(instr_tableNo == 54){//TD
				opcode = "111000" ;
				/*if(!tokenTable.isTranslate &&operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "E00000";
					else tokenTable.machine_code = "E0000000";
					tokenTable.isTranslate = true ;
				}//if*/
				if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					//若literal存在在LITTAB 
					if(isInLiteralTable(literal_tableNum, literal_tableNo, return_literalTable_No)){
						if(LITTAB[return_literalTable_No].hasAddress){
							bits_6 = findLiteral_n_i_x_b_p_e(tokenTable, literal_tableNum, literal_tableNo, disp) ;
							if(bits_6.compare("no translate") != 0){
								disp_str = change_disp_to_string( bits_6.at(5), disp) ;
								tokenTable.machine_code = change_opcode_bits6(opcode+bits_6) + disp_str ;
								tokenTable.isTranslate = true ;
							}//if							
						}//if	
					}//if					
					//若不存在，hash至LITTAB 
					else{
						if(literal_tableNum == 6) literal_str = findToken(table6,literal_tableNo) ;
						else literal_str = findToken(table7,literal_tableNo) ;						
						createLiteralHash(hashValue(countAscii(literal_str)), literal_tableNum, literal_tableNo, literal_type);
					}//else
				}//if
			}//if
			else if(instr_tableNo == 56){//TIXR
				opcode = "001011" ;
				/*if(!tokenTable.isTranslate &&operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "2C0000";
					else tokenTable.machine_code = "2C000000";
					tokenTable.isTranslate = true ;
				}//if*/
					if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					//若literal存在在LITTAB 
					if(isInLiteralTable(literal_tableNum, literal_tableNo, return_literalTable_No)){
						if(LITTAB[return_literalTable_No].hasAddress){
							bits_6 = findLiteral_n_i_x_b_p_e(tokenTable, literal_tableNum, literal_tableNo, disp) ;
							if(bits_6.compare("no translate") != 0){
								disp_str = change_disp_to_string( bits_6.at(5), disp) ;
								tokenTable.machine_code = change_opcode_bits6(opcode+bits_6) + disp_str ;
								tokenTable.isTranslate = true ;
							}//if							
						}//if	
					}//if					
					//若不存在，hash至LITTAB 
					else{
						if(literal_tableNum == 6) literal_str = findToken(table6,literal_tableNo) ;
						else literal_str = findToken(table7,literal_tableNo) ;						
						createLiteralHash(hashValue(countAscii(literal_str)), literal_tableNum, literal_tableNo, literal_type);
					}//else
				}//if
			}//if
			else if(instr_tableNo == 58){//WD
				opcode = "110111" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "EC0000";
					else tokenTable.machine_code = "EC000000";
					tokenTable.isTranslate = true ;
				}//if*/
				if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					//若literal存在在LITTAB 
					if(isInLiteralTable(literal_tableNum, literal_tableNo, return_literalTable_No)){
						if(LITTAB[return_literalTable_No].hasAddress){
							bits_6 = findLiteral_n_i_x_b_p_e(tokenTable, literal_tableNum, literal_tableNo, disp) ;
							if(bits_6.compare("no translate") != 0){
								disp_str = change_disp_to_string( bits_6.at(5), disp) ;
								tokenTable.machine_code = change_opcode_bits6(opcode+bits_6) + disp_str ;
								tokenTable.isTranslate = true ;
							}//if							
						}//if	
					}//if					
					//若不存在，hash至LITTAB 
					else{
						if(literal_tableNum == 6) literal_str = findToken(table6,literal_tableNo) ;
						else literal_str = findToken(table7,literal_tableNo) ;						
						createLiteralHash(hashValue(countAscii(literal_str)), literal_tableNum, literal_tableNo, literal_type);
					}//else
				}//if
			}//if
	        
	        if(!tokenTable.isTranslate && operand_type1 != LITERAL){
	        	tokenTable.loc = LOCCTR ;
	        	
	        	 if(isOperandDefine(tokenTable)) {    	 	
					bits_6 = find_n_i_x_b_p_e(tokenTable, disp) ;
					if(bits_6.compare("no translate") != 0){
						//cout << "disp: " << hex << disp << endl ;
						disp_str = change_disp_to_string( bits_6.at(5), disp) ;				    
					    tokenTable.machine_code = change_opcode_bits6(opcode+bits_6) + disp_str ;				    
						tokenTable.isTranslate = true ;
					}//if		        				    
				}//if
			}//if
		}//if
		
		else {
			tokenTable.loc = LOCCTR ;
			if(!tokenTable.isTranslate){
				if(instr_tableNo == 0){//ADD		   
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "180000";
					else tokenTable.machine_code = "18000000";
					tokenTable.isTranslate = true ;																					
				}//if
				else if(instr_tableNo == 1){//ADDF
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "580000";
					else tokenTable.machine_code = "58000000";
					tokenTable.isTranslate = true ;				
				}//if
				else if(instr_tableNo == 3){//AND				
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "400000";
					else tokenTable.machine_code = "40000000";
					tokenTable.isTranslate = true ;			
				}//if
				else if(instr_tableNo == 5){//COMP				
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "280000";
					else tokenTable.machine_code = "28000000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 6){//COMPF			
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "880000";
					else tokenTable.machine_code = "88000000";
					tokenTable.isTranslate = true ;			
				}//if
				else if(instr_tableNo == 8){//DIV
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "240000";
					else tokenTable.machine_code = "24000000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 9){//DIVF
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "640000";
					else tokenTable.machine_code = "64000000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 14){//J
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "3C0000";
					else tokenTable.machine_code = "3C000000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 15){//JEQ
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "300000";
					else tokenTable.machine_code = "30000000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 16){//JGT
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "340000";
					else tokenTable.machine_code = "34000000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 17){//JLT
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "380000";
					else tokenTable.machine_code = "38000000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 18){//JSUB
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "480000";
					else tokenTable.machine_code = "48000000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 19){//LDA
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "000000";
					else tokenTable.machine_code = "00000000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 20){//LDB
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "680000";
					else tokenTable.machine_code = "68000000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 21){//LDCH
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "500000";
					else tokenTable.machine_code = "50000000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 22){//LDF
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "700000";
					else tokenTable.machine_code = "70000000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 23){//LDL
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "080000";
					else tokenTable.machine_code = "08000000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 24){//LDS
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "6C0000";
					else tokenTable.machine_code = "6C000000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 25){//LDT
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "740000";
					else tokenTable.machine_code = "74000000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 26){//LDX
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "040000";
					else tokenTable.machine_code = "04000000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 27){//LPS
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "D00000";
					else tokenTable.machine_code = "D0000000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 28){//MUL
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "200000";
					else tokenTable.machine_code = "20000000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 29){//MULF
				if(!tokenTable.token_group.isType4) tokenTable.machine_code = "600000";
					else tokenTable.machine_code = "60000000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 32){//OR
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "440000";
					else tokenTable.machine_code = "44000000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 33){//RD
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "D80000";
					else tokenTable.machine_code = "D8000000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 35){//RSUB
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "4F0000";
					else tokenTable.machine_code = "4F000000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 39){//SSK
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "EC0000";
					else tokenTable.machine_code = "EC000000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 40){//STA
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "0C0000";
					else tokenTable.machine_code = "0C000000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 41){//STB
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "780000";
					else tokenTable.machine_code = "78000000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 42){//STCH
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "540000";
					else tokenTable.machine_code = "54000000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 43){//STF
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "800000";
					else tokenTable.machine_code = "80000000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 44){//STI
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "E40000";
					else tokenTable.machine_code = "E4000000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 45){//STL
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "140000";
					else tokenTable.machine_code = "14000000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 46){//STS
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "7C0000";
					else tokenTable.machine_code = "7C000000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 47){//STSW
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "E80000";
					else tokenTable.machine_code = "E8000000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 48){//STT
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "840000";
					else tokenTable.machine_code = "84000000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 49){//STX
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "100000";
					else tokenTable.machine_code = "10000000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 50){//SUB
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "1C0000";
					else tokenTable.machine_code = "1C000000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 51){//SUBF
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "5C0000";
					else tokenTable.machine_code = "5C000000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 54){//TD
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "E00000";
					else tokenTable.machine_code = "E0000000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 56){//TIXR
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "2C0000";
					else tokenTable.machine_code = "2C000000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 58){//WD
					if(!tokenTable.token_group.isType4) tokenTable.machine_code = "EC0000";
					else tokenTable.machine_code = "EC000000";
					tokenTable.isTranslate = true ;
				}//if	       
			}//if
	    	
		}//else
		
       		
		if(!tokenTable.token_group.isType4) LOCCTR = LOCCTR+ 3 ;
		else LOCCTR = LOCCTR+ 4 ;
	}//process_TPYE3or4()
	
	
	string string_turn_16(string str){
		string str_result = "" ;
		for ( int i = 0 ; i < str.size() ; i++){
			str_result = str_result + change10T016((int)str.at(i)) ;
		}//for
		
		return str_result ;
	}
	
	void process_pesudo(tokenTable &tokenTable){
		int instr_tableNo =  tokenTable.token_group.instr.tableNo -1 ;
		string str = "" ;
		string tmp_string = "" ;
		int count = 0 ;
		unsigned int address = 0 ;
		OUT_LITTAB out_littab ;
		
		if(instr_tableNo == 1){ //END
			if(!tokenTable.isTranslate){
				tokenTable.loc = LOCCTR ;
				tokenTable.isTranslate = true ;
			}//if
			
			if(!isPass2){
				for( int i = 0 ; i < 100 ; i++){
					if(!LITTAB[i].hasAddress && LITTAB[i].hashValue != -1){					
						if(LITTAB[i].type == 1){
						   LITTAB[i].address = LOCCTR ;
						   LITTAB[i].hasAddress = true ;
						   out_littab.literalList.push_back(LITTAB[i]) ;
						   LOCCTR = LOCCTR + 1 ;
						}//if
						else if(LITTAB[i].type == 2){						   						
							LITTAB[i].address = LOCCTR ;
						   	LITTAB[i].hasAddress = true ;
						   	out_littab.literalList.push_back(LITTAB[i]) ;
						   	tmp_string = string_turn_16(findToken(table7, LITTAB[i].tableNo));
						   	if(tmp_string.size() % 2== 0 ) count = tmp_string.size() / 2 ;
							else count = tmp_string.size() / 2 + 1 ;																									
							LOCCTR = LOCCTR + count ;	
						}//if
						else{
							LITTAB[i].address = LOCCTR ;
						  	LITTAB[i].hasAddress = true ;
						  	out_littab.literalList.push_back(LITTAB[i]) ;
						  	LOCCTR = LOCCTR + 3 ;
						}//else
					}//if
				}//for
				
				out_littab.pc = LOCCTR ;
				out_literal_list.push_back(out_littab) ;
			}//if
			else{
				LOCCTR = out_literal_list[literal_index].pc;
				literal_index++ ;
			}//else
		}//if
		
		else if(instr_tableNo == 2){ //BYTE
			if(!tokenTable.isTranslate)
				tokenTable.loc = LOCCTR ;
				
			if(tokenTable.token_group.operandList[0].outList[1].tableNum == 6){
				tmp_string = findToken(table6, tokenTable.token_group.operandList[0].outList[1].tableNo);
				if(tmp_string.size() == 2) str = tmp_string ;
				else str = "0" + tmp_string ;
				LOCCTR = LOCCTR + 1 ;	
			}//if
			else{
				str = string_turn_16(findToken(table7, tokenTable.token_group.operandList[0].outList[1].tableNo));
				if(str.size() % 2== 0 ) count = str.size() / 2 ;
				else count = str.size() / 2 + 1 ;																									
				LOCCTR = LOCCTR + count ;	
			}//else	
			
			if(!tokenTable.isTranslate){
				tokenTable.machine_code = str ;	
				tokenTable.isTranslate = true ;	
			}//if							
					
		}//if
		
		else if(instr_tableNo == 3){ //WORD
			
			if(!tokenTable.isTranslate){
				tokenTable.loc = LOCCTR ;			 
				str = change10T016(stringToInt(findToken(table6, tokenTable.token_group.operandList[0].outList[0].tableNo))) ;
				tokenTable.machine_code = str ;	
				tokenTable.isTranslate = true ;	
			}//if
			LOCCTR = LOCCTR + 3 ;			
		}//if
		
		else if(instr_tableNo == 4){ //RESB	
			if(!tokenTable.isTranslate){
				tokenTable.loc = LOCCTR ;
				tokenTable.isTranslate = true ;
			}//if			
			LOCCTR = LOCCTR + stringToInt(findToken(table6, tokenTable.token_group.operandList[0].outList[0].tableNo)) ;
			
		}//if
		
		else if(instr_tableNo == 5){ //RESW
		    if(!tokenTable.isTranslate){
		    	tokenTable.loc = LOCCTR ;
		    	tokenTable.isTranslate = true ;
			}//if			
			LOCCTR = LOCCTR + 3*stringToInt(findToken(table6, tokenTable.token_group.operandList[0].outList[0].tableNo)) ;			
		}//if
		
		else if(instr_tableNo == 7){ //BASE
			if(!tokenTable.isTranslate ){
				tokenTable.loc = LOCCTR ;
				if(tokenTable.token_group.operandList[0].outList[0].tableNum == 5){
					if(isInSymbolTable(tokenTable.token_group.operandList[0].outList[0].tableNo, address)){
						base_reg.address = address ;
						tokenTable.isTranslate = true ;
					}//if
				}//if			
			}//if				
		}//if
		
		else if(instr_tableNo == 8){ //LTORG
			if(!tokenTable.isTranslate){
				tokenTable.loc = LOCCTR ;
				tokenTable.isTranslate = true ;
			}//if		
			
			if(!isPass2){
				for( int i = 0 ; i < 100 ; i++){
					if(!LITTAB[i].hasAddress && LITTAB[i].hashValue != -1 ){					
						if(LITTAB[i].type == 1){
						   LITTAB[i].address = LOCCTR ;
						   LITTAB[i].hasAddress = true ;
						   out_littab.literalList.push_back(LITTAB[i]) ;
						   LOCCTR = LOCCTR + 1 ;
						}//if
						else if(LITTAB[i].type == 2){						
							LITTAB[i].address = LOCCTR ;
						   	LITTAB[i].hasAddress = true ;
						   	out_littab.literalList.push_back(LITTAB[i]) ;
						   	tmp_string = string_turn_16(findToken(table7, LITTAB[i].tableNo));
						   	if(tmp_string.size() % 2== 0 ) count = tmp_string.size() / 2 ;
							else count = tmp_string.size() / 2 + 1 ;																									
							LOCCTR = LOCCTR + count ;	
						}//if
						else{
							LITTAB[i].address = LOCCTR ;
						  	LITTAB[i].hasAddress = true ;
						  	out_littab.literalList.push_back(LITTAB[i]) ;
						  	LOCCTR = LOCCTR + 3 ;
						}//else
					}//if
				}//for
				
				out_littab.pc = LOCCTR ;				
				out_literal_list.push_back(out_littab) ;
			}//if
			else{
				LOCCTR = out_literal_list[literal_index].pc;
				literal_index++ ;
			}//else	
		}//if		
		
	}//process_pesudo()
	
	
	
	void pass1(vector<tokenTable> &tokenTableList){
		string str = "" ;
		SYMTAB symtab ;
		int inStr_tableNo = 0 ;
	    unsigned int address = 0 ;
		
		//opcode == 'START'
	    if(tokenTableList[0].token_group.instr.tableNum  == 2 && tokenTableList[0].token_group.instr.tableNo  == 1){
	    	str = findToken(table6, tokenTableList[0].token_group.operandList[0].outList[0].tableNo) ;
	    	LOCCTR = change16To10(str) ; //將初始位置轉成10進制存入locctr ，設定起始位址
			base_reg.address = -1 ; //將base_reg存入起始位址 
			tokenTableList[0].loc = LOCCTR  ;//設定指令位址	  
			tokenTableList[0].isTranslate = true ; //指令已翻譯	
		}//if
	    	
		else{
			LOCCTR = 0 ; //設定目前位址
			tokenTableList[0].loc = LOCCTR ;//設定指令位址
			tokenTableList[0].isTranslate = true ;
			base_reg.address = -1 ;
		}//else 
		
			
		if(tokenTableList.size() > 1){
			for(int i = 1 ; i < tokenTableList.size() ; i++){
				if(!tokenTableList[i].syntaxError){ //指令沒有syntax error 
					if(!tokenTableList[i].isOnlyComment) { //指令非純註解					
					    //判斷指令是否有lable，若有，執行以下動作 
						if(tokenTableList[i].token_group.label.tableNum  == 5){
							
							if(isInSymbolTable(tokenTableList[i].token_group.label.tableNo, address)){ //label已經存在在crossRef 
								cout << "duplicate symbol!!!" << endl ;
								tokenTableList[i].syntaxError = true ;								
							}//if
							
							else{ //lable不存在在crossRef
							    
								if(tokenTableList[i].token_group.instr.tableNum  == 2 && tokenTableList[i].token_group.instr.tableNo  == 7){//若指令為EQU 
									if(tokenTableList[i].token_group.operandList.empty()){
										tokenTableList[i].loc = LOCCTR; //目前指令位址 
									 	tokenTableList[i].isTranslate = true ; //指令已翻譯 
										symtab.label_tableNo = tokenTableList[i].token_group.label.tableNo ;
									 	symtab.address = LOCCTR ;
									 	crossRef.push_back(symtab) ;
									 	
									}//if
                                  
									else if(tokenTableList[i].token_group.operandList[0].outList[0].tableNum == 5){ //若EQU後面接label
									    
										if(isInSymbolTable(tokenTableList[i].token_group.operandList[0].outList[0].tableNo, address)){ //若operand label存在在symbol 
								 	    	tokenTableList[i].loc = address; //目前指令位址 
									 		tokenTableList[i].isTranslate = true ; //指令已翻譯 
									 		symtab.label_tableNo = tokenTableList[i].token_group.label.tableNo ;
									 		symtab.address = address ;
									 		crossRef.push_back(symtab) ;
									 		//cout << "cross tableNo: " << tokenTableList[i].token_group.label.tableNo<< endl ;
									 		//cout << "cross address: " << symtab.address << endl ;
										}//if																																																		 							
									}//if
									
									else if(tokenTableList[i].token_group.operandList[0].outList[0].tableNum == 6){ //若EQU後面接 integer
										tokenTableList[i].loc = stringToInt(findToken(table6, tokenTableList[i].token_group.operandList[0].outList[0].tableNo)); //目前指令位址 
									 	tokenTableList[i].isTranslate = true ; //指令已翻譯
									 	symtab.label_tableNo =  tokenTableList[i].token_group.label.tableNo ;
										symtab.address = stringToInt(findToken(table6, tokenTableList[i].token_group.operandList[0].outList[0].tableNo)) ;
										crossRef.push_back(symtab) ;
										//cout << "tablNo: " << tokenTableList[i].token_group.label.tableNo << endl ;
										//cout << "address:" << hex << LOCCTR << endl ;
									}//else																			
								}//if
							    
							    	
							    else{				    	
							    	//將SYMBOL放入CROSS REF 
									symtab.label_tableNo = tokenTableList[i].token_group.label.tableNo ;
									symtab.address = LOCCTR ;
									crossRef.push_back(symtab) ;
									//cout << "tablNo: " << tokenTableList[i].token_group.label.tableNo << endl ;
									//cout << "address:" << hex << LOCCTR << endl ;
								}//else
																	
							    cout << symtab.label_tableNo << " " ;
								cout << symtab.address << endl ;														
							
							}//else	
							
						}//if(有label)
						
						//找opcode code 
						//若沒有syntax error && 指令非'EQU'，執行以下動作  
						if(!tokenTableList[i].syntaxError && !(tokenTableList[i].token_group.instr.tableNum  == 2 && tokenTableList[i].token_group.instr.tableNo  == 7) ){							
							inStr_tableNo = tokenTableList[i].token_group.instr.tableNo-1 ;
							//若為execution instruction 
							if(tokenTableList[i].token_group.instr.tableNum == 1){		    					
								if(OPTABlist[inStr_tableNo].format == TYPE1){ //指令為type1 
									process_TYPE1(tokenTableList[i]) ;
								}//if
								
								else if(OPTABlist[inStr_tableNo].format == TYPE2){ //指令為type2									
									process_TYPE2(tokenTableList[i]) ;
								}//if
								
								else{ //指令為type3or4
								    
									process_TPYE3or4(tokenTableList[i]) ;
								}//else
							}//if
							
							//若為pseudo instruction
							else{							
								process_pesudo(tokenTableList[i]) ;
							}//else								
						}//if
				    }//if
				}//if(判斷指令是否syntax error )			
			}//for
		}//if
				
	}//sicxe_translate_instr()
	
	void pass2(vector<tokenTable> &tokenTableList){
		string str = "" ;
		SYMTAB symtab ;
		int inStr_tableNo = 0 ;
	    unsigned int address = 0 ;
		
		//opcode == 'START'
	    if(tokenTableList[0].token_group.instr.tableNum  == 2 && tokenTableList[0].token_group.instr.tableNo  == 1){
	    	str = findToken(table6, tokenTableList[0].token_group.operandList[0].outList[0].tableNo) ;
	    	LOCCTR = change16To10(str) ; //將初始位置轉成10進制存入locctr ，設定起始位址
			base_reg.address = -1 ; //將base_reg存入起始位址 
		}//if
	    	
		else{
			LOCCTR = 0 ; //設定目前位址
			base_reg.address = -1 ;
		}//else 
		
			
		if(tokenTableList.size() > 1){
			for(int i = 1 ; i < tokenTableList.size() ; i++){
				if(!tokenTableList[i].syntaxError){ //指令沒有syntax error 
					if(!tokenTableList[i].isOnlyComment) { //指令非純註解					
					    //判斷指令是否有lable，若有，執行以下動作 
						if(tokenTableList[i].token_group.label.tableNum  == 5){													    
							if(tokenTableList[i].token_group.instr.tableNum  == 2 && tokenTableList[i].token_group.instr.tableNo  == 7){//若指令為EQU  
								if(!tokenTableList[i].isTranslate){
									if(tokenTableList[i].token_group.operandList[0].outList[0].tableNum == 5){ //若EQU後面接label						    
										if(isInSymbolTable(tokenTableList[i].token_group.operandList[0].outList[0].tableNo, address)){ //若operand label存在在symbol 											
											tokenTableList[i].loc = address; //目前指令位址 
									 		tokenTableList[i].isTranslate = true ; //指令已翻譯 
									 		symtab.label_tableNo = tokenTableList[i].token_group.label.tableNo ;
									 		symtab.address = address ;
									 		crossRef.push_back(symtab) ;												 	    	
										}//if	
										
										else{
											cout << "undefined symbol!!!" ;
											tokenTableList[i].syntaxError = true ;
										}//else																																																 							
									}//if	
								}//if                          																																							
							}//if
						}//if(有label)
						
						//找opcode code 
						//若沒有syntax error && 指令非'EQU'，執行以下動作  
						if(!tokenTableList[i].syntaxError && !(tokenTableList[i].token_group.instr.tableNum  == 2 && tokenTableList[i].token_group.instr.tableNo  == 7) ){							
							inStr_tableNo = tokenTableList[i].token_group.instr.tableNo-1 ;
							//若為execution instruction 
							if(tokenTableList[i].token_group.instr.tableNum == 1){
										    					
								if(OPTABlist[inStr_tableNo].format == TYPE1){ //指令為type1 
									process_TYPE1(tokenTableList[i]) ;
								}//if
								
								else if(OPTABlist[inStr_tableNo].format == TYPE2){ //指令為type2									
									process_TYPE2(tokenTableList[i]) ;
								}//if
								
								else{ //指令為type3or4								    
									process_TPYE3or4(tokenTableList[i]) ;
								}//else
							}//if
							
							//若為pseudo instruction
							else{
									process_pesudo(tokenTableList[i]) ;
							}//else								
						}//if
				    }//if
				}//if(判斷指令是否syntax error )			
			}//for
		}//if
				
	}//sicxe_translate_instr()
	
	string changeLOC(int loc){
		string str = change10T016(loc) ;
		string str_result = "" ;
		if(str.size() == 3) str_result = "0" + str ;
		else if(str.size() == 2) str_result = "00" + str ;
		else if(str.size() == 1) str_result = "000" + str ;
		else str_result = str ;
		return str_result ;
	}//changeLOC(int loc)
	
	void output_file(){
		int line = 5 ;
		int index = 0 ;
		string str = "" ;
		
		outFileName = inputFileName+" hw2-output.txt" ;
		outFile.open(outFileName.c_str(), ios::out) ;
		outFile << "Line\tLoc\tSource statement" << setw(20) << "Object code" << endl ;
		outFile << endl ;
		
		for(int n = 0 ; n < tokenTableList.size() ; n++){
			outFile << line << "\t" ;
			line = line + 5 ;
			if(tokenTableList[n].isTranslate){
				if(tokenTableList[n].isOnlyComment)								 
					outFile << "\t" << tokenTableList[n].cmd << endl ;
				else if((tokenTableList[n].token_group.instr.tableNum == 2 && tokenTableList[n].token_group.instr.tableNo == 2) || 
				            (tokenTableList[n].token_group.instr.tableNum == 2 && tokenTableList[n].token_group.instr.tableNo == 9)){
					outFile << "\t" << tokenTableList[n].cmd <<endl ;
					if(index < out_literal_list.size()){
						for ( int i = 0 ; i < out_literal_list[index].literalList.size() ; i++ ){
							outFile << line << "\t" ;
							line = line + 5 ;
							outFile << LowToUpCase(changeLOC(out_literal_list[index].literalList[i].address)) << "\t"  ;
							if(out_literal_list[index].literalList[i].type == 1){
								outFile << "=X'" << findToken(table6, out_literal_list[index].literalList[i].tableNo) << "'\t\t" ;
								outFile << findToken(table6, out_literal_list[index].literalList[i].tableNo) << endl ;
							}//if
							else if(out_literal_list[index].literalList[i].type == 2){
								outFile << "=C'" << findToken(table7, out_literal_list[index].literalList[i].tableNo) << "'\t\t" ;
								outFile << LowToUpCase(string_turn_16(LowToUpCase(findToken(table7, out_literal_list[index].literalList[i].tableNo))))<< endl ;
							}//if
							else{
								str = findToken(table6, out_literal_list[index].literalList[i].tableNo) ;
								outFile << "=" << str << "\t\t" ;
								
								if(str.size() == 6) outFile << str << endl ;
								else if(str.size() == 5) outFile << "0" << str << endl ;
								else if(str.size() == 4) outFile << "00" << str << endl ;
								else if(str.size() == 3) outFile << "000" << str << endl ;
								else if(str.size() == 2) outFile << "0000" << str << endl ;
								else if(str.size() == 1) outFile << "00000" << str << endl ;
							}//else
							
						}//for
						index++ ;
					}//if					
				}//if
							
				else{
					outFile << LowToUpCase(changeLOC(tokenTableList[n].loc)) << "\t" ;
					outFile << tokenTableList[n].cmd << "\t\t" << LowToUpCase(tokenTableList[n].machine_code) << endl ;
				}
				
			}//if
			
			else {
				if(!tokenTableList[n].isOnlyComment) {
					outFile << "\t" << tokenTableList[n].cmd 	<< "\tsyntax error!!!" <<endl ;									
				}//if
				else{
			    	outFile << "\t\t" <<  tokenTableList[n].cmd <<endl ;
				}//else					
			}//else			
		}//for	
		outFile.close() ;	
	}//output_file(){
};

class SICXE: public Token{
public:
	
	void translateMachine(){
		int index = 0 ;
		
		
		for ( int i = 0 ; i < tokenTableList.size() ; i++){
			//印出指令和outList 
		 	cout << tokenTableList[i].cmd << endl ;
			for ( int j = 0 ; j < tokenTableList[i].outList.size() ; j++){
				cout << "(" << tokenTableList[i].outList[j].tableNum << "," << tokenTableList[i].outList[j].tableNo << ")" ;
			}//for
			cout << endl ;
			
			checkSyntax(tokenTableList[i]) ; //判斷指令文法是否正確 
						
			//印出資訊 
			if(!tokenTableList[i].syntaxError) {
				cout << "label: (" << tokenTableList[i].token_group.label.tableNum << "," << tokenTableList[i].token_group.label.tableNo<< ")" << endl ;
				cout << "instruction: (" << tokenTableList[i].token_group.instr.tableNum << "," << tokenTableList[i].token_group.instr.tableNo<< ")" << endl ;
				if(tokenTableList[i].token_group.isType4) cout << "isType4: True"  << endl ;
				else  cout << "isType4: False"  << endl ;
				
			    cout << "operand Size: "  << tokenTableList[i].token_group.operandList.size()<< endl ; 
				cout << "operand List: "  << endl ;
				if(!tokenTableList[i].token_group.operandList.empty()){					
					for ( int k = 0 ; k < tokenTableList[i].token_group.operandList.size() ; k++){	
					    cout << "operand_type " << k+1 << " : " <<  tokenTableList[i].token_group.operandList[k].operand_type << endl ; 					
						for ( int m = 0 ; m < tokenTableList[i].token_group.operandList[k].outList.size() ; m++){							
							cout << "(" << tokenTableList[i].token_group.operandList[k].outList[m].tableNum << "," << tokenTableList[i].token_group.operandList[k].outList[m].tableNo << ")" ;
						}//for
						cout << endl ;
						
					}//for
					cout << endl ;
				}//if				
			}//if(印資訊) 		 	
		}//for(判斷切token完後文法是否正確) 
		//cout <<  tokenTableList[1].token_group.label.tableNo << " " <<  tokenTableList[2].token_group.label.tableNo << " " <<  tokenTableList[3].token_group.label.tableNo << endl ;
		//pass1
		pass1(tokenTableList) ;
		isPass2 = true ;
		for(int n = 0 ; n < tokenTableList.size() ; n++){
			if(tokenTableList[n].isTranslate){
				if(tokenTableList[n].isOnlyComment)
					cout << "\t" << tokenTableList[n].cmd <<endl ;
				else if(tokenTableList[n].token_group.instr.tableNum == 2 && tokenTableList[n].token_group.instr.tableNo == 2){
					cout << "\t" << tokenTableList[n].cmd <<endl ;
					for ( int i = 0 ; i < out_literal_list[index].literalList.size() ; i++ ){
						cout << hex << out_literal_list[index].literalList[i].address << "\t"  ;
						if(out_literal_list[index].literalList[i].type == 1){
							cout << "=X'" << findToken(table6, out_literal_list[index].literalList[i].tableNo) << "'\t\t\t" ;
							cout << findToken(table6, out_literal_list[index].literalList[i].tableNo) << endl ;
						}//if
						else if(out_literal_list[index].literalList[i].type == 2){
							cout << "=C'" << findToken(table7, out_literal_list[index].literalList[i].tableNo) << "'\t\t\t" ;
							cout << string_turn_16(findToken(table7, out_literal_list[index].literalList[i].tableNo))<< endl ;
						}//if
						else{
							cout << "='" << findToken(table6, out_literal_list[index].literalList[i].tableNo) << "\t\t\t" ;
							cout << findToken(table6, out_literal_list[index].literalList[i].tableNo) << endl ;
						}//else
						
					}//for
					index++ ;
				}//if
				else if(tokenTableList[n].token_group.instr.tableNum == 2 && tokenTableList[n].token_group.instr.tableNo == 9){
					cout << "\t" << tokenTableList[n].cmd <<endl ;
					for ( int i = 0 ; i < out_literal_list[index].literalList.size() ; i++ ){
						cout << changeLOC(out_literal_list[index].literalList[i].address) << "\t"  ;
						if(out_literal_list[index].literalList[i].type == 1){
							cout << "=X'" << findToken(table6, out_literal_list[index].literalList[i].tableNo) << "'\t\t" ;
							cout << findToken(table6, out_literal_list[index].literalList[i].tableNo) << endl ;
						}//if
						else if(out_literal_list[index].literalList[i].type == 2){
							cout << "=C'" << findToken(table7, out_literal_list[index].literalList[i].tableNo) << "'\t\t" ;
							cout << string_turn_16(findToken(table7, out_literal_list[index].literalList[i].tableNo))<< endl ;
						}//if
						else{
							cout << "=" << findToken(table6, out_literal_list[index].literalList[i].tableNo) << "\t\t" ;
							cout << findToken(table6, out_literal_list[index].literalList[i].tableNo) << endl ;
						}//else
						
					}//for
					index++ ;
				}//if
				else{
					
					cout << changeLOC(tokenTableList[n].loc) << "\t" ;
					cout << setw(30) << tokenTableList[n].cmd << "\t" ;
			
					cout<< std::right << setw(10) << tokenTableList[n].machine_code << endl ;
				}
			}//if
			
			else {
				if(!tokenTableList[n].isOnlyComment) {
					cout << changeLOC(tokenTableList[n].loc) << "\t" << tokenTableList[n].cmd  << "\t" << "isn't translate!!!" << endl ;					
				}//if
				else{
			    	cout << "\t" << tokenTableList[n].cmd <<endl ;
				}//else
					
					
			}//else			
		}//for
		
		cout << "------------------------------------------------------------------" <<endl ;
		pass2(tokenTableList) ;
		index = 0 ;
		for(int n = 0 ; n < tokenTableList.size() ; n++){
			if(tokenTableList[n].isTranslate){
				if(tokenTableList[n].isOnlyComment)
					cout << "\t" << tokenTableList[n].cmd <<endl ;
				else if(tokenTableList[n].token_group.instr.tableNum == 2 && tokenTableList[n].token_group.instr.tableNo == 2){
					cout << "\t" << tokenTableList[n].cmd <<endl ;
					for ( int i = 0 ; i < out_literal_list[index].literalList.size() ; i++ ){
						cout << out_literal_list[index].literalList[i].address << "\t"  ;
						if(out_literal_list[index].literalList[i].type == 1){
							cout << "=X'" << findToken(table6, out_literal_list[index].literalList[i].tableNo) << "'\t\t\t" ;
							cout << findToken(table6, out_literal_list[index].literalList[i].tableNo) << endl ;
						}//if
						else if(out_literal_list[index].literalList[i].type == 2){
							cout << "=C'" << findToken(table7, out_literal_list[index].literalList[i].tableNo) << "'\t\t\t" ;
							cout << string_turn_16(findToken(table7, out_literal_list[index].literalList[i].tableNo))<< endl ;
						}//if
						else{
							cout << "='" << findToken(table6, out_literal_list[index].literalList[i].tableNo) << "\t\t\t" ;
							cout << findToken(table6, out_literal_list[index].literalList[i].tableNo) << endl ;
						}//else
						
					}//for
					index++ ;
				}//if
				else if(tokenTableList[n].token_group.instr.tableNum == 2 && tokenTableList[n].token_group.instr.tableNo == 9){
					cout << "\t" << tokenTableList[n].cmd <<endl ;
					for ( int i = 0 ; i < out_literal_list[index].literalList.size() ; i++ ){
						cout << changeLOC(out_literal_list[index].literalList[i].address) << "\t" ;
						if(out_literal_list[index].literalList[i].type == 1){
							cout << "=X'" << findToken(table6, out_literal_list[index].literalList[i].tableNo) << "'\t\t\t" ;
							cout << findToken(table6, out_literal_list[index].literalList[i].tableNo) << endl ;
						}//if
						else if(out_literal_list[index].literalList[i].type == 2){
							cout << "=C'" << findToken(table7, out_literal_list[index].literalList[i].tableNo) << "'\t\t\t" ;
							cout << string_turn_16(findToken(table7, out_literal_list[index].literalList[i].tableNo))<< endl ;
						}//if
						else{
							
							cout << "='" << findToken(table6, out_literal_list[index].literalList[i].tableNo) << "\t\t\t" ;
							cout << findToken(table6, out_literal_list[index].literalList[i].tableNo) << endl ;
						}//else
						
					}//for
					index++ ;
				}//if
				else{
					cout << changeLOC(tokenTableList[n].loc) << "\t" << tokenTableList[n].cmd << setw(20)<< tokenTableList[n].machine_code << endl ;
				}
			}//if
			
			else {
				if(!tokenTableList[n].isOnlyComment) {
					cout << changeLOC(tokenTableList[n].loc) << "\t" << tokenTableList[n].cmd  << "\t" << "isn't translate!!!" << endl ;					
				}//if
				else{
			    	cout << setw(3) << "\t" << tokenTableList[n].cmd <<endl ;
				}//else
					
					
			}//else
			
		}//for
		index = 0 ;
	}//translateMachine()
};

class SIC: public Token{
public:
	void translateMachine(){
		int index = 0;
		
		for ( int i = 0 ; i < tokenTableList.size() ; i++){
			//印出指令和outList 
		 	cout << tokenTableList[i].cmd << endl ;
			for ( int j = 0 ; j < tokenTableList[i].outList.size() ; j++){
				cout << "(" << tokenTableList[i].outList[j].tableNum << "," << tokenTableList[i].outList[j].tableNo << "***)" ;
			}//for
			cout << endl ;
			
			checkSyntax(tokenTableList[i]) ; //判斷指令文法是否正確 
						
			//印出資訊 
			if(!tokenTableList[i].syntaxError) {
				cout << "label: (" << tokenTableList[i].token_group.label.tableNum << "," << tokenTableList[i].token_group.label.tableNo<< ")" << endl ;
				cout << "instruction: (" << tokenTableList[i].token_group.instr.tableNum << "," << tokenTableList[i].token_group.instr.tableNo<< ")" << endl ;
				if(tokenTableList[i].token_group.isType4) cout << "isType4: True"  << endl ;
				else  cout << "isType4: False"  << endl ;
				
			    cout << "operand Size: "  << tokenTableList[i].token_group.operandList.size()<< endl ; 
				cout << "operand List: "  << endl ;
				if(!tokenTableList[i].token_group.operandList.empty()){					
					for ( int k = 0 ; k < tokenTableList[i].token_group.operandList.size() ; k++){	
					    cout << "operand_type " << k+1 << " : " <<  tokenTableList[i].token_group.operandList[k].operand_type << endl ; 					
						for ( int m = 0 ; m < tokenTableList[i].token_group.operandList[k].outList.size() ; m++){							
							cout << "(" << tokenTableList[i].token_group.operandList[k].outList[m].tableNum << "," << tokenTableList[i].token_group.operandList[k].outList[m].tableNo << ")" ;
						}//for
						cout << endl ;
						
					}//for
					cout << endl ;
				}//if				
			}//if(印資訊) 		 	
		}//for(判斷切token完後文法是否正確) 
		
		
		pass1(tokenTableList) ;
		isPass2 = true ;
		for(int n = 0 ; n < tokenTableList.size() ; n++){
			if(tokenTableList[n].isTranslate){
				if(tokenTableList[n].isOnlyComment)
					cout << "\t" << tokenTableList[n].cmd <<endl ;
				else if(tokenTableList[n].token_group.instr.tableNum == 2 && tokenTableList[n].token_group.instr.tableNo == 2){
					cout << "\t" << tokenTableList[n].cmd <<endl ;
					for ( int i = 0 ; i < out_literal_list[index].literalList.size() ; i++ ){
						cout << hex << out_literal_list[index].literalList[i].address << "\t"  ;
						if(out_literal_list[index].literalList[i].type == 1){
							cout << "=X'" << findToken(table6, out_literal_list[index].literalList[i].tableNo) << "'\t\t\t" ;
							cout << findToken(table6, out_literal_list[index].literalList[i].tableNo) << endl ;
						}//if
						else if(out_literal_list[index].literalList[i].type == 2){
							cout << "=C'" << findToken(table7, out_literal_list[index].literalList[i].tableNo) << "'\t\t\t" ;
							cout << string_turn_16(findToken(table7, out_literal_list[index].literalList[i].tableNo))<< endl ;
						}//if
						else{
							cout << "='" << findToken(table6, out_literal_list[index].literalList[i].tableNo) << "\t\t\t" ;
							cout << findToken(table6, out_literal_list[index].literalList[i].tableNo) << endl ;
						}//else
						
					}//for
					index++ ;
				}//if
				else if(tokenTableList[n].token_group.instr.tableNum == 2 && tokenTableList[n].token_group.instr.tableNo == 9){
					cout << "\t" << tokenTableList[n].cmd <<endl ;
					for ( int i = 0 ; i < out_literal_list[index].literalList.size() ; i++ ){
						cout << changeLOC(out_literal_list[index].literalList[i].address) << "\t"  ;
						if(out_literal_list[index].literalList[i].type == 1){
							cout << "=X'" << findToken(table6, out_literal_list[index].literalList[i].tableNo) << "'\t\t\t" ;
							cout << findToken(table6, out_literal_list[index].literalList[i].tableNo) << endl ;
						}//if
						else if(out_literal_list[index].literalList[i].type == 2){
							cout << "=C'" << findToken(table7, out_literal_list[index].literalList[i].tableNo) << "'\t\t\t" ;
							cout << string_turn_16(findToken(table7, out_literal_list[index].literalList[i].tableNo))<< endl ;
						}//if
						else{
							cout << "='" << findToken(table6, out_literal_list[index].literalList[i].tableNo) << "\t\t\t" ;
							cout << findToken(table6, out_literal_list[index].literalList[i].tableNo) << endl ;
						}//else
						
					}//for
					index++ ;
				}//if
				else{
					cout << changeLOC(tokenTableList[n].loc) << "\t" << tokenTableList[n].cmd << setw(20)<< tokenTableList[n].machine_code << endl ;
				}
			}//if
			
			else {
				if(!tokenTableList[n].isOnlyComment) {
					cout << changeLOC(tokenTableList[n].loc) << "\t" << tokenTableList[n].cmd  << "\t" << "isn't translate!!!" << endl ;					
				}//if
				else{
			    	cout << "\t" << tokenTableList[n].cmd <<endl ;
				}//else
					
					
			}//else			
		}//for
		
		cout << "------------------------------------------------------------------" <<endl ;
		pass2(tokenTableList) ;
		index = 0 ;
		for(int n = 0 ; n < tokenTableList.size() ; n++){
			if(tokenTableList[n].isTranslate){
				if(tokenTableList[n].isOnlyComment)
					cout << "\t" << tokenTableList[n].cmd <<endl ;
				else if(tokenTableList[n].token_group.instr.tableNum == 2 && tokenTableList[n].token_group.instr.tableNo == 2){
					cout << "\t" << tokenTableList[n].cmd <<endl ;
					for ( int i = 0 ; i < out_literal_list[index].literalList.size() ; i++ ){
						cout << out_literal_list[index].literalList[i].address << "\t"  ;
						if(out_literal_list[index].literalList[i].type == 1){
							cout << "=X'" << findToken(table6, out_literal_list[index].literalList[i].tableNo) << "'\t\t\t" ;
							cout << findToken(table6, out_literal_list[index].literalList[i].tableNo) << endl ;
						}//if
						else if(out_literal_list[index].literalList[i].type == 2){
							cout << "=C'" << findToken(table7, out_literal_list[index].literalList[i].tableNo) << "'\t\t\t" ;
							cout << string_turn_16(findToken(table7, out_literal_list[index].literalList[i].tableNo))<< endl ;
						}//if
						else{
							cout << "='" << findToken(table6, out_literal_list[index].literalList[i].tableNo) << "\t\t\t" ;
							cout << findToken(table6, out_literal_list[index].literalList[i].tableNo) << endl ;
						}//else
						
					}//for
					index++ ;
				}//if
				else if(tokenTableList[n].token_group.instr.tableNum == 2 && tokenTableList[n].token_group.instr.tableNo == 9){
					cout << "\t" << tokenTableList[n].cmd <<endl ;
					for ( int i = 0 ; i < out_literal_list[index].literalList.size() ; i++ ){
						cout << changeLOC(out_literal_list[index].literalList[i].address) << "\t" ;
						if(out_literal_list[index].literalList[i].type == 1){
							cout << "=X'" << findToken(table6, out_literal_list[index].literalList[i].tableNo) << "'\t\t\t" ;
							cout << findToken(table6, out_literal_list[index].literalList[i].tableNo) << endl ;
						}//if
						else if(out_literal_list[index].literalList[i].type == 2){
							cout << "=C'" << findToken(table7, out_literal_list[index].literalList[i].tableNo) << "'\t\t\t" ;
							cout << string_turn_16(findToken(table7, out_literal_list[index].literalList[i].tableNo))<< endl ;
						}//if
						else{
							cout << "='" << findToken(table6, out_literal_list[index].literalList[i].tableNo) << "\t\t\t" ;
							cout << findToken(table6, out_literal_list[index].literalList[i].tableNo) << endl ;
						}//else
						
					}//for
					index++ ;
				}//if
				else{
					cout << changeLOC(tokenTableList[n].loc) << "\t" << tokenTableList[n].cmd << setw(20)<< tokenTableList[n].machine_code << endl ;
				}
			}//if
			
			else {
				if(!tokenTableList[n].isOnlyComment) {
					cout << changeLOC(tokenTableList[n].loc) << "\t" << tokenTableList[n].cmd  << "\t" << "isn't translate!!!" << endl ;					
				}//if
				else{
			    	cout << setw(3) << "\t" << tokenTableList[n].cmd <<endl ;
				}//else
					
					
			}//else
			
		}//for
		index = 0 ;				
	}//translateMachine()
		
		void pass1(vector<tokenTable> &tokenTableList){		
			string str = "" ;
			SYMTAB symtab ;
			int inStr_tableNo = 0 ;
		    unsigned int address = 0 ;
			
			//opcode == 'START'
		    if(tokenTableList[0].token_group.instr.tableNum  == 2 && tokenTableList[0].token_group.instr.tableNo  == 1){
		    	str = findToken(table6, tokenTableList[0].token_group.operandList[0].outList[0].tableNo) ;
		    	LOCCTR = change16To10(str) ; //將初始位置轉成10進制存入locctr ，設定起始位址
				base_reg.address =  LOCCTR ; //將base_reg存入起始位址 
				tokenTableList[0].loc = LOCCTR  ;//設定指令位址	  
				tokenTableList[0].isTranslate = true ; //指令已翻譯	
				
			}//if
		    	
			else{
				LOCCTR = 0 ; //設定目前位址
				tokenTableList[0].loc = LOCCTR ;//設定指令位址
				tokenTableList[0].isTranslate = true ; 
			}//else 
			
			if(tokenTableList.size() > 1){
				
				for(int i = 1 ; i < tokenTableList.size() ; i++){
					if(!tokenTableList[i].syntaxError){ //指令沒有syntax error 
						if(!tokenTableList[i].isOnlyComment) { //指令非純註解
						
						    //判斷指令是否有lable，若有，執行以下動作 
							if(tokenTableList[i].token_group.label.tableNum  == 5){
								if(isInSymbolTable(tokenTableList[i].token_group.label.tableNo, address)){ //label已經存在在crossRef 
									cout << "duplicate symbol!!!" << endl ;
									tokenTableList[i].syntaxError = true ;								
								}//if
								
								else{ //lable不存在在crossRef
									if(tokenTableList[i].token_group.instr.tableNum  == 2 && tokenTableList[i].token_group.instr.tableNo  == 7){//若指令為EQU 
										if(tokenTableList[i].token_group.operandList.empty()){
											tokenTableList[i].loc = LOCCTR; //目前指令位址 
										 	tokenTableList[i].isTranslate = true ; //指令已翻譯 
											symtab.label_tableNo = tokenTableList[i].token_group.label.tableNo ;
										 	symtab.address = LOCCTR ;
										 	crossRef.push_back(symtab) ;										 	
										}//if                              
										else if(tokenTableList[i].token_group.operandList[0].outList[0].tableNum == 5){ //若EQU後面接label										    
											if(isInSymbolTable(tokenTableList[i].token_group.operandList[0].outList[0].tableNo, address)){ //若operand label存在在symbol 
									 	    	tokenTableList[i].loc = address; //目前指令位址 
										 		tokenTableList[i].isTranslate = true ; //指令已翻譯 
										 		symtab.label_tableNo = tokenTableList[i].token_group.label.tableNo ;
										 		symtab.address = address ;
										 		crossRef.push_back(symtab) ;
											}//if	
																																																												 							
										}//if
										
										else if(tokenTableList[i].token_group.operandList[0].outList[0].tableNum == 6){ //若EQU後面接 integer
											tokenTableList[i].loc = stringToInt(findToken(table6, tokenTableList[i].token_group.operandList[0].outList[0].tableNo)) ; //目前指令位址 
										 	tokenTableList[i].isTranslate = true ; //指令已翻譯
										 	symtab.label_tableNo =  tokenTableList[i].token_group.label.tableNo ;
											symtab.address = stringToInt(findToken(table6, tokenTableList[i].token_group.operandList[0].outList[0].tableNo)) ;
											crossRef.push_back(symtab) ;
										}//else																			
									}//if
								
								    else{
								    	//將SYMBOL放入CROSS REF 
										symtab.label_tableNo = tokenTableList[i].token_group.label.tableNo ;
										symtab.address = LOCCTR ;
										crossRef.push_back(symtab) ;
										
									}//else		
									
									cout << symtab.label_tableNo << " " ;
									cout << symtab.address << endl ;						    																										
								}//else			
							}//if(有label)
							
							//找opcode code 
							//若沒有syntax error && 指令非'EQU'，執行以下動作  
							if(!tokenTableList[i].syntaxError && !(tokenTableList[i].token_group.instr.tableNum  == 2 && tokenTableList[i].token_group.instr.tableNo  == 7) ){							
								inStr_tableNo = tokenTableList[i].token_group.instr.tableNo-1 ;
								//若為execution instruction 
								if(tokenTableList[i].token_group.instr.tableNum == 1){
									
									if(OPTABlist[inStr_tableNo].format == TYPE1){ //指令為type1 
										process_TYPE1(tokenTableList[i]) ;
									}//if
									
									else if(OPTABlist[inStr_tableNo].format == TYPE2){ //指令為type2										
										process_TYPE2(tokenTableList[i]) ;
									}//if
									
									else{ //指令為type3or4									    
										process_TPYE3or4(tokenTableList[i]) ;
									}//else
								}//if
								
								//若為pseudo instruction
								else{									
									process_pesudo(tokenTableList[i]) ;
								}//else								
							}//if
					    }//if
					}//if(判斷指令是否syntax error )			
				}//for
			}//if
		}//pass1()
		
		void pass2(vector<tokenTable> &tokenTableList){
			string str = "" ;
			SYMTAB symtab ;
			int inStr_tableNo = 0 ;
		    unsigned int address = 0 ;
			
			//opcode == 'START'
		    if(tokenTableList[0].token_group.instr.tableNum  == 2 && tokenTableList[0].token_group.instr.tableNo  == 1){
		    	str = findToken(table6, tokenTableList[0].token_group.operandList[0].outList[0].tableNo) ;
		    	LOCCTR = change16To10(str) ; //將初始位置轉成10進制存入locctr ，設定起始位址 				
			}//if
		    	
			else{
				LOCCTR = 0 ; //設定目前位址 
			}//else 
			
			if(tokenTableList.size() > 1){				
				for(int i = 1 ; i < tokenTableList.size() ; i++){
					if(!tokenTableList[i].syntaxError){ //指令沒有syntax error 
						if(!tokenTableList[i].isOnlyComment) { //指令非純註解																
							if(tokenTableList[i].token_group.instr.tableNum  == 2 && tokenTableList[i].token_group.instr.tableNo  == 7){//若指令為EQU                               
								if(tokenTableList[i].token_group.operandList[0].outList[0].tableNum == 5){ //若EQU後面接label
								    //若指令尚未被翻譯 
									if(!tokenTableList[i].isTranslate){
										if(isInSymbolTable(tokenTableList[i].token_group.operandList[0].outList[0].tableNo, address)){ //若operand label存在在symbol 
								 	    	tokenTableList[i].loc = address; //目前指令位址 
									 		tokenTableList[i].isTranslate = true ; //指令已翻譯 
									 		symtab.label_tableNo = tokenTableList[i].token_group.label.tableNo ;
									 		symtab.address = address ;
									 		crossRef.push_back(symtab) ;
										}//if
										else{
											cout << "undefined symbol!!!" << endl ;
											tokenTableList[i].syntaxError = true ;
										}//else
									}//if									    																																																					 							
								}//if(有label)																					
							}//if(為equ)								    																																			
																		 
							//若沒有syntax error && 指令非'EQU'，執行以下動作  
							if(!tokenTableList[i].syntaxError && !(tokenTableList[i].token_group.instr.tableNum  == 2 && tokenTableList[i].token_group.instr.tableNo  == 7) ){							
								inStr_tableNo = tokenTableList[i].token_group.instr.tableNo-1 ;
								//若為execution instruction 
								if(tokenTableList[i].token_group.instr.tableNum == 1){
									
									if(OPTABlist[inStr_tableNo].format == TYPE1){ //指令為type1 
										process_TYPE1(tokenTableList[i]) ;
									}//if
									
									else if(OPTABlist[inStr_tableNo].format == TYPE2){ //指令為type2										
										process_TYPE2(tokenTableList[i]) ;
									}//if
									
									else{ //指令為type3or4									    
										process_TPYE3or4(tokenTableList[i]) ;
									}//else
								}//if
								
								//若為pseudo instruction
								else{									
									process_pesudo(tokenTableList[i]) ;
								}//else								
						    }//if
					    }//if
					}//if
				}//for
			}//if			
		}//pass2()
		
		void process_TYPE1(tokenTable &tokenTable){
			int instr_tableNo =  tokenTable.token_group.instr.tableNo -1 ;
			if(instr_tableNo == 11){ //FIX
			 	if(!tokenTable.isTranslate){
			 		tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "C40000" ;
					tokenTable.isTranslate = true ;
				}//if			    
				LOCCTR = LOCCTR + 3 ;
			}//if
			else if(instr_tableNo == 12){ //FLOAT
				if(!tokenTable.isTranslate){
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "C00000" ;
					tokenTable.isTranslate = true ;
				}//if				
				LOCCTR = LOCCTR + 3 ;
			}//if
			else if(instr_tableNo == 13){ //HIO
				if(!tokenTable.isTranslate){
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "F40000" ;
					tokenTable.isTranslate = true ;
				}//if				
				LOCCTR = LOCCTR + 3 ;
			}//if
			else if(instr_tableNo == 31){ //NORM
				if(!tokenTable.isTranslate){
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "C80000" ;
					tokenTable.isTranslate = true ;
				}//if				
				LOCCTR = LOCCTR + 3 ;
			}//if
			else if(instr_tableNo == 38){ //SIO
				if(!tokenTable.isTranslate){
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "F00000" ;
					tokenTable.isTranslate = true ;
				}//if				
				LOCCTR = LOCCTR + 3 ;
			}//if
			else if(instr_tableNo == 55){ //TIO
				if(!tokenTable.isTranslate){
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "F80000" ;
					tokenTable.isTranslate = true ;
				}//if				
				LOCCTR = LOCCTR + 3 ;
			}//if
			else{
				if(!tokenTable.isTranslate){
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "000000" ;
					tokenTable.isTranslate = true ;
				}//if				
				LOCCTR = LOCCTR + 3 ;
			}//else
		}//process_TYPE1()
	
	
	void process_TYPE2(tokenTable &tokenTable){
		int instr_tableNo =  tokenTable.token_group.instr.tableNo -1 ;	
		string opcode = "" ;
		
		if(instr_tableNo == 2) { //ADDR	
			if(!tokenTable.isTranslate){
				opcode = "90" ;					
				tokenTable.machine_code = opcode;
				tokenTable.loc = LOCCTR ;
				tokenTable.isTranslate = true ;
			}//if 		  			
			LOCCTR = LOCCTR+ 3 ;
		}//if
		
		else if(instr_tableNo == 4){ //CLEAR
			if(!tokenTable.isTranslate){
				opcode = "B40000" ;					
				tokenTable.machine_code = opcode ;
				tokenTable.loc = LOCCTR ;
				tokenTable.isTranslate = true ;
			}//if				
			LOCCTR = LOCCTR+ 3 ;
		}//if
		
		else if(instr_tableNo == 7){ //COMPPR
			if(!tokenTable.isTranslate){
				opcode = "A00000" ;					
				tokenTable.machine_code = opcode;
				tokenTable.loc = LOCCTR ;
				tokenTable.isTranslate = true ;
			}//if				
			LOCCTR = LOCCTR+ 3 ;
		}//if
		else if(instr_tableNo == 10){ //DIVR
			if(!tokenTable.isTranslate){
				opcode = "9C0000" ;						
				tokenTable.machine_code = opcode ;
				tokenTable.loc = LOCCTR ;
				tokenTable.isTranslate = true ;
			}//if					
			LOCCTR = LOCCTR+ 3 ;
		}//if
		else if(instr_tableNo == 30){ //MULR
			if(!tokenTable.isTranslate){
				opcode = "980000" ;					
				tokenTable.machine_code = opcode;
				tokenTable.loc = LOCCTR ;
				tokenTable.isTranslate = true ;
			}//if				
			LOCCTR = LOCCTR+ 3 ;
		}//if
		else if(instr_tableNo == 34){ //RMO	
			if(!tokenTable.isTranslate){
				opcode = "AC0000" ;					
				tokenTable.machine_code = opcode ;
				tokenTable.loc = LOCCTR ;
				tokenTable.isTranslate = true ;
			}//if			
			LOCCTR = LOCCTR+ 3 ;
		}//if
		else if(instr_tableNo == 36){ //SHIFTL
			if(!tokenTable.isTranslate){
				opcode = "A40000" ;									
				tokenTable.machine_code = opcode ;
				tokenTable.loc = LOCCTR ;
				tokenTable.isTranslate = true ;	
			}//if	       			
			LOCCTR = LOCCTR+ 3 ;
		}//if
		else if(instr_tableNo == 37){ //SHIFTR	
			if(!tokenTable.isTranslate){
				opcode = "A80000" ;						
				tokenTable.machine_code = opcode ;
				tokenTable.loc = LOCCTR ;
				tokenTable.isTranslate = true ;
			}//if		 			
			LOCCTR = LOCCTR+ 3 ;
		}//if
		else if(instr_tableNo == 52){ //SUBR
			if(!tokenTable.isTranslate){
				opcode = "940000" ;						
				tokenTable.machine_code = opcode ;
				tokenTable.loc = LOCCTR ;
				tokenTable.isTranslate = true ;	
			}//if			
			LOCCTR = LOCCTR+ 3 ;
		}//if
		else if(instr_tableNo == 53){ //SVC
			if(!tokenTable.isTranslate){
				opcode = "B00000" ;		
				tokenTable.machine_code = opcode ;
				tokenTable.loc = LOCCTR ;
				tokenTable.isTranslate = true ;
			}//if			
			LOCCTR = LOCCTR+ 3 ;
		}//if
		else if(instr_tableNo == 57){ //TIXR
			if(!tokenTable.isTranslate){
				opcode = "B80000" ;						
				tokenTable.machine_code = opcode;
				tokenTable.loc = LOCCTR ;
				tokenTable.isTranslate = true ;
			}//if			
			LOCCTR = LOCCTR+ 3 ;
		}//if		
	}//process_TYPE2()
	
	void process_TPYE3or4(tokenTable &tokenTable){
		int instr_tableNo =  tokenTable.token_group.instr.tableNo -1 ;
		string opcode = "" ;
		unsigned int disp = 0 ;
		string disp_str = "" ;
		string x= "" ;
		string machine_code_str = "" ; 
		
		int literal_tableNum = 0 ;
		int literal_tableNo  = 0 ;
		int literal_type = 0 ;
		int return_literalTable_No = 0 ;
		string literal_str ="" ;
		
		int operand_type1 = 0 ;
	    if(!tokenTable.token_group.operandList.empty()){
	    	operand_type1 = tokenTable.token_group.operandList[0].operand_type ;
	    	if(operand_type1 == LITERAL){
	    		if(tokenTable.token_group.operandList[0].outList[1].tableNum == 4){
	    			if(tokenTable.token_group.operandList[0].outList[2].tableNum == 6) literal_type = 1 ;
	    			else literal_type = 2 ;
	    			literal_tableNum = tokenTable.token_group.operandList[0].outList[2].tableNum ;
	    			literal_tableNo = tokenTable.token_group.operandList[0].outList[2].tableNo ;
				}//if
				else{
					literal_type = 3 ;
					literal_tableNum = tokenTable.token_group.operandList[0].outList[1].tableNum ;
	    			literal_tableNo = tokenTable.token_group.operandList[0].outList[1].tableNo ;
				}//else
			}//if
			
	    	if(instr_tableNo == 0){//ADD		   
				opcode = "00011000" ;	
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "180000";
					tokenTable.isTranslate = true ;
				}//if*/							
										
			}//if
			else if(instr_tableNo == 1){//ADDF
				opcode = "01011000" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "580000";
					tokenTable.isTranslate = true ;
				}//if*/
				
			}//if
			else if(instr_tableNo == 3){//AND
				opcode = "01000000" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "400000";
					tokenTable.isTranslate = true ;
				}//if*/
				
			}//if
			else if(instr_tableNo == 5){//COMP
				opcode = "00101000" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "280000";
					tokenTable.isTranslate = true ;
				}//if*/
				
			}//if
			else if(instr_tableNo == 6){//COMPF
				opcode = "10001000" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "880000";
					tokenTable.isTranslate = true ;
				}//if*/
				
			}//if
			else if(instr_tableNo == 8){//DIV
				opcode = "00100100" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "240000";
					tokenTable.isTranslate = true ;
				}//if*/
					
			}//if
			else if(instr_tableNo == 9){//DIVF
				opcode = "01100100" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "640000";
					tokenTable.isTranslate = true ;
				}//if*/
			}//if
			else if(instr_tableNo == 14){//J
				opcode = "00111100" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "3C0000";
					tokenTable.isTranslate = true ;
				}//if*/
				
			}//if
			else if(instr_tableNo == 15){//JEQ
				opcode = "00110000" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "300000";
					tokenTable.isTranslate = true ;
				}//if*/
					
			}//if
			else if(instr_tableNo == 16){//JGT
				opcode = "00110100" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "340000";
					tokenTable.isTranslate = true ;
				}//if*/
			}//if
			else if(instr_tableNo == 17){//JLT
				opcode = "00111000" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "380000";
					tokenTable.isTranslate = true ;
				}//if*/
			}//if
			else if(instr_tableNo == 18){//JSUB
				opcode = "01001000" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "480000";
					tokenTable.isTranslate = true ;
				}//if*/
			}//if
			else if(instr_tableNo == 19){//LDA
				opcode = "00000000" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "000000";
					tokenTable.isTranslate = true ;
				}//if*/
				
			}//if
			else if(instr_tableNo == 20){//LDB
				opcode = "01101000" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "680000";
					tokenTable.isTranslate = true ;
				}//if*/
			}//if
			else if(instr_tableNo == 21){//LDCH
				opcode = "01010000" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "500000";
					tokenTable.isTranslate = true ;
				}//if*/
			}//if
			else if(instr_tableNo == 22){//LDF
				opcode = "01110000" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "700000";
					tokenTable.isTranslate = true ;
				}//if*/
			}//if
			else if(instr_tableNo == 23){//LDL
				opcode = "00001000" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "080000";
					tokenTable.isTranslate = true ;
				}//if*/
				
			}//if
			else if(instr_tableNo == 24){//LDS
				opcode = "01101100" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "6C0000";
					tokenTable.isTranslate = true ;
				}//if*/
				
			}//if
			else if(instr_tableNo == 25){//LDT
				opcode = "01110100" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "740000";
					tokenTable.isTranslate = true ;
				}//if*/
					
			}//if
			else if(instr_tableNo == 26){//LDX
				opcode = "00000100" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "040000";
					tokenTable.isTranslate = true ;
				}//if*/
				
			}//if
			else if(instr_tableNo == 27){//LPS
				opcode = "11010000" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "D00000";
					tokenTable.isTranslate = true ;
				}//if*/
				
			}//if
			else if(instr_tableNo == 28){//MUL
				opcode = "00100000" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "200000";
					tokenTable.isTranslate = true ;
				}//if*/
					
			}//if
			else if(instr_tableNo == 29){//MULF
				opcode = "01100000" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "600000";
					tokenTable.isTranslate = true ;
				}//if*/
					
			}//if
			else if(instr_tableNo == 32){//OR
				opcode = "01000100" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "440000";
					tokenTable.isTranslate = true ;
				}//if*/
					
			}//if
			else if(instr_tableNo == 33){//RD
				opcode = "11011000" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "D80000";
					tokenTable.isTranslate = true ;
				}//if*/
					
			}//if
			else if(instr_tableNo == 35){//RSUB
				opcode = "01001100" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "4C0000";
					tokenTable.isTranslate = true ;
				}//if*/
					
			}//if
			else if(instr_tableNo == 39){//SSK
				opcode = "11101100" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "EC0000";
					tokenTable.isTranslate = true ;
				}//if*/
				
			}//if
			else if(instr_tableNo == 40){//STA
				opcode = "00001100" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "0C0000";
					tokenTable.isTranslate = true ;
				}//if*/
					
			}//if
			else if(instr_tableNo == 41){//STB
				opcode = "01111000" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "780000";
					tokenTable.isTranslate = true ;
				}//if*/
					
			}//if
			else if(instr_tableNo == 42){//STCH
				opcode = "01010100" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "540000";
					tokenTable.isTranslate = true ;
				}//if*/
				
			}//if
			else if(instr_tableNo == 43){//STF
				opcode = "10000000" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "800000";
					tokenTable.isTranslate = true ;
				}//if*/
					
			}//if
			else if(instr_tableNo == 44){//STI
				opcode = "11010100" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "E40000";
					tokenTable.isTranslate = true ;
				}//if*/
				
			}//if
			else if(instr_tableNo == 45){//STL
				opcode = "00010100" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "140000";
					tokenTable.isTranslate = true ;
				}//if*/
					
			}//if
			else if(instr_tableNo == 46){//STS
				opcode = "01111100" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "7C0000";
					tokenTable.isTranslate = true ;
				}//if*/
					
			}//if
			else if(instr_tableNo == 47){//STSW
				opcode = "11101000" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "E80000";
					tokenTable.isTranslate = true ;
				}//if*/
					
			}//if
			else if(instr_tableNo == 48){//STT
				opcode = "10000100" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "840000";
					tokenTable.isTranslate = true ;
				}//if*/
					
			}//if
			else if(instr_tableNo == 49){//STX			
				opcode = "00010000" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "100000";
					tokenTable.isTranslate = true ;
				}//if*/
					
			}//if
			else if(instr_tableNo == 50){//SUB
				opcode = "00011100" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "1C0000";
					tokenTable.isTranslate = true ;
				}//if*/
					
			}//if
			else if(instr_tableNo == 51){//SUBF
				opcode = "01011100" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "5C0000";
					tokenTable.isTranslate = true ;
				}//if*/
			}//if
			else if(instr_tableNo == 54){//TD
				opcode = "11100000" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "E00000";
					tokenTable.isTranslate = true ;
				}//if*/
				
			}//if
			else if(instr_tableNo == 56){//TIXR
				opcode = "00101100" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "2C0000";
					tokenTable.isTranslate = true ;
				}//if*/
				
			}//if
			else if(instr_tableNo == 58){//WD
				opcode = "11011100" ;
				/*if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "EC0000";
					tokenTable.isTranslate = true ;
				}//if*/
			}//if
			
			if(!tokenTable.isTranslate && operand_type1 == LITERAL){
					tokenTable.loc = LOCCTR ;  
					//若literal存在在LITTAB 
					if(isInLiteralTable(literal_tableNum, literal_tableNo, return_literalTable_No)){
						if(LITTAB[return_literalTable_No].hasAddress){
							if(tokenTable.token_group.operandList.size() == 2) x = "1" ;
							else  x = "0" ;	
							disp = find_literal_disp(literal_tableNum, literal_tableNo) ;
							machine_code_str = opcode + x + change_disp_to_string(disp) ;
							tokenTable.machine_code = change_opcode_bits6(machine_code_str) ;
							tokenTable.isTranslate = true ;
						}//if					
					}//if
					
					//若literal不存在在LITTAB，hash至LITTAB 
					else{
						if(literal_tableNum == 6) literal_str = findToken(table6, literal_tableNo) ;
						else literal_str = findToken(table7, literal_tableNo) ;
						createLiteralHash(hashValue(countAscii(literal_str)), literal_tableNum, literal_tableNo, literal_type) ;
					}//else
				}//if
						
	        
	        else if(!tokenTable.isTranslate && operand_type1 != LITERAL){    
			     tokenTable.loc = LOCCTR ;   	
	        	 if(isOperandDefine(tokenTable)) {   				 	  	 
				    if(tokenTable.token_group.operandList.size() == 2) x = "1" ;
					else  x = "0" ;	
					disp = fine_sic_disp(tokenTable) ;
					machine_code_str = opcode + x + change_disp_to_string(disp) ;
				    tokenTable.machine_code = change_opcode_bits6(machine_code_str) ;				    
					tokenTable.isTranslate = true ;
				}//if
			}//if
		}//if
		
		else {
			if(!tokenTable.isTranslate){
				if(instr_tableNo == 0){//ADD
					tokenTable.loc = LOCCTR ;		   
					tokenTable.machine_code = "180000";
					tokenTable.isTranslate = true ;																					
				}//if
				else if(instr_tableNo == 1){//ADDF
					tokenTable.loc = LOCCTR ;
				    tokenTable.machine_code = "580000";
					tokenTable.isTranslate = true ;				
				}//if
				else if(instr_tableNo == 3){//AND
					tokenTable.loc = LOCCTR ;				
					tokenTable.machine_code = "400000";
					tokenTable.isTranslate = true ;			
				}//if
				else if(instr_tableNo == 5){//COMP	
					tokenTable.loc = LOCCTR ;			
					tokenTable.machine_code = "280000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 6){//COMPF	
					tokenTable.loc = LOCCTR ;		
					tokenTable.machine_code = "880000";
					tokenTable.isTranslate = true ;			
				}//if
				else if(instr_tableNo == 8){//DIV
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "240000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 9){//DIVF
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "640000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 14){//J
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "3C0000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 15){//JEQ
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "300000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 16){//JGT
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "340000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 17){//JLT
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "380000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 18){//JSUB
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "480000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 19){//LDA
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "000000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 20){//LDB
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "680000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 21){//LDCH
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "500000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 22){//LDF
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "700000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 23){//LDL
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "080000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 24){//LDS
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "6C0000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 25){//LDT
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "740000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 26){//LDX
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "040000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 27){//LPS
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "D00000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 28){//MUL
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "200000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 29){//MULF
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "600000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 32){//OR
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "440000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 33){//RD
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "D80000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 35){//RSUB
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "4C0000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 39){//SSK
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "EC0000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 40){//STA
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "0C0000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 41){//STB
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "780000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 42){//STCH
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "540000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 43){//STF
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "800000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 44){//STI
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "E40000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 45){//STL
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "140000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 46){//STS
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "7C0000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 47){//STSW
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "E80000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 48){//STT
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "840000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 49){//STX
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "100000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 50){//SUB
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "1C0000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 51){//SUBF
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "5C0000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 54){//TD
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "E00000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 56){//TIXR
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "2C0000";
					tokenTable.isTranslate = true ;
				}//if
				else if(instr_tableNo == 58){//WD
					tokenTable.loc = LOCCTR ;
					tokenTable.machine_code = "EC0000";
					tokenTable.isTranslate = true ;
				}//if	       
			}//if	    	
		}//else
		
		LOCCTR = LOCCTR + 3 ;
	}//process_TPYE3or4()
	
	int find_literal_disp(int tableNum, int tableNo){
		unsigned int address ;
		unsigned int disp ;
		int return_literalTable_No = 0 ;
		
		isInLiteralTable(tableNum, tableNo, return_literalTable_No) ;
		address = LITTAB[return_literalTable_No].address ;
		
		disp = address ;
		return disp ;						
	}//if
	
	int fine_sic_disp(tokenTable tokenTable){
		int operand_type = tokenTable.token_group.operandList[0].operand_type ;
		unsigned int address ;
		unsigned int disp ;
		if(operand_type == IMME_VALUE){
			//立即值後面接label 
			if(tokenTable.token_group.operandList[0].outList[1].tableNum == 5){
				isInSymbolTable(tokenTable.token_group.operandList[0].outList[1].tableNo, address) ;
				disp = address ;
			}//if
			else{
				disp = stringToInt(findToken(table6, tokenTable.token_group.operandList[0].outList[1].tableNo)) ;
			}//else
		}//if
		
		else if(operand_type == VALUE){
			disp = stringToInt(findToken(table6, tokenTable.token_group.operandList[0].outList[0].tableNo ))  ;
		}//if
		else if(operand_type == MEMORY){
			
			//indirect
			if(tokenTable.token_group.operandList[0].outList[0].tableNum == 4){
				isInSymbolTable(tokenTable.token_group.operandList[0].outList[1].tableNo, address ) ;
			}//if
			//direct
			else {				
				isInSymbolTable(tokenTable.token_group.operandList[0].outList[0].tableNo, address ) ;				
			}//else
			
			disp = address ;
		}//if
		else {
			disp = 0 ;	
		}//else
		
		return disp ;
	}//fine_sic_disp()
	
	string changeBinary(int n)  
	{  
		 string str = "" ;
		 string str2 = "" ;
		 int x = n ; //商
		 
		 if(n == 0)
		 	return "0" ; 
		 while(x != 0){
		 	str = str + intToString(x % 2) ;
		 	x = x / 2 ;
		 }//while
		 
		 for ( int i = str.size()-1 ; i >= 0 ; i--){
		 	str2 = str2 + str.at(i) ;
		 }//for
		 
		 return str2 ;
	}//changeBinary()
	
	string change_disp_to_string( int disp){
    	stringstream ss;
    	string str_result = "" ;
    	string str = "";
    	string str2 = "" ;  	
    	
    	ss << hex << disp ;
    	str = ss.str() ;
    	//cout << str ;
    	str2 = change16To2(str) ;
    	/*if(disp == 8255){
    		cout << str ; << endl ;
			cout << str2 << endl ;	
		}//if*/
    		
    	if(str2.size() > 15){
    		str_result = str_result + str2.substr(str2.size()-15, 15) ;
		}//if
		else if(str2.size() == 15){
			str_result = str_result + str2 ;
		}//if
		else if(str2.size() == 14){
			str_result = str_result + "0" + str2;
		}//if
		else if(str2.size() == 13){
			str_result = str_result + "00" + str2 ;
		}//if
		else if(str2.size() == 12){
			str_result = str_result + "000" + str2 ;
		}//if
		else if(str2.size() == 11){
			str_result = str_result + "0000" + str2 ;
		}//if
		else if(str2.size() == 10){
			str_result = str_result + "00000" + str2 ;
		}//if
		else if(str2.size() == 9){
			str_result = str_result + "000000" + str2 ;
		}//if
		else if(str2.size() == 8){
			str_result = str_result + "0000000" + str2 ;
		}//if
		else if(str2.size() == 7){
			str_result = str_result + "00000000" + str2 ;
		}//if
		else if(str2.size() == 6){
			str_result = str_result + "000000000" + str2 ;
		}//if
		else if(str2.size() == 5){
			str_result = str_result + "0000000000" + str2 ;
		}//if
		else if(str2.size() == 4){
			str_result = str_result + "00000000000" + str2 ;
		}//if
		else if(str2.size() == 3){
			str_result = str_result + "000000000000" + str2 ;
		}//if
		else if(str2.size() == 2){
			str_result = str_result + "0000000000000" + str2 ;
		}//if
		else if(str2.size() == 1){
			str_result = str_result + "00000000000000" + str2 ;
		}//if
 	    
 	    //cout << str_result << endl ;
    	return str_result ;
	}//change_disp_to_string()
	
	string change_opcode_bits6(string str){
		string str1 = str.substr(0,4) ;		
		string str2 = str.substr(4,4) ;	
		string str3 = str.substr(8,4);	
		string str4 = str.substr(12,4);
		string str5 = str.substr(16,4);
		string str6 = str.substr(20,4);			
		return change2To16(str1) + change2To16(str2) + change2To16(str3) + change2To16(str4) + change2To16(str5) + change2To16(str6);	
	}//change_opcode_bits6()
	
	void process_pesudo(tokenTable &tokenTable){
		int instr_tableNo =  tokenTable.token_group.instr.tableNo -1 ;
		string str = "" ;
		int count = 0 ;
		unsigned int address = 0 ;
		string disp = "" ;
		string tmp_string = "" ;
		
		OUT_LITTAB out_littab ;
		
		if(instr_tableNo == 1){ //END
			if(!tokenTable.isTranslate){
				tokenTable.loc = LOCCTR ;
				tokenTable.isTranslate = true ;
			}//if
			
			if(!isPass2){
				for( int i = 0 ; i < 100 ; i++){
					if(!LITTAB[i].hasAddress && LITTAB[i].hashValue != -1){
						if(LITTAB[i].type == 1){
							LITTAB[i].address = LOCCTR ;
							LITTAB[i].hasAddress = true ;
							out_littab.literalList.push_back(LITTAB[i]) ;
							LOCCTR = LOCCTR + 1 ;
						}//if
						else if(LITTAB[i].type == 2){
							LITTAB[i].address = LOCCTR ;
							LITTAB[i].hasAddress = true ;
							out_littab.literalList.push_back(LITTAB[i]) ;
							tmp_string = string_turn_16(findToken(table7, LITTAB[i].tableNo)) ;
							if(tmp_string.size() % 2 == 0) count = tmp_string.size() / 2 ;
							else count = tmp_string.size() / 2 + 1 ;
							LOCCTR = LOCCTR + count ;
						}//if
						else{
							LITTAB[i].address = LOCCTR ;
							LITTAB[i].hasAddress = true ;
							out_littab.literalList.push_back(LITTAB[i]) ;
							LOCCTR = LOCCTR + 3 ;
						}//else
					}//if				
				}//for
				
				out_littab.pc = LOCCTR ;
				out_literal_list.push_back(out_littab) ;
			}//if
			else{
				LOCCTR = out_literal_list[literal_index].pc ;
				literal_index++ ;
			}//else
			
		}//if
		
		else if(instr_tableNo == 2){ //BYTE
		    if(!tokenTable.isTranslate)
				tokenTable.loc = LOCCTR ;
				
			if(tokenTable.token_group.operandList[0].outList[1].tableNum == 6){
				tmp_string = findToken(table6, tokenTable.token_group.operandList[0].outList[1].tableNo) ;
				if(tmp_string.size() == 2) str = tmp_string ;
				else str = "0" + tmp_string ;
				LOCCTR = LOCCTR + 1 ;	
			}//if
			else{				
				str = string_turn_16(findToken(table7, tokenTable.token_group.operandList[0].outList[1].tableNo));
				if(str.size() %2 == 0 ) count = str.size() / 2 ;
				else count = str.size() / 2 + 1 ;	
																												
				LOCCTR = LOCCTR + count ;	
			}//else	
			
			if(!tokenTable.isTranslate){
				tokenTable.machine_code = str ;	
				tokenTable.isTranslate = true ;	
			}//if						
					
		}//if
		
		else if(instr_tableNo == 3){ //WORD
			if(!tokenTable.isTranslate){
				tokenTable.loc = LOCCTR ;
				str = change_pseudo_disp(stringToInt(findToken(table6, tokenTable.token_group.operandList[0].outList[0].tableNo))) ;
				tokenTable.machine_code = str ;	
				tokenTable.isTranslate = true ;	
			}//IF			
			LOCCTR = LOCCTR + 3 ; 			
		}//if
		
		else if(instr_tableNo == 4){ //RESB
			if(!tokenTable.isTranslate){
				tokenTable.loc = LOCCTR ;
				tokenTable.isTranslate = true ;
			}//if
			
			LOCCTR = LOCCTR + stringToInt(findToken(table6, tokenTable.token_group.operandList[0].outList[0].tableNo)) ;			
		}//if
		
		else if(instr_tableNo == 5){ //RESW
			if(!tokenTable.isTranslate){
				tokenTable.loc = LOCCTR ;
				tokenTable.isTranslate = true ;
			}//if			
			LOCCTR = LOCCTR + 3*stringToInt(findToken(table6, tokenTable.token_group.operandList[0].outList[0].tableNo)) ;			
		}//if
		
		else if(instr_tableNo == 7){ //BASE
			if(!tokenTable.isTranslate){
				tokenTable.loc = LOCCTR ;
				if(tokenTable.token_group.operandList[0].outList[0].tableNum == 5){
					if(isInSymbolTable(tokenTable.token_group.operandList[0].outList[0].tableNo, address)){
						base_reg.address = address ;
						tokenTable.isTranslate = true ;
					}//if
				}//if	
			}//if		
				
		}//if
		
		else if(instr_tableNo == 8){ //LTORG
			if(!tokenTable.isTranslate){
				tokenTable.loc = LOCCTR ;
				tokenTable.isTranslate = true ;
			}//if			
			
				if(!isPass2){
				for( int i = 0 ; i < 100 ; i++){
					if(!LITTAB[i].hasAddress && LITTAB[i].hashValue != -1){
						if(LITTAB[i].type == 1){
							LITTAB[i].address = LOCCTR ;
							LITTAB[i].hasAddress = true ;
							out_littab.literalList.push_back(LITTAB[i]) ;
							LOCCTR = LOCCTR + 1 ;
						}//if
						else if(LITTAB[i].type == 2){
							LITTAB[i].address = LOCCTR ;
							LITTAB[i].hasAddress = true ;
							out_littab.literalList.push_back(LITTAB[i]) ;
							tmp_string = string_turn_16(findToken(table7, LITTAB[i].tableNo)) ;
							if(tmp_string.size() % 2 == 0) count = tmp_string.size() / 2 ;
							else count = tmp_string.size() / 2 + 1 ;
							LOCCTR = LOCCTR + count ;
						}//if
						else{
							LITTAB[i].address = LOCCTR ;
							LITTAB[i].hasAddress = true ;
							out_littab.literalList.push_back(LITTAB[i]) ;
							LOCCTR = LOCCTR + 3 ;
						}//else
					}//if				
				}//for
				
				out_littab.pc = LOCCTR ;
				out_literal_list.push_back(out_littab) ;
			}//if
			else{
				LOCCTR = out_literal_list[literal_index].pc ;
				literal_index++ ;
			}//else
		}//if		
		
	}//process_pesudo()
	
	string change_pseudo_disp(int disp){
		stringstream ss;
		string str = "" ;
		string str_result = "" ;
		
		ss << hex << disp ;
		str = ss.str() ;
		
		if(str.size() > 6) str_result = str_result + str.substr(-6,6) ;
		else if(str.size() == 6) str_result = str ;
		else if(str.size() == 5) str_result = "0" + str ;
		else if(str.size() == 4) str_result = "00" + str ;
		else if(str.size() == 3) str_result = "000" + str ;
		else if(str.size() == 2) str_result = "0000" + str ;
		else if(str.size() == 1) str_result = "00000" + str ;
		else if(str.size() == 6) str_result = "000000" + str ;
		
		return str_result ;
	}//change_pseudo_disp() 
	
	void checkSyntax(tokenTable &tokenTable){
		int i = 0 ; //目前判斷的token index 
		int outlistSize = tokenTable.outList.size() ;
		OPERAND_GROUP tmp ;
		outTable outtab ;
		
		//先判斷指令開頭是否正確
		if(isFirstTokenRight(tokenTable.outList[0])){ 
		    		    		    
			//指令開頭為label 
	       if(tokenTable.outList[0].tableNum == 5){
	       	    //更新指令token_group的label 
	       	    tokenTable.token_group.label.tableNum = tokenTable.outList[0].tableNum ;
		        tokenTable.token_group.label.tableNo = tokenTable.outList[0].tableNo ;
		        
	       	    i++ ; //i = 1
	       		if(outlistSize > i){ //判斷label後是否還有東西 	       		    
	       			if(tokenTable.outList[i].tableNum == 1 || tokenTable.outList[i].tableNum == 2){ //第二個token為instruction 
					    cout << "case2!!!!!!!" << endl ;
					    tokenTable.token_group.instr.tableNum = tokenTable.outList[i].tableNum ;
		    			tokenTable.token_group.instr.tableNo = tokenTable.outList[i].tableNo ;
		    			
						i++ ; //i = 2
						if(outlistSize > i){ //判斷instruction後是否還有東西//若沒有，直接結束 
    					    //判斷operand的開頭或結尾是否是',' 
    						if(!(tokenTable.outList[i].tableNum == 4 && tokenTable.outList[i].tableNo == 1) &&  !(tokenTable.outList[outlistSize-1].tableNum == 4 && tokenTable.outList[outlistSize-1].tableNo == 1) &&
					                	!((tokenTable.outList[outlistSize-1].tableNum == 4 && tokenTable.outList[outlistSize-1].tableNo == 10) &&(tokenTable.outList[outlistSize-2].tableNum == 4 && tokenTable.outList[outlistSize-2].tableNo == 1)) ){
    							for(int j = i ; j < outlistSize ; j++){
    								while((j < outlistSize) && !(tokenTable.outList[j].tableNum == 4 && tokenTable.outList[j].tableNo == 1 )){
    									//判斷此token使否不是最後一個且也不是',' 
    									if( !(j == outlistSize-1 && (tokenTable.outList[j].tableNum == 4 && tokenTable.outList[j].tableNo == 10 ))){
    										outtab.tableNum = tokenTable.outList[j].tableNum ;
    										outtab.tableNo = tokenTable.outList[j].tableNo ;
	    									tmp.outList.push_back(outtab) ;
										}//if 
										j++ ;
									}//while
									
									if(!tmp.outList.empty()) {
										tokenTable.token_group.operandList.push_back(tmp) ;									
									}//if  	
									tmp.outList.clear() ;	    								
								}//for
								
								
								if(!isInstrSyntaxRight(tokenTable)){
									cout << "syntax error" << endl ;
									tokenTable.syntaxError = true ;
									tokenTable.token_group.operandList.clear() ;
								}//if
								
							}//if
							else{ //若operand的開頭或結尾是'.'，syntax error  
								cout << "syntax error" << endl ;
								tokenTable.syntaxError = true ;
							}//else
						}//if
					}//if					
					else{ //非上述，syntax error 
						cout << "syntax error" << endl ;
						tokenTable.syntaxError = true ;
					}//else				
				}//if
				else { //若lable後沒有東西syntax error 
					cout << "syntax error" << endl ;
					tokenTable.syntaxError = true ;
				}//else
		   }//if
		   
		   //指令開頭為instruction
		   else if(tokenTable.outList[0].tableNum == 1 || tokenTable.outList[0].tableNum == 2){
		   	    cout << "case3!!!!!!!" << endl ;
		   	    tokenTable.token_group.instr.tableNum = tokenTable.outList[i].tableNum ;
    			tokenTable.token_group.instr.tableNo = tokenTable.outList[i].tableNo ;
    			
				i++ ; //i = 1
				if(outlistSize > i){ //判斷instruction後是否還有東西//若沒有，直接結束 
				    //判斷operand的開頭或結尾是否是',' 
					if(!(tokenTable.outList[i].tableNum == 4 && tokenTable.outList[i].tableNo == 1) &&  !(tokenTable.outList[outlistSize-1].tableNum == 4 && tokenTable.outList[outlistSize-1].tableNo == 1) &&
					      !((tokenTable.outList[outlistSize-1].tableNum == 4 && tokenTable.outList[outlistSize-1].tableNo == 10) &&(tokenTable.outList[outlistSize-2].tableNum == 4 && tokenTable.outList[outlistSize-2].tableNo == 1))){
						for(int j = i ; j < outlistSize ; j++){
							while((j < outlistSize) && !(tokenTable.outList[j].tableNum == 4 && tokenTable.outList[j].tableNo == 1 )){
								//判斷此token使否不是最後一個且也不是'.' 
								if( !(j == outlistSize-1 && (tokenTable.outList[j].tableNum == 4 && tokenTable.outList[j].tableNo == 10 ))){
									outtab.tableNum = tokenTable.outList[j].tableNum ;
									outtab.tableNo = tokenTable.outList[j].tableNo ;
									tmp.outList.push_back(outtab) ;
								}//if 
								j++ ;
							}//while

							if(!tmp.outList.empty()) {
								tokenTable.token_group.operandList.push_back(tmp) ;								
							}//if  		
							tmp.outList.clear() ;	    								
						}//for
						
						if(!isInstrSyntaxRight(tokenTable)){
							cout << "syntax error" << endl ;
							tokenTable.syntaxError = true ;
							tokenTable.token_group.operandList.clear() ;
						}//if
					}//if
					else{ //若operand的開頭或結尾是','，syntax error  
						cout << "syntax error" << endl ;
						tokenTable.syntaxError = true ;
					}//else
				}//if
		   	
		   }//if
		   
		   //指令開頭為'.'
		   else{
		   	    cout << "case5!!!!!!!" << endl ;
		   		tokenTable.isOnlyComment = true ;
		   }//else
		}//if
		
		else{
			cout << "syntax error" << endl ;
			tokenTable.syntaxError = true ;
		}//else		
	}//checkSyntax()
	
	bool isFirstTokenRight(outTable firstTab){
		if(firstTab.tableNum == 5 || firstTab.tableNum == 2 || firstTab.tableNum == 1 ||(firstTab.tableNum == 4 && firstTab.tableNo == 10 ))
			  return true ;
	    else
	    	return false ;		    
	} //isFirstTokenRight()
	

	
};

int main(int argc, char** argv) {
	Token token ;
	SICXE SICXE ;
	SIC SIC ;
    int cmd = 0 ;
    cout << "#####sp-作業二###" << endl ;
	cout << "INPUT SIC = 0 SICXE = 1" << endl;
	cout << "請輸入" ; 
    cin >> cmd ;
    
    if(cmd == 0){
    	SIC.readAllTable() ;
		SIC.readFile() ;
	    SIC.cutFileToken() ;
	    SIC.createTable(); 
	    SIC.translateMachine() ;
	    SIC.output_file() ;
	}//if
	
	else if(cmd == 1){
		SICXE.readAllTable() ;
		SICXE.readFile() ;
	    SICXE.cutFileToken() ;
	    SICXE.createTable(); 
	    SICXE.translateMachine() ;
	    SICXE.output_file() ;
	}//if
	else{
		cout << "輸入錯誤，請重新執行"<< endl ; 
	}//else
	

	return 0;
}

