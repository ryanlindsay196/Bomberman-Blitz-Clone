workspace "MultiplayerProject"
	configurations { "Debug Client", "Debug Editor", "Debug Server", "Release Client", "Release Editor", "Release Server" }
	platforms { "x64", "x86" }

project "MultiplayerProject"
	kind "ConsoleApp"
	language "C++"
	includedirs { "$(SolutionDir)MultiplayerProject/Source/Headers", "$(SolutionDir)MultiplayerProject/Source/Vendor/Raknet/Include", "$(SolutionDir)MultiplayerProject/Source/Vendor/SDL/include", "$(SolutionDir)MultiplayerProject/Source/Vendor/SDL_image/include" , "$(SolutionDir)MultiplayerProject/Source/Vendor/Math/include", "$(SolutionDir)MultiplayerProject/Source/Vendor/RapidJSON/Include" }
	debugdir "$(SolutionDir)MultiplayerProject"

	files {"**.h", "**.cpp" }
	
	filter "configurations:Debug Client"
		defines { "DEBUG", "ClientMode" }
		symbols "On"
		targetdir "bin/Debug Client"

	filter "configurations:Debug Editor"
		defines { "DEBUG", "InEditor" }
		symbols "On"
		targetdir "bin/Debug Editor"
		
	filter "configurations:Debug Server"
		defines { "DEBUG", "ServerMode" }
		symbols "On"
		targetdir "bin/Debug Server"

	filter "configurations:Release Client"
		defines { "NDEBUG", "ClientMode" }
		symbols "On"
		targetdir "bin/Release Client"
		optimize "On"

	filter "configurations:Release Editor"
		defines { "NDEBUG", "InEditor" }
		symbols "On"
		targetdir "bin/Release Editor"
		optimize "On"

	filter "configurations:Release Server"
		defines { "NDEBUG", "ServerMode" }
		symbols "On"
		targetdir "bin/Release Server"
		optimize "On"

	vpaths {
		 ["Source Files/Headers/*"] = "MultiplayerProject/Headers/**.h",
		 ["Source Files/Classes/*"] = "MultiplayerProject/Classes/**.cpp"
		}

	filter "platforms:x86"
	postbuildcommands
	{
		"xcopy /y /d %[$(SolutionDir)MultiplayerProject/Source/Vendor/SDL/lib/x86/SDL2.dll] %[$(OutDir)]",
		"xcopy /y /d %[$(SolutionDir)MultiplayerProject/Source/Vendor/SDL_image/lib/x86/SDL2_image.dll] %[$(OutDir)]",
		"xcopy /y /d /e /i %[$(SolutionDir)MultiplayerProject/Resources] %[$(OutDir)/Resources]"
	}

filter { "configurations:*", "platforms:*" }
	links { "SDL2", "SDL2main", "SDL2_image", "ws2_32" }

filter "platforms:x86"
	libdirs { "$(SolutionDir)MultiplayerProject/Source/Vendor/SDL/lib/x86", "$(SolutionDir)MultiplayerProject/Source/Vendor/SDL_image/lib/x86" }
filter "platforms:x64"
	libdirs { "$(SolutionDir)MultiplayerProject/Source/Vendor/SDL/lib/x64", "$(SolutionDir)MultiplayerProject/Source/Vendor/SDL_image/lib/x64" }

function os.winSdkVersion()
    local reg_arch = iif( os.is64bit(), "\\Wow6432Node\\", "\\" )
    local sdk_version = os.getWindowsRegistry( "HKLM:SOFTWARE" .. reg_arch .."Microsoft\\Microsoft SDKs\\Windows\\v10.0\\ProductVersion" )
    if sdk_version ~= nil then return sdk_version end
end

	filter {"system:windows", "action:vs*"}
	    systemversion(os.winSdkVersion() .. ".0")