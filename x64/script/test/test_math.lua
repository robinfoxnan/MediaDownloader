author = "Robin-Fox"
version = 1.0

setting = {
    name = "����math",
    dir = "d:\\",
	desc = "�������ú�����math",
	input1 = "������",
	input2 = "ҳ��",
	input3 = "����/ҳ",
}


function lua_main(keyWord, pageIndex, pageSize)
	--printMessage ���ܽ����ַ���

	print(math.abs(-10))  -- ��� 10
	print(math.abs(3.14)) -- ��� 3.14
	--math.ceil(x)
	--���ز�С�� x ����С������



	print(math.ceil(3.14)) -- ��� 4
	print(math.ceil(-3.14)) -- ��� -3
	--math.floor(x)
	--���ز����� x �����������



	print(math.floor(3.14)) -- ��� 3
	print(math.floor(-3.14)) -- ��� -4
	--math.sqrt(x)
	--���� x ��ƽ������
	

	print(math.sqrt(4))  -- ��� 2
	print(math.sqrt(9))  -- ��� 3
	print(math.sqrt(16)) -- ��� 4

	--math.random([m[, n]])
	--����һ�� [m, n] ֮���������������������������򷵻�һ�� [0,1) ֮������С����
	
	math.randomseed(os.time()) -- ��ʼ����������ӣ�ȷ��ÿ�����н����ͬ
	print(math.random())      -- ���һ�� [0,1) ֮������С��
	print(math.random(10))    -- ���һ�� [1,10] ֮����������
	print(math.random(5, 10)) -- ���һ�� [5,10] ֮����������
	
	--math.max(x1, x2, ...)
	--���ز����е����ֵ��


	
	print(math.max(1, 2, 3))    -- ��� 3
	print(math.max(-1, -2, -3)) -- ��� -1
	
	--math.min(x1, x2, ...)
	--���ز����е���Сֵ��
	
	
	print(math.min(1, 2, 3))    -- ��� 1
	print(math.min(-1, -2, -3)) -- ��� -3
	
	--ע��-- ����ʹ�� math.random ����ǰ��Ӧ��ʹ�� math.randomseed ������ʼ����������ӣ���ȷ��ÿ�����н����ͬ��

	
	print(math.ceil(3.14)) -- ��� 4
	print(math.ceil(-3.14)) -- ��� -3
	
	--math.floor(x)
	--���ز����� x �����������

	print(math.floor(3.14)) -- ��� 3
	print(math.floor(-3.14)) -- ��� -4
	--math.sqrt(x)-- ������ x ��ƽ������


	
	print(math.sqrt(4))  -- ��� 2
	print(math.sqrt(9))  -- ��� 3
	print(math.sqrt(16)) -- ��� 4
	
	--math.random([m[, n]])
	--����һ�� [m, n] ֮���������������������������򷵻�һ�� [0,1) ֮������С����

	math.randomseed(os.time()) -- ��ʼ����������ӣ�ȷ��ÿ�����н����ͬ
	print(math.random())      -- ���һ�� [0,1) ֮������С��
	print(math.random(10))    -- ���һ�� [1,10] ֮����������
	print(math.random(5, 10)) -- ���һ�� [5,10] ֮����������


    return 0
 end