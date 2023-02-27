author = "Robin"
version = 1.0

setting = {
    name = "测试string",
    dir = "d:\\",
	desc = "测试内置函数库string",
	input1 = "搜索词",
	input2 = "页号",
	input3 = "条数/页",
}


function lua_main(keyWord, pageIndex, pageSize)
	--printMessage 仅能接收字符串

	-- 字符串长度
	local str = "Hello, world!"
	printMessage(tostring(#str)) -- 输出 13

	-- 字符串连接
	local str1 = "Hello"
	local str2 = "world"
	printMessage(str1 .. ", " .. str2 .. "!") -- 输出 Hello, world!

	-- 字符串截取
	local str = "Hello, world!"
	printMessage(string.sub(str, 1, 5)) -- 输出 "Hello"

	-- 字符串查找
	local str = "Hello, world!"
	printMessage(string.find(str, "world")) -- 输出 8

	-- 字符串替换
	local str = "Hello, world!"
	printMessage(string.gsub(str, "world", "Lua")) -- 输出 "Hello, Lua!"

	-- 字符串格式化
	local num = 42
	local str = "Lua"
	printMessage(string.format("The answer is %d, and the language is %s.", num, str)) -- 输出 "The answer is 42, and the language is Lua."


    return 0
 end