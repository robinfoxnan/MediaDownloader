author = "Robin-Fox"
version = 1.0

setting = {
    name = "���Ի���string���",
    dir = "d:\\",
	desc = "�������ú�����",
	input1 = "������",
	input2 = "ҳ��",
	input2 = "����/ҳ",
}


local person = {}
function person:new(name, age, gender)
	  local obj = {name = name, age = age, gender = gender}
	  setmetatable(obj, self)
	  self.__index = self
	  return obj
end

function lua_main(keyWord, pageIndex, pageSize)
	--printMessage ���ܽ����ַ���

	local alice = person:new("Alice", 25, "female")
	print(alice.name)
	print(alice.age)
	print(alice.gender)


    return 0
 end