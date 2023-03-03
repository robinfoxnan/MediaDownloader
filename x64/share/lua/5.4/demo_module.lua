-- 文件名为 module.lua
-- 定义一个名为 module 的模块
demo_module = {}
 
-- 定义一个常量
demo_module.constant = "这是一个常量"
 
-- 定义一个函数
function demo_module.func1()
    --
	print("func1 it is a test Function()")
end
 
local function func2()
    print("it is a private Function()")
end
 
function demo_module.func3()
    func2()
end
 
return demo_module