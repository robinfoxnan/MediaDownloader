author = "Robin"
version = 1.0

setting = {
    name = "�ļ�����",
    dir = "d:\\",
	desc = "չʾ���ʹ�õ�ǰĿ¼������ƴ�Ӵ�����Ŀ¼����д��Ͷ�ȡ�ļ�",
	input1 = "������",
	input2 = "ҳ��",
	input3 = "����/ҳ",
}


function lua_main(keyWord, pageIndex, pageSize)
    printMessage("��ǰĿ¼��" .. setting.dir)

	singer = "�ܶ�"
    subDir = combinePath(setting.dir,  singer)
	
	b = mkDir(subDir)
	if b then 
		print("�����ɹ�" .. subDir)
	else 
		print("����ʧ��" .. subDir)
	end
	
	b = fileExist(subDir)
	if b then 
		print("����" .. subDir)
	else 
		print("������" .. subDir)
	end
	
	print(b)
	song = "test"
    fileName = subDir .. "\\"..  song .. ".txt"
    printMessage("׼��д�ļ�" .. fileName)
	
    writeToFile("it is a test.", fileName)
    
    str = readFileAsString(fileName)
    print(str)

    return 0
 end