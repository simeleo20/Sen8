tile = {
    {7,7,7,7,7,7,7,7},
    {7,7,7,7,7,7,7,7},
    {7,7,7,7,7,7,7,7},
    {7,7,7,7,7,7,7,7},
    {7,7,7,7,7,7,7,7},
    {7,7,7,7,7,7,7,7},
    {7,7,7,7,7,7,7,7},
    {7,7,7,7,7,7,7,7}
}

selectedColor = 0

function setup()
    sendAllColor()
    for color=0,15 do
        bgSet(color,1,color)
    end
    drawHorizontalLine(3,3,10,7)
    drawHorizontalLine(3,12,10,7)
    drawVerticalLine(3,3,10,7)
    drawVerticalLine(12,3,10,7)
    drawBigTile(4,4)
    drawBigColors(14,3)
end

function loop()
    detectMouseDraw(4,4)
    detectColorSelect(14,3)

end

function sendAllColor()
    for color=0,15 do
        fillTile(color)
        bgTileLoad(color,tile)
    end
end

function fillTile(color)
    for i=1,8 do
        for j=1,8 do
            tile[i][j] = color
        end
    end
end

function drawHorizontalLine(x, y, len, color )
    for i=x,x+len-1 do
        bgSet(i,y,color)
    end
end
function drawVerticalLine(x,y,len,color)
    for i=y,y+len-1 do
        bgSet(x,i,color)
    end
end

function drawBigTile(x,y)
    for y1=1,8 do
        for x1=1,8 do
            bgSet(x+x1-1,y+y1-1,tile[y1][x1])
        end
    end
end

function detectMouseDraw(x,y)
    local mouseX = math.floor(getMouseX()/8)
    local mouseY = math.floor(getMouseY()/8)
    
    if mouseX >= x and mouseX < x+8 and mouseY >= y and mouseY < y+8 then
        if btn(6) == 1 then
            print(mouseX-x,mouseY-y)
            tile[mouseY-y+1][mouseX-x+1] = selectedColor
            bgSet(mouseX,mouseY,selectedColor)
        end
    end
end

function drawBigColors(x,y)
   local color = 0
    for y1=1,4 do
        for x1=1,4 do
            bgSet(x+x1-1,y+y1-1,color)
            color = color + 1
        end
    end
end

function detectColorSelect(x,y)
    local mouseX = math.floor(getMouseX()/8)
    local mouseY = math.floor(getMouseY()/8)
    
    if mouseX >= x and mouseX < x+4 and mouseY >= y and mouseY < y+4 then
        if btn(6) == 1 then
            selectedColor = (mouseY-y)*4 + (mouseX-x)
            print(selectedColor)
        end
    end
end

