local config = require("config")
local Logger, LoggerScope = require("src.modules.logger").Logger, require("src.modules.logger").LoggerScope

local function main()
    local mainScope = LoggerScope:new("main")
    local logger = Logger:new(mainScope);
    mainScope:log("This is a test message.")
    logger:log("Hello");
end

main()
