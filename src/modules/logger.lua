local Logger = {}
local LoggerScope = {}
Logger.__index = Logger
LoggerScope.__index = LoggerScope


function Logger:new(scope)
    local obj = setmetatable({}, self)
    obj.scope = scope;
    return obj
end

function LoggerScope:new(name)
    local obj = setmetatable({}, self)
    obj.name = name
    return obj
end

function Logger:log(message)
    print("[" .. (self.scope.name or "Global") .. "] " .. message)
end

function LoggerScope:log(message)
    print("[" .. self.name .. "] " .. message)
end

return {
    Logger = Logger,
    LoggerScope = LoggerScope
}
