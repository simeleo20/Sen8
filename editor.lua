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

tilesInDrawing ={}
spritesInDrawing = {}

selectedColor = 0
selectedTile = 1


function setup()

    setTransparent(16)
    --fillTile(0)
    --bgTileLoad(0,tile)
    loadSprites()
    moveSelector(14,3)
    moveSelector2(1,21)
    setupTilesInDrawing()

    loadTilesBin()
    tileInDrawingToTile(selectedTile)
    



    
    
    drawTilesDisplay(0,21,0)


end

function loop()
    
    detectMouseDraw(4,4)
    detectColorSelect(14,3)
    detectTileSelect(0,21)
    if(btn(4) == 1) then
        printTile()
    end
    if(btn(1) == 1) then
        saveTiles()
    end
    
end

function vblank()
    drawHorizontalLine(3,3,10,7)
    drawHorizontalLine(3,12,10,7)
    drawVerticalLine(3,3,10,7)
    drawVerticalLine(12,3,10,7)
    drawBigTile(4,4)
    drawBigColors(14,3)
    drawHorizontalLine(0,29,32,8)
    drawHorizontalLine(0,0,32,8)
    printS(2,233,7,"x:"..math.floor(getMouseX()).." y:"..math.floor(getMouseY()));

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
        drawFilled(i,y,color)
    end
end
function drawVerticalLine(x,y,len,color)
    for i=y,y+len-1 do
        drawFilled(x,i,color)
    end
end

function drawBigTile(x,y)
    for y1=1,8 do
        for x1=1,8 do
            drawFilled(x+x1-1,y+y1-1,tile[y1][x1])
        end
    end
end

function detectMouseDraw(x,y)
    local mouseX = math.floor(getMouseX()/8)
    local mouseY = math.floor(getMouseY()/8)
    
    if mouseX >= x and mouseX < x+8 and mouseY >= y and mouseY < y+8 then
        if btn(6) == 1 then
            --print(mouseX-x,mouseY-y)
            tile[mouseY-y+1][mouseX-x+1] = selectedColor
            drawFilled(mouseX,mouseY,selectedColor)
            bgTileLoad(selectedTile,tile)
        end
    end
end

function drawBigColors(x,y)
   local color = 0
    for y1=1,4 do
        for x1=1,4 do
            drawFilled(x+x1-1,y+y1-1,color)
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
            moveSelector(mouseX,mouseY)
            --print(selectedColor)
        end
    end
end

function drawTilesDisplay(x,y,index)
    local counter = 0 
    for y1=0,7 do
        for x1=0,31 do
            bgSet(x+x1,y+y1,index+counter)
            counter = counter + 1
        end
    end
end
function detectTileSelect(x,y)
    local mouseX = math.floor(getMouseX()/8)
    local mouseY = math.floor(getMouseY()/8)
    
    if mouseX >= x and mouseX < x+32 and mouseY >= y and mouseY < y+8 then
        if btn(6) == 1 then
            tileToTilesInDrawing(selectedTile)
            selectedTile = (mouseY-y)*32 + (mouseX-x)
            tileInDrawingToTile(selectedTile)
            moveSelector2(mouseX,mouseY)
            print(selectedTile)
        end
    end
end

function printTile()
    for i=1,8 do
        for j=1,8 do
            if(tile[i][j] == 0) then
                io.write("0")
            else
                io.write("1")
            end
            io.write(", ")
            
        end
    end    
    print("")
    print("")
