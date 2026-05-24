@echo off
for %%i in (*.bmp) do echo {"type": "sprite_tiles","height":16} > %%~ni.json