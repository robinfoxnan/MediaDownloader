author = "Robin"
version = 1.0

setting = {
    name = "ʾ�����ݽ�������",
    dir = "d:\\",
	desc = "��ʾ���ؽ������ݵĺ���notifyDataʹ�÷���",
	input1 = "������",
	input2 = "ҳ��",
	input3 = "����/ҳ",
}


function lua_main(keyWord, pageIndex, pageSize)
	-- test printMessage
    printMessage("test printMessage")
	
	
	local tbl = {
		singer = "����",
		song = "�Ƶ��˼�",
		album = "ĳר��",
		tags = "�Ƶ�",
		size = "5.4M",
		url = "HTTS://AFSDAFAAF",
	}
	-- ��������ĳ��������Ŀ��ʱ�򣬿���ʹ�ô˺���֪ͨ����
	notifyData(1, tbl)


    return 0
 end