end
function loadSprites()
    -- under score 1
    loadInTile({16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 7, 7, 7, 7, 7, 7, 7, 7})
    spriteTileLoad(1,tile)
    setSpriteTileIndex(1,1)

    -- upper score 2
    loadInTile({7, 7, 7, 7, 7, 7, 7, 7, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16})
    spriteTileLoad(2,tile)
    setSpriteTileIndex(2,2)

    -- right score 3
    loadInTile({16, 16, 16, 16, 16, 16, 16, 7, 16, 16, 16, 16, 16, 16, 16, 7, 16, 16, 16, 16, 16, 16, 16, 7, 16, 16, 16, 16, 16, 16, 16, 7, 16, 16, 16, 16, 16, 16, 16, 7, 16, 16, 16, 16, 16, 16, 16, 7, 16, 16, 16, 16, 16, 16, 16, 7, 16, 16, 16, 16, 16, 16, 16, 7})
    spriteTileLoad(3,tile)
    setSpriteTileIndex(3,3)

    -- left score 4
    loadInTile({7, 16, 16, 16, 16, 16, 16, 16, 7, 16, 16, 16, 16, 16, 16, 16, 7, 16, 16, 16, 16, 16, 16, 16, 7, 16, 16, 16, 16, 16, 16, 16, 7, 16, 16, 16, 16, 16, 16, 16, 7, 16, 16, 16, 16, 16, 16, 16, 7, 16, 16, 16, 16, 16, 16, 16, 7, 16, 16, 16, 16, 16, 16, 16})
    spriteTileLoad(4,tile)
    setSpriteTileIndex(4,4)

    -- square 5
    loadInTile({0, 0, 0, 0, 0, 0, 0, 0, 0, 16, 16, 16, 16, 16, 16, 0, 0, 16, 16, 16, 16, 16, 16, 0, 0, 16, 16, 16, 16, 16, 16, 0, 0, 16, 16, 16, 16, 16, 16, 0, 0, 16, 16, 16, 16, 16, 16, 0, 0, 16, 16, 16, 16, 16, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0})
    spriteTileLoad(5,tile)
    setSpriteTileIndex(5,5)

    -- under score 6
    setSpriteTileIndex(6,1)
    -- upper score 7
    setSpriteTileIndex(7,2)
    -- right score 8
    setSpriteTileIndex(8,3)
    -- left score 9
    setSpriteTileIndex(9,4)
    -- square 10
    setSpriteTileIndex(10,5)

end
function moveSelector(x,y)
    x = x*8
    y = y*8
    -- under score 1
    setSpriteX(1,x)
    setSpriteY(1,y-8)
    -- upper score 2
    setSpriteX(2,x)
    setSpriteY(2,y+8)
    -- right score 3
    setSpriteX(3,x-8)
    setSpriteY(3,y)
    -- left score 4
    setSpriteX(4,x+8)
    setSpriteY(4,y)
    -- square 5
    setSpriteX(5,x)
    setSpriteY(5,y)

end
function moveSelector2(x,y)
    x = x*8
    y = y*8
    -- under score 1
    setSpriteX(6,x)
    setSpriteY(6,y-8)
    -- upper score 2
    setSpriteX(7,x)
    setSpriteY(7,y+8)
    -- right score 3
    setSpriteX(8,x-8)
    setSpriteY(8,y)
    -- left score 4
    setSpriteX(9,x+8)
    setSpriteY(9,y)
end

function loadInTile(data)
    for i=1,8 do
        for j=1,8 do
            tile[i][j] = data[(i-1)*8+j]
        end
    end
end

function tileToTilesInDrawing(index)
    for y=0,7 do
        for x=0,7 do
            tilesInDrawing[index+1][y+1][x+1] = tile[y+1][x+1]
        end
    end
end

function tileInDrawingToTile(index)
    for y=0,7 do
        for x=0,7 do
            tile[y+1][x+1] = tilesInDrawing[index+1][y+1][x+1]
        end
    end
end

function setupTilesInDrawing()
    for i=1,256 do
        tilesInDrawing[i] = {}
        for j=1,8 do
            tilesInDrawing[i][j] = {}
            for k=1,8 do
                tilesInDrawing[i][j][k] = 0
            end
        end
    end
end

function loadTilesBin()
    bin = loadTilesData("tiles.bin")
    local counter = 1
    for i=1,256 do
        for j=1,8 do
            for k=1,8 do
                tilesInDrawing[i][j][k] = bin[(i-1)*64+(j-1)*8+k]
                counter = counter + 1
            end
        end
    end
end

