author = "Robin"
version = 1.0

setting = {
    name = "�ַ���ʹ��",
    dir = "d:\\",
	desc = "չʾ�ַ���ת��",
	input1 = "������",
	input2 = "ҳ��",
	input3 = "����/ҳ",
}


function lua_main(keyWord, pageIndex, pageSize)
    printMessage(
        [[�ַ��������룬�ṩ��4������������
        ansiToUtf8
        utf8ToAnsi
        urlEncodeAnsi
        urlDecodeAnsi
        ������ʾʹ�÷�����
        ]])

    str = "i love it, "
    str = string.gsub(str, " ", "")

    printMessage("url is: " .. "%E5%8F%8C%E7%AC%99")
    ansi = '˫��'
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