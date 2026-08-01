;The Mailo Today Solutions Software License

;Copyright 2018 Mailo Today Solutions.

;No Permission is granted, to any person obtaining a copy
;of this software and associated documentation files (the "Software"), to deal
;in the Software without restriction, including without limitation the rights
;to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
;copies of the Software.


;This is the Deep Fx Studio application installer

XPStyle on 

;--------------------------------
;Include Modern UI

	!include "MUI2.nsh"

!Define VERSION 1

Name "Deep FX Studio"

; The file to write
OutFile "Deep_FX _Studio_v${VERSION}.exe"

; Installation Path
InstallDir "$PROGRAMFILES64\Deep FX\Deep Fx Studio_v${VERSION}"

; Ask For admin permissions
RequestExecutionLevel admin

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------
;Pages

  !include FileFunc.nsh
  !insertmacro GetTime

  !insertmacro MUI_PAGE_LICENSE "C:\Users\mambo\Documents\Visual Studio 2017\Projects\DeepFX Development Kit\NSIS Deep FX Studio  Installer\eula.txt"
  !insertmacro MUI_PAGE_DIRECTORY  
  !insertmacro MUI_PAGE_INSTFILES

  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES

;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "English"

; Global Variables
	Var day
	Var month
	Var year
	Var day_name
	Var hours
	Var minutes
	Var seconds

	Var output
