author = "Robin"
version = 1.0

setting = {
    name = "����string",
    dir = "d:\\",
	desc = "�������ú�����string",
	input1 = "������",
	input2 = "ҳ��",
	input3 = "����/ҳ",
}


function lua_main(keyWord, pageIndex, pageSize)
	--printMessage ���ܽ����ַ���

	-- �ַ�������
	local str = "Hello, world!"
	printMessage(tostring(#str)) -- ��� 13

	-- �ַ�������
	local str1 = "Hello"
	local str2 = "world"
	printMessage(str1 .. ", " .. str2 .. "!") -- ��� Hello, world!

	-- �ַ�����ȡ
	local str = "Hello, world!"
	printMessage(string.sub(str, 1, 5)) -- ��� "Hello"

	-- �ַ�������
	local str = "Hello, world!"
	printMessage(string.find(str, "world")) -- ��� 8

	-- �ַ����滻
	local str = "Hello, world!"
	printMessage(string.gsub(str, "world", "Lua")) -- ��� "Hello, Lua!"

	-- �ַ�����ʽ��
	local num = 42
	local str = "Lua"
	printMessage(string.format("The answer is %d, and the language is %s.", num, str)) -- ��� "The answer is 42, and the language is Lua."


    return 0
 end