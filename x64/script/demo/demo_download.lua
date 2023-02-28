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

	url = 'https://other-web-ri01-sycdn.kuwo.cn/e4f0672a81e44a802cf67a51c7640849/63fd96a0/resource/n3/97/99/2560035031.mp3'
	
	local header = HttpHeader()
    header:setItem('User-Agent', 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/96.0.4664.110 Safari/537.36')
	
	singer = 'test1'
	song = "测试"
    client1 = HttpClient()
    subDir = combinePath(setting.dir, singer)
	
	mkDir(subDir)
    fileName = subDir .. "\\"..  song .. ".mp3"
    printMessage(fileName)
    code = client1:doGetToFile(url, header, fileName)
    print(code)
	size = client1:getSize()
	print(size)
	
	print(client1:getErr())

    return 0
 end