author = "Robin"
version = 1.0

setting = {
    name = "字符集使用",
    dir = "d:\\",
	desc = "展示字符集转码",
	input1 = "搜索词",
	input2 = "页号",
	input3 = "条数/页",
}


function lua_main(keyWord, pageIndex, pageSize)
    printMessage("当前目录：" .. setting.dir)

	singer = "周懂"
    subDir = combinePath(setting.dir,  singer)
	
	b = mkDir(subDir)
	if b then 
		print("创建成功" .. subDir)
	else 
		print("创建失败" .. subDir)
	end
	
	b = fileExist(subDir)
	if b then 
		print("存在" .. subDir)
	else 
		print("不存在" .. subDir)
	end
	
	print(b)
	song = "test"
    fileName = subDir .. "\\"..  song .. ".txt"
    printMessage("准备写文件" .. fileName)
	
    writeToFile("it is a test.", fileName)
    
    str = readFileAsString(fileName)
    print(str)

    return 0
 end