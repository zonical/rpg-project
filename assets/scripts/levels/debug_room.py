from Engine import Engine

def OnLevelLoaded():
    print(Engine.OverworldState().OffsetCamera(100, 1000))