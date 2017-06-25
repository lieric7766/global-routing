1. 使用之程式語言：< C++ >
2. 使用之編譯平台：< Linux GNU g++ >
3. 各檔案說明：
	 global-routing/main    		: 主程式執行檔
	 global-routing/main.cpp		: 主程式source code
         global-routing/global_routing.cpp
	 global-routing/global_routing.h
	 
	 global-routing/readme.txt		: 本檔案
4. 編譯方式說明：        	
   主程式：
	   在 global-routing/ 這個資料夾下指令 : g++ global_routing.cpp main.cpp -o main
	   即可產生 main 執行檔
	
5. 執行、使用方式說明：
   主程式：
   	   compile 完成後，在 /global-routing/ 目錄下會產生一個 main 的執行檔
   	   執行檔的命令格式為 :  ./main [input file name] [output file name]

	   ex: ./main 12.in 12.out
