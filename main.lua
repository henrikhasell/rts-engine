function engine.load()
    button = engine.createButton(100, 200, 300, 30, "Test #")
    button:setText("Hello, world!")

    -- function button:callback()
    --     print("Boop!")
    -- end

    ninja = engine.createMesh("models/ninja/ninja.b3d")

    ninja:addAnimationRange("walk", 1, 14)
    ninja:addAnimationRange("stealth walk", 15, 30)
    ninja:addAnimationRange("jump", 94, 102)
    ninja:addAnimationRange("attack1", 32, 44)
    ninja:addAnimationRange("idle1", 184, 205)
    ninja:addAnimationRange("idle2", 206, 250)
    ninja:addAnimationRange("idle3", 251, 300)
    ninja:addAnimationRange("backflip", 146, 158)
    ninja:addAnimationRange("climb", 159, 165)
    ninja:addAnimationRange("death1", 166, 173)
    ninja:addAnimationRange("death2", 174, 182)
    ninja:addAnimationRange("kick", 73, 83)

    ninja:setAnimationRange("idle2")
end

function engine.update(timeElapsed)

end

function engine.draw2D()
    button:draw()
end

function engine.draw3D(timeElapsed)
    ninja:draw(timeElapsed/100)
end

function engine.quit()

end

engine.load()
