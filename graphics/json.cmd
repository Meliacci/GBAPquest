@echo off
for %%i in (*.bmp) do echo {"type": "sprite","height":16} > %%~ni.json