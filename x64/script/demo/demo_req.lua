require("demo_module")

author = "Robin"
version = 1.0

setting = {
    name = "脚本依赖方法演示脚本",
    dir = "d:\\",
	desc = "演示内置require的功能",
	input1 = "搜索词",
	input2 = "页号",
	input3 = "条数/页",
}


function lua_main(keyWord, pageIndex, pageSize)
    
	demo_module.func3()
    


    return 0
end

