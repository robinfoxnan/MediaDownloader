author = "Robin"
version = 1.0

setting = {
    name = "���Խ��ù���",
    dir = "d:\\",
	desc = "�Ѿ�������os, file, io��",
	input1 = "������",
	input2 = "ҳ��",
	input3 = "����/ҳ",
}


function lua_main(keyWord, pageIndex, pageSize)
	--printMessage ���ܽ����ַ���

	-- ������io����������������ļ�
	-- ���ļ�����ȡ����
	local f = io.open("d:\\TBuild\\test.txt", "r")
	local content = f:read("*all")
	io.close(f)
	print(content)


    return 0
 end