
project "cs_signal"
	if _ACTION == "vs2019" then
		cppdialect "C++17"
		location ("../builds/VisualStudio2019/projects")
	end
	if _ACTION == "vs2017" then
		location ("../builds/VisualStudio2017/projects")
	end
	if _ACTION == "vs2015" then
		location ("../builds/VisualStudio2015/projects")
	end
	
    kind "StaticLib"
    language "C++"
   
    flags { "MultiProcessorCompile"}
	targetdir ("../builds/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../builds/bin-int/" .. outputdir .. "/%{prj.name}")
	
	includedirs
	{
	
    }

	files
	{
		"source/*.h", 
		"source/*.hpp", 
		"source/*.cpp", 
    }
	
	filter "system:windows"
        staticruntime "On"
        systemversion "latest"
        disablewarnings { "4244", "4267", "4996", "4305", "4018", "4334", "4312", "4311", "4146"}
		defines { "_CRT_SECURE_NO_WARNINGS", "NOMINMAX"}

    filter "configurations:Debug"
        symbols "On"
                
    filter "configurations:Release"
        optimize "On"
	