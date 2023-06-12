function newCounter()
    local i = -1
    return funtion() i = i+1 return i end
end

cl = newCounter()
for i = 1,10 do print(cl()) end