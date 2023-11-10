workspace "Test"  
    configurations { "Debug", "Release" } 
    project "Test"  
    kind "ConsoleApp"   
    language "C++"   
    targetdir "bin/%{cfg.buildcfg}" 
    files {"./src/**.cpp" } 
    includedirs {"./src/", "E:/libraries/stb/"}
    filter "configurations:Debug"
    defines { "DEBUG" }  
    symbols "On" 
    filter "configurations:Release"  
    defines { "NDEBUG" }    
    optimize "On" 