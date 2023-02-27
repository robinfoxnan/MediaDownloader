author = "Robin-Fox"
version = 1.0

setting = {
    name = "测试基础string相关",
    dir = "d:\\",
	desc = "测试内置函数库",
	input1 = "搜索词",
	input2 = "页号",
	input2 = "条数/页",
}


local person = {}
function person:new(name, age, gender)
	  local obj = {name = name, age = age, gender = gender}
	  setmetatable(obj, self)
	  self.__index = self
	  return obj
end

function lua_main(keyWord, pageIndex, pageSize)
	--printMessage 仅能接收字符串

	local alice = person:new("Alice", 25, "female")
	print(alice.name)
	print(alice.age)
	print(alice.gender)


    return 0
 end