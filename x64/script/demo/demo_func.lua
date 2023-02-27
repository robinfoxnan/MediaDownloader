author = "Robin"
version = 1.0

setting = {
    name = "示范传递界面数据",
    dir = "d:\\",
	desc = "演示返回界面数据的函数notifyData使用方法",
	input1 = "搜索词",
	input2 = "页号",
	input3 = "条数/页",
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