local a, b = require("modules/a.lua")
local b2, b3 = require("modules/b.lua"), require("modules/b.lua")
-- local invalid = require('modules/c.lua')

assert(a == 'a' and b == 'b', 'First require failed')
assert(b2 == 'b', 'Second require failed')
assert(b3 == 'b', 'Third require failed')
print('It worked!')