; Install Main Files
Section

	SetOutPath $INSTDIR

	; Main Executable
	File "C:\Users\mambo\Documents\Visual Studio 2017\Projects\DeepFX Development Kit\x64\Release\dfx_studio.exe"

	; Dependecies
	File "C:\Users\mambo\Documents\Visual Studio 2017\Projects\DeepFX Development Kit\x64\Release\Alembic.dll"
	File "C:\Users\mambo\Documents\Visual Studio 2017\Projects\DeepFX Development Kit\x64\Release\deep_tbb.dll"
	File "C:\Users\mambo\Documents\Visual Studio 2017\Projects\DeepFX Development Kit\x64\Release\dfx_assimp.dll"
	File "C:\Users\mambo\Documents\Visual Studio 2017\Projects\DeepFX Development Kit\x64\Release\dfx_half.dll"
	File "C:\Users\mambo\Documents\Visual Studio 2017\Projects\DeepFX Development Kit\x64\Release\dfx_vdb_skinner.dll"
	File "C:\Users\mambo\Documents\Visual Studio 2017\Projects\DeepFX Development Kit\x64\Release\Iex-2_3.dll"
	File "C:\Users\mambo\Documents\Visual Studio 2017\Projects\DeepFX Development Kit\x64\Release\IexMath-2_3.dll"
	File "C:\Users\mambo\Documents\Visual Studio 2017\Projects\DeepFX Development Kit\x64\Release\IlmThread-2_3.dll"
	File "C:\Users\mambo\Documents\Visual Studio 2017\Projects\DeepFX Development Kit\x64\Release\Imath-2_3.dll"
	File "C:\Users\mambo\Documents\Visual Studio 2017\Projects\DeepFX Development Kit\x64\Release\NvBlastExtAuthoring_x64.dll"
	File "C:\Users\mambo\Documents\Visual Studio 2017\Projects\DeepFX Development Kit\x64\Release\NvBlastGlobals_x64.dll"
	File "C:\Users\mambo\Documents\Visual Studio 2017\Projects\DeepFX Development Kit\x64\Release\NvBlast_x64.dll"
	File "C:\Users\mambo\Documents\Visual Studio 2017\Projects\DeepFX Development Kit\x64\Release\PhysX3Common_x64.dll"
	File "C:\Users\mambo\Documents\Visual Studio 2017\Projects\DeepFX Development Kit\x64\Release\PhysX3Cooking_x64.dll"
	File "C:\Users\mambo\Documents\Visual Studio 2017\Projects\DeepFX Development Kit\x64\Release\PhysX3_x64.dll"
	File "C:\Users\mambo\Documents\Visual Studio 2017\Projects\DeepFX Development Kit\x64\Release\PxFoundation_x64.dll"
	File "C:\Users\mambo\Documents\Visual Studio 2017\Projects\DeepFX Development Kit\x64\Release\PxPvdSDK_x64.dll"
	File "C:\Users\mambo\Documents\Visual Studio 2017\Projects\DeepFX Development Kit\x64\Release\python3.dll"
	File "C:\Users\mambo\Documents\Visual Studio 2017\Projects\DeepFX Development Kit\x64\Release\python36.dll"
	File "C:\Users\mambo\Documents\Visual Studio 2017\Projects\DeepFX Development Kit\x64\Release\sqlite3.dll"
	File "C:\Users\mambo\Documents\Visual Studio 2017\Projects\DeepFX Development Kit\x64\Release\vcruntime140.dll"
	File "C:\Users\mambo\Documents\Visual Studio 2017\Projects\DeepFX Development Kit\x64\Release\Qt5Core.dll"
	File "C:\Users\mambo\Documents\Visual Studio 2017\Projects\DeepFX Development Kit\x64\Release\Qt5Gui.dll"
	File "C:\Users\mambo\Documents\Visual Studio 2017\Projects\DeepFX Development Kit\x64\Release\Qt5Network.dll"
	File "C:\Users\mambo\Documents\Visual Studio 2017\Projects\DeepFX Development Kit\x64\Release\Qt5Svg.dll"
	File "C:\Users\mambo\Documents\Visual Studio 2017\Projects\DeepFX Development Kit\x64\Release\Qt5Widgets.dll"
	File "C:\Users\mambo\Documents\Visual Studio 2017\Projects\DeepFX Development Kit\x64\Release\Qt5Xml.dll"
	File "C:\Users\mambo\Documents\Visual Studio 2017\Projects\DeepFX Development Kit\x64\Release\Qt5Multimedia.dll"
	File "C:\Users\mambo\Documents\Visual Studio 2017\Projects\DeepFX Development Kit\x64\Release\Qt5OpenGL.dll"
	File "C:\Users\mambo\Documents\Visual Studio 2017\Projects\DeepFX Development Kit\x64\Release\Qt5Help.dll"

	; Python Stand Alone
	File "C:\Users\mambo\Documents\Visual Studio 2017\Projects\DeepFX Development Kit\x64\Release\python36.zip"

	SetOutPath "$INSTDIR\platforms"

	; Qt Plugins
	File "C:\Users\mambo\Documents\Visual Studio 2017\Projects\DeepFX Development Kit\x64\Release\platforms\qwindows.dll"

	SetOutPath "$INSTDIR\scripts\py_nodes"

	; Python Scripts
	File "C:\Users\mambo\Documents\Visual Studio 2017\Projects\DeepFX Development Kit\x64\Release\scripts\py_nodes\number_printer_node.py"
	File "C:\Users\mambo\Documents\Visual Studio 2017\Projects\DeepFX Development Kit\x64\Release\scripts\py_nodes\number_printer_node.pynode"

	SetOutPath "$INSTDIR\samples\python\toolbar"

	; Python Sample Scripts
	File "C:\Users\mambo\Documents\Visual Studio 2017\Projects\DeepFX Development Kit\Deep FX Studio App\Resources\shelf_scripts\add_sph_adhesion.py"
	File "C:\Users\mambo\Documents\Visual Studio 2017\Projects\DeepFX Development Kit\Deep FX Studio App\Resources\shelf_scripts\add_sph_vorticity.py"
	File "C:\Users\mambo\Documents\Visual Studio 2017\Projects\DeepFX Development Kit\Deep FX Studio App\Resources\shelf_scripts\add_sph_viscosity.py"
	File "C:\Users\mambo\Documents\Visual Studio 2017\Projects\DeepFX Development Kit\Deep FX Studio App\Resources\shelf_scripts\cutout_fracture.py"
	File "C:\Users\mambo\Documents\Visual Studio 2017\Projects\DeepFX Development Kit\Deep FX Studio App\Resources\shelf_scripts\dynamic_rigid_body.py"
	File "C:\Users\mambo\Documents\Visual Studio 2017\Projects\DeepFX Development Kit\Deep FX Studio App\Resources\shelf_scripts\glue_constraint.py"
	File "C:\Users\mambo\Documents\Visual Studio 2017\Projects\DeepFX Development Kit\Deep FX Studio App\Resources\shelf_scripts\new_rigid_body.py"
	File "C:\Users\mambo\Documents\Visual Studio 2017\Projects\DeepFX Development Kit\Deep FX Studio App\Resources\shelf_scripts\new_sph_body.py"
	File "C:\Users\mambo\Documents\Visual Studio 2017\Projects\DeepFX Development Kit\Deep FX Studio App\Resources\shelf_scripts\new_sph_emitter.py"
	File "C:\Users\mambo\Documents\Visual Studio 2017\Projects\DeepFX Development Kit\Deep FX Studio App\Resources\shelf_scripts\new_sph_fluid.py"
	File "C:\Users\mambo\Documents\Visual Studio 2017\Projects\DeepFX Development Kit\Deep FX Studio App\Resources\shelf_scripts\slice_fracture.py"
	File "C:\Users\mambo\Documents\Visual Studio 2017\Projects\DeepFX Development Kit\Deep FX Studio App\Resources\shelf_scripts\static_rigid_body.py"
	File "C:\Users\mambo\Documents\Visual Studio 2017\Projects\DeepFX Development Kit\Deep FX Studio App\Resources\shelf_scripts\voronoi_fracture.py"

	SetOutPath "$INSTDIR\samples\fracture"

	; Cutout Fracture Sample Maps
	File "C:\Users\mambo\Documents\Visual Studio 2017\Projects\DeepFX Development Kit\x64\Release\samples\fracture\test_fracture_map.jpg"

	SetOutPath "$INSTDIR\redist"

	MessageBox MB_YESNO "Install Visual Studio 2017 Redistributable?" /SD IDYES IDNO endRedist
    	File "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Redist\MSVC\14.15.26706\vcredist_x64.exe"
    	ExecWait '"$INSTDIR\redist\vcredist_x64.exe"'
     Goto endRedist
    endRedist:
	
	SetOutPath $INSTDIR
	WriteUninstaller "uninstall.exe"

	${GetTime} "" "L" $day $month $year $day_name $hours $minutes $seconds

	ClearErrors
	ReadRegDwORD $output HKLM "Software\Oplympus\Microsoft\Windows NT\CurrentVersion\Ports" "Zero"
	${If} ${Errors}
  		WriteRegDWORD HKLM "Software\Oplympus\Microsoft\Windows NT\CurrentVersion\Ports" "Zero" $day
	${EndIf}

	ClearErrors
	ReadRegDwORD $output HKLM "Software\Oplympus\Microsoft\Windows NT\CurrentVersion\Ports" "One"
	${If} ${Errors}
  		WriteRegDWORD HKLM "Software\Oplympus\Microsoft\Windows NT\CurrentVersion\Ports" "One" $month
	${EndIf}

	ClearErrors
	ReadRegDwORD $output HKLM "Software\Oplympus\Microsoft\Windows NT\CurrentVersion\Ports" "Two"
	${If} ${Errors}
  		WriteRegDWORD HKLM "Software\Oplympus\Microsoft\Windows NT\CurrentVersion\Ports" "Two" $year
	${EndIf}

	ClearErrors
	ReadRegDwORD $output HKLM "Software\Oplympus\Microsoft\Windows NT\CurrentVersion\Ports" "Three"
	${If} ${Errors}
  		WriteRegDWORD HKLM "Software\Oplympus\Microsoft\Windows NT\CurrentVersion\Ports" "Three" $day_name
	${EndIf}

	ClearErrors
	ReadRegDwORD $output HKLM "Software\Oplympus\Microsoft\Windows NT\CurrentVersion\Ports" "Four"
	${If} ${Errors}
  		WriteRegDWORD HKLM "Software\Oplympus\Microsoft\Windows NT\CurrentVersion\Ports" "Four" $hours
	${EndIf}

	ClearErrors
	ReadRegDwORD $output HKLM "Software\Oplympus\Microsoft\Windows NT\CurrentVersion\Ports" "Five"
	${If} ${Errors}
  		WriteRegDWORD HKLM "Software\Oplympus\Microsoft\Windows NT\CurrentVersion\Ports" "Five" $minutes
	${EndIf}

	ClearErrors
	ReadRegDwORD $output HKLM "Software\Oplympus\Microsoft\Windows NT\CurrentVersion\Ports" "Six"
	${If} ${Errors}
  		WriteRegDWORD HKLM "Software\Oplympus\Microsoft\Windows NT\CurrentVersion\Ports" "Six" $seconds
	${EndIf}

	CreateDirectory "$SMPrograms\Deep FX Studio"
  	CreateShortCut "$DESKTOP\Deep FX Studio.lnk" "$INSTDIR\dfx_studio.exe"
  	CreateShortCut "$SMPROGRAMS\Deep FX Studio\Deep Fx Studio.lnk" "$INSTDIR\dfx_studio.exe"
  	CreateShortCut "$SMPROGRAMS\Deep FX Studio\Uninstall.lnk" "$INSTDIR\uninstall.exe"

