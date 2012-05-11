@echo off

@SET FrameworkDir=c:\WINDOWS\Microsoft.NET\Framework
@SET FrameworkVersion=v2.0.50727

@set PATH=%VS8_HOME%\Common7\IDE;%VS8_HOME%\VC\BIN;%VS8_HOME%\Common7\Tools;%VS8_HOME%\Common7\Tools\bin;%VS8_HOME%\VC\PlatformSDK\bin;%VS8_HOME%\SDK\v2.0\bin;%FrameworkDir%\%FrameworkVersion%;%VS8_HOME%\VC\VCPackages;%PATH%

@set INCLUDE=%VS8_HOME%\VC\ATLMFC\INCLUDE;%VS8_HOME%\VC\INCLUDE;%VS8_HOME%\VC\PlatformSDK\include;%VS8_HOME%\SDK\v2.0\include;%INCLUDE%

@set LIB=%VS8_HOME%\VC\ATLMFC\LIB;%VS8_HOME%\VC\LIB;%VS8_HOME%\VC\PlatformSDK\lib;%VS8_HOME%\SDK\v2.0\lib;%LIB%

@set LIBPATH=%FrameworkDir%\%FrameworkVersion%;%VS8_HOME%\VC\ATLMFC\LIB


