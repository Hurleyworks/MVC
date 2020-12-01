
project "nanogui"
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
    
    
    flags { "MultiProcessorCompile" }
	
	targetdir ("../builds/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../builds/bin-int/" .. outputdir .. "/%{prj.name}")
	
	includedirs
	{
        "include",
		"include/nanogui",
		"ext/glad/include",
		"ext/nanovg/src",
		"../glfw/include/GLFW",
		"../glfw/include",
		"../stb/include"
    }

	files
	{
		"include/*.h",
		"include/**.h",
		"src/*.h",
		"ext/**.h",
        "src/*.cpp",
		"ext/**.cpp",
		"ext/**.c",
		"ext/**.cc",
    }
	
	filter "configurations:Release"
        optimize "On"
    
	filter "system:windows"
        systemversion "latest"
        staticruntime "On"
        disablewarnings { "4244", "4996" }
		characterset "MBCS"
        files
        {
           
        }

		defines 
		{ 
			"_WIN32",
			"_WINDOWS",
            "_CRT_SECURE_NO_WARNINGS", "NANOGUI_USE_OPENGL", "NANOGUI_GLAD",
		}