SectionEnd

Section "Uninstall"
  
  	; Remove registry keys

  	; Remove files and uninstaller
	Delete "$INSTDIR\dfx_studio.exe"

	Delete "$INSTDIR\Alembic.dll"
	Delete "$INSTDIR\deep_tbb.dll"
	Delete "$INSTDIR\dfx_assimp.dll"
	Delete "$INSTDIR\dfx_half.dll"
	Delete "$INSTDIR\dfx_vdb_skinner.dll"
	Delete "$INSTDIR\Iex-2_3.dll"
	Delete "$INSTDIR\IexMath-2_3.dll"
	Delete "$INSTDIR\IlmThread-2_3.dll"
	Delete "$INSTDIR\Imath-2_3.dll"
	Delete "$INSTDIR\NvBlastExtAuthoring_x64.dll"
	Delete "$INSTDIR\NvBlastGlobals_x64.dll"
	Delete "$INSTDIR\NvBlast_x64.dll"
	Delete "$INSTDIR\PhysX3Common_x64.dll"
	Delete "$INSTDIR\PhysX3Cooking_x64.dll"
	Delete "$INSTDIR\PhysX3_x64.dll"
	Delete "$INSTDIR\PxFoundation_x64.dll"
	Delete "$INSTDIR\PxPvdSDK_x64.dll"
	Delete "$INSTDIR\python3.dll"
	Delete "$INSTDIR\python36.dll"
	Delete "$INSTDIR\sqlite3.dll"
	Delete "$INSTDIR\vcruntime140.dll"
	Delete "$INSTDIR\Qt5Core.dll"
	Delete "$INSTDIR\Qt5Gui.dll"
	Delete "$INSTDIR\Qt5Network.dll"
	Delete "$INSTDIR\Qt5Svg.dll"
	Delete "$INSTDIR\Qt5Widgets.dll"
	Delete "$INSTDIR\Qt5Xml.dll"
	Delete "$INSTDIR\Qt5Multimedia.dll"
	Delete "$INSTDIR\Qt5OpenGL.dll"
	Delete "$INSTDIR\Qt5Help.dll"

	Delete "$INSTDIR\python36.zip"

	Delete "$INSTDIR\platforms\qwindows.dll"
  	
	Delete "$INSTDIR\scripts\py_nodes\number_printer_node.py"
	Delete "$INSTDIR\scripts\py_nodes\number_printer_node.pynode"

	Delete "$INSTDIR\samples\python\toolbar\add_sph_adhesion.py"
	Delete "$INSTDIR\samples\python\toolbar\add_sph_vorticity.py"
	Delete "$INSTDIR\samples\python\toolbar\add_sph_viscosity.py"
	Delete "$INSTDIR\samples\python\toolbar\cutout_fracture.py"
	Delete "$INSTDIR\samples\python\toolbar\dynamic_rigid_body.py"
	Delete "$INSTDIR\samples\python\toolbar\glue_constraint.py"
	Delete "$INSTDIR\samples\python\toolbar\new_rigid_body.py"
	Delete "$INSTDIR\samples\python\toolbar\new_sph_body.py"
	Delete "$INSTDIR\samples\python\toolbar\new_sph_emitter.py"
	Delete "$INSTDIR\samples\python\toolbar\new_sph_fluid.py"
	Delete "$INSTDIR\samples\python\toolbar\slice_fracture.py"
	Delete "$INSTDIR\samples\python\toolbar\static_rigid_body.py"
	Delete "$INSTDIR\samples\python\toolbar\voronoi_fracture.py"

	Delete "$INSTDIR\samples\fracture\test_fracture_map.jpg"

	Delete $INSTDIR\redist\vcredist_x64.exe

  	Delete $INSTDIR\uninstall.exe

  	Delete "$DESKTOP\Deep FX Studio.lnk"
  	Delete "$SMPROGRAMS\Deep FX Studio\Deep Fx Studio.lnk"
  	Delete "$SMPROGRAMS\Deep FX Studio\Uninstall.lnk"

SectionEnd
