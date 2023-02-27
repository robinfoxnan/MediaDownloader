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
    printMessage(
        [[字符集与编解码，提供了4个基础函数：
        ansiToUtf8
        utf8ToAnsi
        urlEncodeAnsi
        urlDecodeAnsi
        下面演示使用方法：
        ]])

    str = "i love it, "
    str = string.gsub(str, " ", "")

    printMessage("url is: " .. "%E5%8F%8C%E7%AC%99")
    ansi = '双笙'
    utf8 = ansiToUtf8(ansi)
    urlkey = urlEncodeAnsi(ansi)
    utf8ToAnsi(utf8)

    printMessage("ansi = ".. ansi)
    printMessage("utf8 = ".. utf8)
    printMessage("ansi = ".. utf8ToAnsi(utf8))
    printMessage("url = ".. urlkey)
    printMessage("ansi = " .. urlDecodeAnsi(urlKey))

    return 0
 end