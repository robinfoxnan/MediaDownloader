author = "Robin"
version = 1.0

setting = {
    name = "չʾ���ع���",
    dir = "d:\\",
	desc = "չʾ����һ���ļ��漰�ĺ���",
	input1 = "������",
	input2 = "ҳ��",
	input3 = "����/ҳ",
}


function lua_main(keyWord, pageIndex, pageSize)
    printMessage("��ǰĿ¼��" .. setting.dir)

	url = 'https://blog.csdn.net/boomworks/article/details/124521383'
	print('׼�����أ�' .. url)
	
	-- ����httpHeader
	local header = HttpHeader()
    header:setItem('User-Agent', 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/96.0.4664.110 Safari/537.36')
	
	singer = 'csdn'
	song = "����"
	-- ����һ��HTTP���ʵ�ʵ��
    client1 = HttpClient()
    subDir = combinePath(setting.dir, singer)
	
	mkDir(subDir)
    fileName = subDir .. "\\"..  song .. ".html"
    printMessage('·����' ..fileName)
    
    -- ֱ�����ص��ļ���
    code = client1:doGetToFile(url, header, fileName)
    print('http Ӧ����' .. code)
    
	size = client1:getSize()
	print('�ļ���С' .. size)
	
	print('������=' .. client1:getErr())

    return 0
 end