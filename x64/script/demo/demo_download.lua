author = "Robin"
version = 1.0

setting = {
    name = "展示下载功能",
    dir = "d:\\",
	desc = "展示下载一个文件涉及的函数",
	input1 = "搜索词",
	input2 = "页号",
	input3 = "条数/页",
}


function lua_main(keyWord, pageIndex, pageSize)
    printMessage("当前目录：" .. setting.dir)

	url = 'https://blog.csdn.net/boomworks/article/details/124521383'
	print('准备下载：' .. url)
	
	-- 定义httpHeader
	local header = HttpHeader()
    header:setItem('User-Agent', 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/96.0.4664.110 Safari/537.36')
	
	singer = 'csdn'
	song = "测试"
	-- 定义一个HTTP访问的实体
    client1 = HttpClient()
    subDir = combinePath(setting.dir, singer)
	
	mkDir(subDir)
    fileName = subDir .. "\\"..  song .. ".html"
    printMessage('路径：' ..fileName)
    
    -- 直接下载到文件中
    code = client1:doGetToFile(url, header, fileName)
    print('http 应答码' .. code)
    
	size = client1:getSize()
	print('文件大小' .. size)
	
	print('错误码=' .. client1:getErr())

    return 0
 end