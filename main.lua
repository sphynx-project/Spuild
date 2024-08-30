local config = require("config")

local function main()
    print("Default Entry: " .. config.defaultEntry)
    print("Default Recipe: " .. config.defaultRecepie)
    print("Default File Name Pattern: " .. config.defaultFileNamePattern)
end

main()