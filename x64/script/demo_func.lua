author = "Robin-Fox"
version = 1.0

setting = {
    name = "演示函数使用",
    dir = "d:\\",
	desc = "根据关键字搜索歌曲，搜索时可以模拟网页设置分页，选择浏览第几页",
	input1 = "搜索词",
	input2 = "页号",
	input2 = "条数/页",
}


function lua_main(keyWord, pageIndex, pageSize)
	-- test printMessage
    printMessage("test printMessage")
	
	
	local tbl = {
		singer = "程响",
		song = "云底人间",
		album = "某专辑",
		tags = "云底",
		size = "5.4M",
		url = "HTTS://AFSDAFAAF",
	}
	-- 当解析到某个音乐条目的时候，可以使用此函数通知界面
	notifyData(1, tbl)


    return 0
 end