author = "Robin-Fox"
version = 1.0

setting = {
    name = "��ʾ����ʹ��",
    dir = "d:\\",
	desc = "���ݹؼ�����������������ʱ����ģ����ҳ���÷�ҳ��ѡ������ڼ�ҳ",
	input1 = "������",
	input2 = "ҳ��",
	input2 = "����/ҳ",
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