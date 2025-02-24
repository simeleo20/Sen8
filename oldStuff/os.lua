function setup()
    print("setup")
    --bgSet(2,1,1)
end
scrollx = 0

function loop()
    mouseX = getMouseX()
    mouseY = getMouseY()
    if (btn(6) == 1) then
        bgSet((mouseX+(scrollx))%(256*2)/8, mouseY/8, 1)
    end
    --print(scrollx % 256)
    if(btn(2) == 1) then
        scrollx = scrollx + 1
        setScrollX(scrollx)
    end
    local key = getCharPressed()
    if(key ~= 0) then
        print(string.char(key))
    end
    --
end
