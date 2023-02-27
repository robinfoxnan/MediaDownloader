author = "Robin"
version = 1.0

setting = {
    name = "测试禁用功能",
    dir = "d:\\",
	desc = "已经禁用了os, file, io库",
	input1 = "搜索词",
	input2 = "页号",
	input3 = "条数/页",
}


function lua_main(keyWord, pageIndex, pageSize)
	--printMessage 仅能接收字符串

	-- 禁用了io，不再能随意访问文件
	-- 打开文件并读取内容
	local f = io.open("d:\\TBuild\\test.txt", "r")
	local content = f:read("*all")
	io.close(f)
	print(content)


    return 0
 end